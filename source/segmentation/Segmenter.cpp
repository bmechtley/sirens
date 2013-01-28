/*
	Copyright 2009 Arizona State University
	
	This file is part of Sirens.
	
	Sirens is free software: you can redistribute it and/or modify it under the terms 
	of the GNU Lesser General Public License as  published by the Free Software 
	Foundation, either version 3 of the License, or (at your option) any later version.
	
	Sirens is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
	without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
	PURPOSE.  See the GNU General Public License for more details.
	
	You should have received a copy of the GNU Lesser General Public License along
	with Sirens. If not, see <http://www.gnu.org/licenses/>.
*/

#include "Segmenter.h"

#include <algorithm>
#include <cmath>
using namespace std;

namespace Sirens {
	Segmenter::Segmenter(double p_new, double p_off, int beams) {
		setPNew(p_new);
		setPOff(p_off);
		setBeams(beams);
		
		progressCallback = NULL;
		featureSet = NULL;
		initialized = false;
	}
	
	Segmenter::~Segmenter() {
	}
	
	/*--------------------------------------*
	 * Helpers for indexing large matrices. *
	 *--------------------------------------*/
	
	// Return all which modes each feature (and global mode) are in for a particular state index.
	vector<int> Segmenter::getFeatureModes(int state) {
		vector<int> indices(features.size() + 1, 0);
		
		indices[0] = int(ceil(double(state) / double(states / 3)));
		
		for (int i = 0; i < features.size(); i++) {			
			int top_index = pow(3.0, double(features.size() - i));
			int bottom_index = pow(3.0, double(features.size() - i - 1));
						
			indices[i + 1] = ceil(double(state % top_index) / double(bottom_index));
						
			if (indices[i + 1] == 0)
				indices[i + 1] = 3;
		}
	
		return indices;
	}
	
	// Calculate the cost (function of error) for estimating the state of a feature with a Gaussian for a
	// particular state transition.
	double Segmenter::KalmanLPF(double y, double p[2][2], double x[2], double r, double q, double alpha) {
		double k[2];	// Kalman gain.
		double err;		// Error from the lowpass filter.
		double s;		// Residual variance from the Kalman filter.
		
		double oma = 1 - alpha;
		double oma2 = oma * oma;
		
		// Prediction.
		x[1] = (1 - alpha) * x[0] + alpha * x[1];
		
		// Prediction covariance.
		p[1][1] = p[0][0] * oma2 + 2 * p[0][1] * alpha * oma + p[1][1] * alpha * alpha + q * oma2;
		p[1][0] = p[0][0] * oma + p[1][0] * alpha + q * oma;
		p[0][1] = p[1][0];
		p[0][0] = p[0][0] + q;
		
		// Calculate lowpass filter error and Kalman filter residual variance.
		err = y - x[1];
		s = p[1][1] + r;
		
		// Calculate Kalman gain.
		k[0] = p[0][1] / s;
		k[1] = p[1][1] / s;
		
		// Update posterior estimate covariance.
		p[0][0] -= k[0] * p[0][1];
		p[1][0] -= k[0] * p[1][1];
		p[0][1] = p[1][0];
		p[1][1] -= k[1] * p[1][1];
		
		// Update estimate.
		x[0] += k[0] * err;
		x[1] += k[1] * err;
		
		// Total cost.
		return 0.5 * (log(s) + (err * err / s));
	}
	
	/*-------------*
	 * Algorithms. *
	 *-------------*/
	
