/*
	Copyright 2009 Arizona State University
	
	This file is part of Sirens.
	
	Sirens is free software: you can redistribute it and/or modify it under the terms
	of the GNU Lesser General Public License as published by the Free Software
	Foundation, either version 3 of the License, or (at your option) any later version.
	
	Sirens is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
	without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
	PURPOSE.  See the GNU General Public License for more details.
	
	You should have received a copy of the GNU Lesser General Public License along
	with Sirens. If not, see <http://www.gnu.org/licenses/>.
*/

/*
	Loa a feature CSV file and segment it according to given parameters.
	Usage: segmentcsv features.csv parameters.csv [samples_per_hop=882]
	
	features.csv contains 1 row for each frame, one column for each feature.
	params.csv is formatted as follows:
		P(on), P(off)
		alpha_1,r_1,stayoff_1,turnon_1,turnoff_1,newsegment_1,stayon_1,lag+_1,lag-_1
		alpha_2,r_2,stayoff_2,turnon_2,turnoff_2,newsegment_2,stayon_2,lag+_2,lag-_2
		...
	
	samples_per_hop is just for output. The last two columns are segment start and
	end times in samples rather than feature frames.
*/

#include <iostream>
#include <string>
#include <fstream>
using namespace std;

#include <sndfile.h>

#include "../source/Sirens.h"
#include "../source/support/string_support.h"
using namespace Sirens;

int main(int argc, char** argv) {
	if (argc < 3) {
		cerr << "Usage: segmentcsv features parameters [samples_per_hop=882]" << endl;
		return 1;
	} else {
		double samples_per_hop = 882;
		
		if (argc > 3)
			samples_per_hop = string_to_double(argv[3]);
		
		// Load feature values. One line per frame.
	 	cout << "1: Loading features." << argv[1] << endl;
		
		vector<Feature*> features;
		vector<vector<double> > values;
		
		ifstream csvfile;
		csvfile.open(argv[1]);
		if (csvfile.is_open()) {
			while (csvfile.good()) {
				string line;
				vector<string> tokens;
				getline(csvfile, line);
				tokenise(line, tokens, ", ");
				
				if (values.size() < 1) {
					for (int i = 0; i < tokens.size(); i++)
						values.push_back(vector<double>());
				}
				
				for (int i = 0; i < tokens.size(); i++)
					values[i].push_back(string_to_double(tokens[i]));
			}
		}
		
		for (int i = 0; i < values.size(); i++) {
			Feature* feature = new Feature();
			feature->setMaxHistorySize(values[i].size());
			
			for (int j = 0; j < values[i].size(); j++)
				feature->addHistoryFrame(values[i][j]);
			
			features.push_back(feature);
		}
		
		csvfile.close();
			
		// Load segmentation parameters. One line per feature.
		cout << "2: Loading parameters." << argv[2] << endl;
		
		ifstream paramsfile;
		paramsfile.open(argv[2]);
		int i = 0;
		double pon = 0;
		double poff = 0;
		
		if (paramsfile.is_open()) {
			while (paramsfile.good()) {
				string line;
				vector<string> tokens;
				getline(paramsfile, line);
				tokenise(line, tokens, ", ");
				
				if (i == 0 && tokens.size() >= 2) {
					pon = string_to_double(tokens[0]);
					poff = string_to_double(tokens[1]);
				} else if (tokens.size() >= 9) {
					SegmentationParameters* params = features[i - 1]->getSegmentationParameters();
					params->setAlpha(string_to_double(tokens[0]));
					params->setR(string_to_double(tokens[1]));
					params->setCStayOff(string_to_double(tokens[2]));
					params->setCTurnOn(string_to_double(tokens[3]));
					params->setCTurnOff(string_to_double(tokens[4]));
					params->setCNewSegment(string_to_double(tokens[5]));
					params->setCStayOn(string_to_double(tokens[6]));
					params->setPLagPlus(string_to_double(tokens[7]));
					params->setPLagMinus(string_to_double(tokens[8]));
				}
				
				i++;
			}
		}
		
		paramsfile.close();
		
		cout << "3. Summary." << endl;
			
		for (int i = 0; i < features.size(); i++) {
			SegmentationParameters* params = features[i]->getSegmentationParameters();
			
			cout << "\tFeature " << i << endl <<
				"\t\t" << features[i]->getHistorySize() << " frames." << endl <<
				"\t\talpha: " << params->getAlpha() << endl <<
				"\t\tr: " << params->getR() << endl <<
				"\t\tstay off: " << params->getCStayOff() << endl <<
				"\t\tturn on: " << params->getCTurnOn() << endl <<
				"\t\tturn off: " << params->getCTurnOff() << endl <<
				"\t\tnew segment: " << params->getCNewSegment() << endl <<
				"\t\tstay on: " << params->getCStayOn() << endl <<
				"\t\tlag+: " << params->getPLagPlus() << endl <<
				"\t\tlag-: " << params->getPLagMinus() << endl << endl;
		}
		
		// Segment.
		cout << "4. Segmenting feature trajectory." << endl;
		
		FeatureSet feature_set;
		
		for (int i = 0; i < features.size(); i++)
			feature_set.addSampleFeature(features[i]);
		
		Segmenter segmenter(pon, poff);
		segmenter.setFeatureSet(&feature_set);
		segmenter.segment();
			
		vector<vector<int> > segments = segmenter.getSegments();
					
		if (segments.size() > 0) {
			cout << "5. CSV output (index, start frame, end frame, start sample, end sample):" << endl;
			
			for (int i = 0; i < segments.size(); i++) {
				cout << "\t" << i << "," 
					<< segments[i][0] << "," 
					<< segments[i][1] << ","
					<< segments[i][0] * samples_per_hop << "," 
					<< segments[i][1] * samples_per_hop << endl;
			}
		} else
			cout << "5. No segments." << endl;
	
		for (int i = 0; i < features.size(); i++)
			delete features[i];
	}
	
	return 0;
}
