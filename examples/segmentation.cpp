/*
	Copyright 2009 Brandon Mechtley
	
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

#include <iostream>
using namespace std;

#include "../source/Sirens.h"
#include "../source/support/string_support.h"
using namespace Sirens;

int main(int argc, char** argv) {
	if (argc < 2) {
		cerr << "Usage: segemntation file" << endl;
		return 1;
	} else {
		// Open sound.
		Sound* sound = new Sound();
		sound->setFrameLength(0.04);
		sound->setHopLength(0.02);
		sound->open(argv[1]);
	
		cout << argv[1] << endl;
		cout << "\tSamples: " << sound->getSampleCount() << endl;
		cout << "\tFrames: " << sound->getFrameCount() << endl;
		cout << "\tSpectrum size: " << sound->getSpectrumSize() << endl;
		cout << "\tSample rate: " << sound->getSampleRate() << endl;
	
		// Initialize features.	
		Loudness* loudness = new Loudness(sound->getFrameCount());
		loudness->setMin(-60);
		loudness->setMax(0);
		loudness->getSegmentationParameters()->setAlpha(0.15);
		loudness->getSegmentationParameters()->setR(0.0098);
		loudness->getSegmentationParameters()->setCStayOff(0.0015);
		loudness->getSegmentationParameters()->setCTurnOn(0.085);
		loudness->getSegmentationParameters()->setCTurningOn(0.085);
		loudness->getSegmentationParameters()->setCTurnOff(0.085);
		loudness->getSegmentationParameters()->setCNewSegment(0.085);
		loudness->getSegmentationParameters()->setCStayOn(0.05);
		loudness->getSegmentationParameters()->setPLagPlus(0.75);
		loudness->getSegmentationParameters()->setPLagMinus(0.75);
	
		SpectralCentroid* spectral_centroid = new SpectralCentroid(sound->getFrameCount(), sound->getSpectrumSize(), sound->getSampleRate());
		spectral_centroid->setMin(0.4994);
		spectral_centroid->setMax(25.7848);
		spectral_centroid->getSegmentationParameters()->setAlpha(0.05);
		spectral_centroid->getSegmentationParameters()->setR(0.00000196);
		spectral_centroid->getSegmentationParameters()->setCStayOff(0.0000933506);
		spectral_centroid->getSegmentationParameters()->setCTurnOn(0.85);
		spectral_centroid->getSegmentationParameters()->setCTurningOn(0.85);
		spectral_centroid->getSegmentationParameters()->setCTurnOff(0.85);
		spectral_centroid->getSegmentationParameters()->setCNewSegment(0.85);
		spectral_centroid->getSegmentationParameters()->setCStayOn(0.0025296018);
		spectral_centroid->getSegmentationParameters()->setPLagPlus(0.75);
		spectral_centroid->getSegmentationParameters()->setPLagMinus(0.075);
	
		SpectralSparsity* spectral_sparsity = new SpectralSparsity(sound->getFrameCount());
		spectral_sparsity->setMin(0);
		spectral_sparsity->setMax(0.6509);
		spectral_sparsity->getSegmentationParameters()->setAlpha(0.05);
		spectral_sparsity->getSegmentationParameters()->setR(0.0196);
		spectral_sparsity->getSegmentationParameters()->setCStayOff(0.001833506);
		spectral_sparsity->getSegmentationParameters()->setCTurnOn(0.85);
		spectral_sparsity->getSegmentationParameters()->setCTurningOn(0.85);
		spectral_sparsity->getSegmentationParameters()->setCTurnOff(0.85);
		spectral_sparsity->getSegmentationParameters()->setCNewSegment(0.85);
		spectral_sparsity->getSegmentationParameters()->setCStayOn(.009296018);
		spectral_sparsity->getSegmentationParameters()->setPLagPlus(0.75);
		spectral_sparsity->getSegmentationParameters()->setPLagMinus(0.75);
	
		FeatureSet* feature_set = new FeatureSet();
		feature_set->addSampleFeature(loudness);
		feature_set->addSpectralFeature(spectral_centroid);
		feature_set->addSpectralFeature(spectral_sparsity);
	
		// Extract features.	
		sound->setFeatureSet(feature_set);
		sound->extractFeatures();	
		sound->close();
		
		// Segment sound.
		Segmenter* segmenter = new Segmenter(0.00000000001, 0.00000000001);
		segmenter->setFeatureSet(feature_set);
		segmenter->segment();
	
		vector<vector<int> > segments = segmenter->getSegments();
	
		// Output segments.
		cout << "\tSegments: " << endl;
	
		for (int i = 0; i < segments.size(); i++)
			cout << "\t\t" << i << ": " << segments[i][0] << "-" << segments[i][1] << endl;
		
		// Clean up.
		delete sound;
		delete segmenter;
		delete feature_set;
		delete loudness;
		delete spectral_centroid;
		delete spectral_sparsity;
	
		return 0;
	}
}