	void Segmenter::viterbi(int frame) {
		partial_sort(oldCosts.begin(), oldCosts.begin() + beams, oldCosts.end());
		
		// Compute costs for all possible next states.
		for (int ni = 0; ni < states; ni++) {
			for (int beam = 0; beam < beams; beam++) {
				int oi = oldCosts[beam].index;
				
				double cost = 0;
				
				for (int fi = 0; fi < features.size(); fi++) {
					SegmentationParameters* parameters = features[fi]->parameters();
					
					newDistributions[fi][ni][oi].cost = KalmanLPF(
						y[fi],
						newDistributions[fi][ni][oi].covariance,
						newDistributions[fi][ni][oi].mean,
						parameters->r,
						parameters->q[modeMatrix[fi + 1][oi] - 1][modeMatrix[fi + 1][ni] - 1],
						parameters->alpha
					);
					
					cost += newDistributions[fi][ni][oi].cost;
				}
				
				costs[ni][oi] = oldCosts[beam].cost + cost - probabilityMatrix[ni][oi];
			}
		}
		
		// Find the next element with least cost along each path.
		for (int i = 0; i < states; i++) {
			vector<double>::iterator minimum_iterator = min_element(costs[i].begin(), costs[i].end());
			psi[frame][i] = distance(costs[i].begin(), minimum_iterator);
			oldCosts[i].cost = *minimum_iterator;
			oldCosts[i].index = i;
		}
		
		// Copy best filtered distributions.
		for (int i = 0; i < states; i++) {
			for (int f = 0; f < features.size(); f++) {
				for (int row = 0; row < 2; row++) {
					maxDistributions[f][i].mean[row] = newDistributions[f][i][psi[frame][i]].mean[row];
					
					for (int column = 0; column < 2; column++)
						maxDistributions[f][i].covariance[row][column] = newDistributions[f][i][psi[frame][i]].covariance[row][column];					
				}
			}
		}
		
		// Copy best filtered distributions as input distributions to next frame.
		for (int i = 0; i < states; i++) {
			for (int j = 0; j < states; j++) {
				for (int f = 0; f < features.size(); f++) {
					for (int row = 0; row < 2; row ++) {
						newDistributions[f][i][j].mean[row] = maxDistributions[f][i].mean[row];
						
						for (int column = 0; column < 2; column++)
							newDistributions[f][i][j].covariance[row][column] = maxDistributions[f][i].covariance[row][column];
					}
				}
			}
		}
	}
	
	/*-----------*
	 * Features. *
	 *-----------*/
	
	void Segmenter::setFeatureSet(FeatureSet* feature_set) {
		featureSet = feature_set;
		features = featureSet->getFeatures();
	}
	
	FeatureSet* Segmenter::getFeatureSet() {
		return featureSet;
	}
	
	/*-----------------*
	 * Initialization. *
	 *-----------------*/
	
	// Create transition probability matrix for switching between global modes.
	void Segmenter::createModeLogic() {
		vector<double> row(3, 0);
		modeTransitions = vector<vector<double> >(3, row);
		
		// Off.
		modeTransitions[0][0] = 1.0 - pNew;
		modeTransitions[0][1] = pNew;
		modeTransitions[0][2] = 0;
		
		// Onset.
		modeTransitions[1][0] = 0;
		modeTransitions[1][1] = 0;
		modeTransitions[1][2] = 1;
		
		// On.
		modeTransitions[2][0] = pOff - pOff * pNew;
		modeTransitions[2][1] = pNew;
		modeTransitions[2][2] = 1.0 - pNew - pOff + pOff * pNew;
	}
		
	// Create prior probability table for switching between all pairs of states.
	void Segmenter::createProbabilityTable() {
		int mold, mnew, fmold, fmnew;
		double gate;
		
		// Create the matrix that defines feature modes (plus global mode) for each possible state index.
		vector<int> int_row(states);
		modeMatrix = vector<vector<int> >(int(features.size() + 1), int_row);
		
		for (int i = 0; i < states; i++) {
			vector<int> indices = getFeatureModes(i + 1);
			
			for (int j = 0; j < features.size() + 1; j++)
				modeMatrix[j][i] = indices[j];
		}
		
		// Create the prior probability matrix for every possible state transition.
		vector<double> double_row(states);
		probabilityMatrix = vector<vector<double> >(states, double_row);
		
		for (int i = 0; i < states; i++) {
			for (int j = 0; j < states; j++) {
				mold = modeMatrix[0][i];
				mnew = modeMatrix[0][j];
				gate = 1.0;
				
				for (int k = 0; k < features.size(); k++) {
					fmold = modeMatrix[k + 1][i];
					fmnew = modeMatrix[k + 1][j];
					
					gate *= features[k]->parameters()->fusion[mold - 1][mnew - 1][fmold - 1][fmnew - 1];
				}
				
				probabilityMatrix[j][i] = log(modeTransitions[mold - 1][mnew - 1] * gate);
			}
		}
	}
	
