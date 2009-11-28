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
#include <string>
using namespace std;

#include <sndfile.h>

#include "../source/Sirens.h"
#include "../source/support/string_support.h"
using namespace Sirens;

int main(int argc, char** argv) {
	if (argc < 2) {
		cerr << "Usage: segmentation file" << endl;
		return 1;
	} else {
		/*----------------*
		 * 1. Open sound. *
		 *----------------*/
		cout << "1: Loading sound: " << argv[1] << endl;
		
		Sound* sound = new Sound();
		sound->setFrameLength(0.04);
		sound->setHopLength(0.02);
		sound->open(argv[1]);
	
		cout << "\tDuration: " << sound->getSampleCount() / sound->getChannels() << " samples/channel (" 
			<< sound->getFrameCount() << " frames, " << 
			sound->getSampleCount() / (sound->getSampleRate() * sound->getChannels()) << "s)" << endl;
		cout << "\tChannels: " << sound->getChannels() << endl;
		cout << "\tSample rate: " << sound->getSampleRate() << " Hz" << endl;
		cout << "\tFrame size: " << sound->getSamplesPerFrame() / sound->getChannels() << " samples/channel (" << sound->getFrameLength() << "s)" << endl;
		cout << "\tHop size: " << sound->getSamplesPerHop() / sound->getChannels() << " samples/channel (" << sound->getHopLength() << "s)" << endl;
		cout << "\tSpectrum size: " << sound->getSpectrumSize() << " bins" << endl;
		
		/*--------------------------------------*
		 * 2. Initialize segmentation features. *
		 *--------------------------------------*/
		cout << "2. Initializing feature parameters." << endl;
		
		Loudness* loudness = new Loudness(sound->getFrameCount());
		loudness->setMin(-60);
		loudness->setMax(0);
		loudness->getSegmentationParameters()->setAlpha(0.15);
		loudness->getSegmentationParameters()->setR(0.0098);
		loudness->getSegmentationParameters()->setCStayOff(0.0015);
		loudness->getSegmentationParameters()->setCTurnOn(0.085);
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
		spectral_sparsity->getSegmentationParameters()->setCTurnOff(0.85);
		spectral_sparsity->getSegmentationParameters()->setCNewSegment(0.85);
		spectral_sparsity->getSegmentationParameters()->setCStayOn(.009296018);
		spectral_sparsity->getSegmentationParameters()->setPLagPlus(0.75);
		spectral_sparsity->getSegmentationParameters()->setPLagMinus(0.75);
	
		FeatureSet* feature_set = new FeatureSet();
		feature_set->addSampleFeature(loudness);
		feature_set->addSpectralFeature(spectral_centroid);
		feature_set->addSpectralFeature(spectral_sparsity);
	
		/*----------------------------------*
		 * 3. Extract feature trajectories. *
		 *----------------------------------*/
		cout << "3. Extracting features from sound." << endl;
		
		sound->setFeatureSet(feature_set);
		sound->extractFeatures();	
		sound->close();
		
		/*--------------------------------*
		 * 4. Get the segment boundaries. *
		 *--------------------------------*/
		cout << "4. Segmenting sound." << endl;
		
		Segmenter* segmenter = new Segmenter(0.00000000001, 0.00000000001);
		segmenter->setFeatureSet(feature_set);
		segmenter->segment();
		
		vector<vector<int> > segments = segmenter->getSegments();
		
		if (segments.size() > 0) {
			cout << "\tSegments ([start frame]-[end frame]): " << endl;
			
			for (int i = 0; i < segments.size(); i++)
				cout << "\t\t" << i << ": " << segments[i][0] << "-" << segments[i][1] << endl;
		} else
			cout << "\tNo segments." << endl;
		
		/*-----------------------------------------------------------------*
		 * 5. Reload the sound and cut it up, saving the segments to disk. *
		 *-----------------------------------------------------------------*/
		cout << "5. Saving segments to disk." << endl;
		
		// Sound information.
		SF_INFO sound_info;
		SNDFILE* sound_file = sf_open(argv[1], SFM_READ, &sound_info);
		SNDFILE* segment_file = NULL;
		int samples_per_hop = sound->getSamplesPerHop() * sound_info.channels;
		double* sound_data = new double[samples_per_hop];
		
		// Counters.
		int segment = 0;		
		int sound_frames = 0;
		int readcount = 0;
		bool new_segment = true;
		
		if (segments.size() < 1)
			cout << "\tNo segments to save." << endl;
		else {
			cout << "\tSaving " << segments.size() << " segments ([start sample]-[end sample]): " << endl;
			
			// Begin reading samples in from the file, one hop at a time.
			while((readcount = sf_read_double(sound_file, sound_data, samples_per_hop))) {
				// Increment the segment if we are past the previous segment's endpoint.
				if (sound_frames >= segments[segment][1] * samples_per_hop) {
					// Make sure there are segments left. If there aren't, we're done.
					if (segments.size() > (segment + 1)) {
						segment ++;

						if (segment_file != NULL) {
							sf_close(segment_file);
							segment_file = NULL;
							new_segment = true;
						}
					} else
						break;
				}
				
				// Open the segment file if we need to open a new one (either we just started, or we are on a segment boundary.)
				if (new_segment) {
					string filename = "segment" + double_to_string(segment) + ".wav";
					
					cout << "\tOpening " << filename << " (" << segments[segment][0] * samples_per_hop << "-" <<
						segments[segment][1] * samples_per_hop << ")." << endl;
					
					segment_file = sf_open(filename.c_str(), SFM_WRITE, &sound_info);
					new_segment = false;
				}
				
				// Only write to the file if we are past the segment's beginning point.
				if (sound_frames >= segments[segment][0] * samples_per_hop)
					sf_write_double(segment_file, sound_data, readcount);
				
				// Keep track of how many frames we've read total.
				sound_frames += readcount;
			}
		}
		
		/*--------------*
		 * 6. Clean up. *
		 *--------------*/
		
		if (segment_file != NULL)
			sf_close(segment_file);
		
		delete [] sound_data;
		
		delete sound;
		delete segmenter;
		delete feature_set;
		delete loudness;
		delete spectral_centroid;
		delete spectral_sparsity;
	
		return 0;
	}
}
