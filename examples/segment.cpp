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

#include <iostream>
#include <string>
using namespace std;

#include <sndfile.h>

#include "../source/Sirens.h"
#include "../source/string_support.h"
using namespace Sirens;

int main(int argc, char** argv) {
	if (argc < 2) {
		cerr << "Usage: segment [--save] file" << endl;
		return 1;
	} else {
		int save = 0;

		for (int i = 1; i < argc; i++) {
			if (string(argv[i]) == "--save")
				save = i;
		}

		/*----------------*
		 * 1. Open sound. *
		 *----------------*/
		cout << "1: Loading sound: " << argv[save + 1] << endl;

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
			loudness.parameters()->alpha = 0.15;
			loudness.parameters()->r = 0.0098;
			loudness.parameters()->cStayOff = 0.0015;
			loudness.parameters()->cTurnOn = 0.085;
			loudness.parameters()->cTurnOff = 0.085;
			loudness.parameters()->cNewSegment = 0.085;
			loudness.parameters()->cStayOn = 0.05;
			loudness.parameters()->pLagPlus = 0.75;
			loudness.parameters()->pLagMinus = 0.75;

			SpectralCentroid spectral_centroid(sound.getFrameCount(), sound.getSpectrumSize(), sound.getSampleRate());
			spectral_centroid.parameters()->alpha = 0.05;
			spectral_centroid.parameters()->r = 0.00000196;
			spectral_centroid.parameters()->cStayOff = 0.0000933506;
			spectral_centroid.parameters()->cTurnOn = 0.85;
			spectral_centroid.parameters()->cTurnOff = 0.85;
			spectral_centroid.parameters()->cNewSegment = 0.85;
			spectral_centroid.parameters()->cStayOn = 0.0025296018;
			spectral_centroid.parameters()->pLagPlus = 0.75;
			spectral_centroid.parameters()->pLagMinus = 0.075;

			SpectralSparsity spectral_sparsity(sound.getFrameCount());
			spectral_sparsity.parameters()->alpha = 0.05;
			spectral_sparsity.parameters()->r = 0.0196;
			spectral_sparsity.parameters()->cStayOff = 0.001833506;
			spectral_sparsity.parameters()->cTurnOn = 0.85;
			spectral_sparsity.parameters()->cTurnOff = 0.85;
			spectral_sparsity.parameters()->cNewSegment = 0.85;
			spectral_sparsity.parameters()->cStayOn = 0.009296018;
			spectral_sparsity.parameters()->pLagPlus = 0.75;
			spectral_sparsity.parameters()->pLagMinus = 0.75;

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

			Segmenter segmenter(0.00000000001, 0.00000000001, 5);
			segmenter.setFeatureSet(&feature_set);
			segmenter.segment();

			vector<vector<int> > segments = segmenter.getSegments();
			vector<int> modes = segmenter.getModes();

			if (segments.size() > 0) {
				cout << "5. CSV output (index, start frame, end frame):" << endl;

				for (int i = 0; i < segments.size(); i++) {
					cout << "\t" << i << "," << segments[i][0] << "," << segments[i][1] << ","
						<< segments[i][0] * sound.getSamplesPerHop() << ","
						<< segments[i][1] * sound.getSamplesPerHop() << endl;
				}
			} else
				cout << "5. No segments." << endl;

			/*-----------------------------------------------------------------*
			 * 5. Reload the sound and cut it up, saving the segments to disk. *
			 *-----------------------------------------------------------------*/

			if (save) {
				cout << "6. Saving segments to disk." << endl;

				if (segments.size() < 1)
					cout << "\tNo segments to save." << endl;
				else {
					cout << "\tSaving " << segments.size() << " segments." << endl;

					for (int i = 0; i < segments.size(); i++) {
						cout << "\t\tSaving segment " << i << "." << endl;
						sound.saveSegment("segment" + double_to_string(i) + ".wav", segments[i][0], segments[i][1]);
					}
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