	// Initialize everything.
	void Segmenter::initialize() {
		if (!initialized) {
			// Initialize prior distributions.
			for (int i = 0; i < features.size(); i++)
				features[i]->parameters()->initialize();
			
			states = pow(3.0, double(features.size() + 1));
			
			if (beams == -1)
				beams = states;
			
			createModeLogic();
			createProbabilityTable();
			
			// Initialize global mode sequence (on/off/onset for each frame).
			modes = vector<int>(frames, 0);
			
			// Initialize cost vectors used by Viterbi.
			vector<double> cost_vector = vector<double>(states, 0);
			costs = vector<vector<double> >(states, cost_vector);
			oldCosts = vector<CostIndex>(states);
				
			// Best state transitions for each state in each frame.
			vector<int> psi_row = vector<int>(states, 0);
			psi = vector<vector<int> >(frames, psi_row);
			
			// Initialize Gaussians used by Viterbi.
			vector<ViterbiDistribution> temp1(states);
			maxDistributions = vector<vector<ViterbiDistribution> >(features.size(), temp1);
			
			vector<vector<ViterbiDistribution> > temp2(states, temp1);
			newDistributions = vector<vector<vector<ViterbiDistribution> > >(features.size(), temp2);
			
			for (int i = 0; i < features.size(); i++) {
				for (int a = 0; a < 2; a++) {
					maxDistributions[i][0].mean[a] = features[i]->parameters()->xInit[a];
					
					for (int b = 0; b < 2; b++)
						maxDistributions[i][0].covariance[a][b] = features[i]->parameters()->pInit[a][b];
				}
			}
			
			// Initialize feature vector for current frame.
			y = vector<double>(features.size(), 0);
			
			initialized = true;
		}
	}
	
	
	/*---------------*
	 * Segmentation. *
	 *---------------*/
	
	void Segmenter::segment() {
		if (featureSet != NULL) {
			frames = featureSet->getMinHistorySize();
			
			initialize();
			
			// For each frame, perform Viterbi and get the optimal state sequence.
			for (int i = 0; i < frames; i++) {
				if (progressCallback != NULL)
					progressCallback(i, frames);
				
				for (int j = 0; j < features.size(); j++)
					y[j] = features[j]->getHistoryFrame(i);
				
				viterbi(i);
			}
			
			vector<int> state_sequence(frames, 0);
			
			// Find the next state with the least cost and choose it to assign to the state of the last frame.
			vector<CostIndex>::iterator minimum_cost = min_element(oldCosts.begin(), oldCosts.end());
			state_sequence[frames - 1] = distance(oldCosts.begin(), minimum_cost);
			
			// Find the "off" states and "on"/"onset" states that have minimum cost. 
			int start = (states / 3);
			
			// Traverse the state transitions backward from the last frame's optimal mode to get the state sequence.
			for (int i = frames - 2; i > -1; i--)
				state_sequence[i] = psi[i][state_sequence[i + 1]];
			
			// Find the mode sequence.
			for (int i = 0; i < frames; i++)
				modes[i] = modeMatrix[0][state_sequence[i]];
		}
	}
	
	
	/*---------------------*
	 * After segmentation. *
	 *---------------------*/
	
	vector<vector<int> > Segmenter::getSegments() {
		vector<vector<int> > segments;
		
		// If the last frame is an onset, ignore it and just make it whatever the previous frame was.
		for (int i = 0; i < modes.size(); i++) {
			if (i < modes.size() - 1) {
				if ((i == 0 && modes[i] == 3) || (modes[i] == 2)) {
					vector<int> segment(2, 0);
					segment[0] = i;
					
					for (int j = i; j < modes.size(); j++) {
						if ((modes[j] == 1) || (j == modes.size() - 1)) {
							segment[1] = j;
							break;
						}
					}
					
					segments.push_back(segment);
				}
			}
		}
		
		return segments;
	}
	
	vector<int> Segmenter::getModes() {
		return modes;
	}
}
