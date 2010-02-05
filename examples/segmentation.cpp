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
		
		Sound sound;
		sound.setFrameLength(0.04);
		sound.setHopLength(0.02);
		
		try {
			sound.open(argv[1]);
			
			cout << "\tDuration: " << sound.getSampleCount() / sound.getChannels() << " samples/channel (" 
				<< sound.getFrameCount() << " frames, " << 
				sound.getSampleCount() / (sound.getSampleRate() * sound.getChannels()) << "s)" << endl;
			cout << "\tChannels: " << sound.getChannels() << endl;
			cout << "\tSample rate: " << sound.getSampleRate() << " Hz" << endl;
			cout << "\tFrame size: " << sound.getSamplesPerFrame() / sound.getChannels() << " samples/channel (" << sound.getFrameLength() << "s)" << endl;
			cout << "\tHop size: " << sound.getSamplesPerHop() / sound.getChannels() << " samples/channel (" << sound.getHopLength() << "s)" << endl;
			cout << "\tSpectrum size: " << sound.getSpectrumSize() << " bins" << endl;
			
			/*--------------------------------------*
			 * 2. Initialize segmentation features. *
			 *--------------------------------------*/
			cout << "2. Initializing feature parameters." << endl;
			
			Loudness loudness(sound.getFrameCount());
			loudness.setMin(-60);
			loudness.setMax(0);
			loudness.getSegmentationParameters()->setAlpha(0.15);
			loudness.getSegmentationParameters()->setR(0.0098);
			loudness.getSegmentationParameters()->setCStayOff(0.0015);
			loudness.getSegmentationParameters()->setCTurnOn(0.085);
			loudness.getSegmentationParameters()->setCTurnOff(0.085);
			loudness.getSegmentationParameters()->setCNewSegment(0.085);
			loudness.getSegmentationParameters()->setCStayOn(0.05);
			loudness.getSegmentationParameters()->setPLagPlus(0.75);
			loudness.getSegmentationParameters()->setPLagMinus(0.75);
			
			SpectralCentroid spectral_centroid(sound.getFrameCount(), sound.getSpectrumSize(), sound.getSampleRate());
			spectral_centroid.setMin(0.4994);
			spectral_centroid.setMax(25.7848);
			spectral_centroid.getSegmentationParameters()->setAlpha(0.05);
			spectral_centroid.getSegmentationParameters()->setR(0.00000196);
			spectral_centroid.getSegmentationParameters()->setCStayOff(0.0000933506);
			spectral_centroid.getSegmentationParameters()->setCTurnOn(0.85);
			spectral_centroid.getSegmentationParameters()->setCTurnOff(0.85);
			spectral_centroid.getSegmentationParameters()->setCNewSegment(0.85);
			spectral_centroid.getSegmentationParameters()->setCStayOn(0.0025296018);
			spectral_centroid.getSegmentationParameters()->setPLagPlus(0.75);
			spectral_centroid.getSegmentationParameters()->setPLagMinus(0.075);
			
			SpectralSparsity spectral_sparsity(sound.getFrameCount());
			spectral_sparsity.setMin(0);
			spectral_sparsity.setMax(0.6509);
			spectral_sparsity.getSegmentationParameters()->setAlpha(0.05);
			spectral_sparsity.getSegmentationParameters()->setR(0.0196);
			spectral_sparsity.getSegmentationParameters()->setCStayOff(0.001833506);
			spectral_sparsity.getSegmentationParameters()->setCTurnOn(0.85);
			spectral_sparsity.getSegmentationParameters()->setCTurnOff(0.85);
			spectral_sparsity.getSegmentationParameters()->setCNewSegment(0.85);
			spectral_sparsity.getSegmentationParameters()->setCStayOn(0.009296018);
			spectral_sparsity.getSegmentationParameters()->setPLagPlus(0.75);
			spectral_sparsity.getSegmentationParameters()->setPLagMinus(0.75);
			
			FeatureSet feature_set;
			feature_set.addSampleFeature(&loudness);
			feature_set.addSpectralFeature(&spectral_centroid);
			feature_set.addSpectralFeature(&spectral_sparsity);
			
			/*----------------------------------*
			 * 3. Extract feature trajectories. *
			 *----------------------------------*/
			cout << "3. Extracting features from sound." << endl;
			
			sound.setFeatureSet(&feature_set);
			sound.extractFeatures();	
			
			/*--------------------------------*
			 * 4. Get the segment boundaries. *
			 *--------------------------------*/
			cout << "4. Segmenting sound." << endl;
			
			Segmenter segmenter(0.00000000001, 0.00000000001);
			segmenter.setFeatureSet(&feature_set);
			segmenter.segment();
			
			vector<vector<int> > segments = segmenter.getSegments();
			vector<int> modes = segmenter.getModes();
			
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
			
			if (segments.size() < 1)
				cout << "\tNo segments to save." << endl;
			else {
				cout << "\tSaving " << segments.size() << " segments." << endl;
				
				for (int i = 0; i < segments.size(); i++) {
					cout << "\t\tSaving segment " << i << "." << endl;
					sound.saveSegment("segment" + double_to_string(i) + ".wav", segments[i][0], segments[i][1]);
				}
			}
			
			/*--------------*
			 * 6. Clean up. *
			 *--------------*/
			
			sound.close();
		} catch (exception& e) {
			cerr << e.what() << endl;
		}
	}
	
	return 0;
}
