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

#include <iostream>
#include <fstream>
using namespace std;

#include "../source/Sirens.h"
#include "../source/support/matrix_support.h"
#include "../source/support/string_support.h"
using namespace Sirens;

#include <boost/numeric/ublas/io.hpp>

int main(int argc, char** argv) {
	if (argc < 2) {
		cerr << "Usage: retrieval file1 file2 . . . fileN" << endl;
		return 1;
	} else {
		// Open sound.
		vector<string> files;
		
		for (int i = 1; i < argc; i++)
			files.push_back(argv[i]);
		
		vector<string> tokens;
		tokenise(files[0], tokens, ".");
		string csvfn = tokens[0] + ".csv";
		
		/* 
			We have three of everything, because we have two files and 
			(unfortunately) features can't be shared.
		*/

		vector<Sound*> sounds(files.size());
		vector<Loudness*> loudness(files.size());
		vector<TemporalSparsity*> temporal_sparsity(files.size());
		vector<SpectralSparsity*> spectral_sparsity(files.size());
		vector<SpectralCentroid*> spectral_centroid(files.size());
		vector<TransientIndex*> transient_index(files.size());
		vector<Harmonicity*> harmonicity(files.size());
		vector<FeatureSet*> feature_sets(files.size());
		vector<SoundComparator*> comparators(files.size());
		
		// Initialize sounds
		Sound* sound = new Sound();
		sound->setFrameLength(0.04);
		sound->setHopLength(0.02);
		
		for (int i = 0; i < files.size(); i++) {
			cout << files[i] << " . . . ";
			
			// Initialize the sound file.
			sound->open(files[i]);
			
			/* 
				The first frame of TransientIndex is junk, so record everything 
				except the first frame.
			*/

			int frames = sound->getFrameCount() - 1;
			int spectrum_size = sound->getSpectrumSize();
			int sample_rate = sound->getSampleRate();
			
			// Initialize the features.
			loudness[i] = new Loudness(frames);
			temporal_sparsity[i] = new TemporalSparsity(frames);
			spectral_sparsity[i] = new SpectralSparsity(frames);

			transient_index[i] = new TransientIndex(
				frames, 
				spectrum_size, 
				sample_rate
			);
			
			spectral_centroid[i] = new SpectralCentroid(
				frames,
				spectrum_size, 
				sample_rate
			);
			
			harmonicity[i] = new Harmonicity(
				frames, 
				spectrum_size, 
				sample_rate
			);
			
			// Initialize the feature set.
			feature_sets[i] = new FeatureSet();
			feature_sets[i]->addSampleFeature(loudness[i]);
			feature_sets[i]->addSpectralFeature(spectral_centroid[i]);
			feature_sets[i]->addSpectralFeature(spectral_sparsity[i]);
			feature_sets[i]->addSampleFeature(temporal_sparsity[i]);
			feature_sets[i]->addSpectralFeature(transient_index[i]);
			feature_sets[i]->addSpectralFeature(harmonicity[i]);
			
			// Extract features.
			sound->setFeatureSet(feature_sets[i]);
			sound->extractFeatures();
			sound->close();
			
			comparators[i] = new SoundComparator(feature_sets[i]);
			
			cout << "done." << endl;
		}
		

		// Compare each sound to itself and the other sound.
		cout << "Comparing sounds . . . ";
		
		ublas::vector<double> likelihood(files.size());
		
		for (int i = 1; i < files.size(); i++)
			likelihood(i - 1) = comparators[0]->compare(comparators[i]);
		
		cout << "done." << endl << endl;
		
		// Save to CSV.
		cout << "Saving likelihood vector to " << csvfn << " . . . ";
		ofstream csvfile(csvfn.c_str());

		ublas::vector<double>::iterator likit;

		if (csvfile.is_open()) {
			for (likit = likelihood.begin(); likit != likelihood.end(); likit++) {
				csvfile << *likit << "\n";
			}

			csvfile.close();
		}
		
		cout << "done." << endl;
		
		// Clean up.
		delete sound;
		
		for (int i = 0; i < files.size(); i++) {
			delete comparators[i];
			delete feature_sets[i];
			delete loudness[i];
			delete spectral_sparsity[i];
			delete spectral_centroid[i];
			delete transient_index[i];
			delete harmonicity[i];
		}
		
		return 0;
	}
}
