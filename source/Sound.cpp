/*
	Copyright 2008, 2009 Brandon Hawley
	
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

#include "Sound.h"

#include "FFT.h"
#include "CircularArray.h"
#include "support/math_support.h"
#include "support/string_support.h"

#include "Exceptions.h"

void throw_io_error(SNDFILE* file) throw (IOException) {
	int error = sf_error(file);
	
	if (error == 1)
		throw UnrecognizedFormatException();
	else if (error == 2)
		throw SystemException();
	else if (error == 3)
		throw MalformedFileException();
	else if (error == 4)
		throw UnsupportedEncodingException();
}

namespace Sirens {
	Sound::Sound() {
		frameLength = 0.04;
		hopLength = 0.02;
		channelOption = 0;
		
		soundFile = NULL;
		path = "";
	}
	
	Sound::Sound(string path_in) {
		frameLength = 0.04;
		hopLength = 0.02;
		channelOption = 0;
		soundFile = NULL;
		
		open(path_in);
	}
	
	Sound::~Sound() {
		close();
	}
	
	/*-----*
	 * IO. *
	 *-----*/
	
	void Sound::open(string path_in) throw (IOException) {
		path = path_in;
		
		soundFile = sf_open(path.c_str(), SFM_READ, &soundInfo);
		
		if (!soundFile)
			throw_io_error(soundFile);
	}
	
	void Sound::saveSegment(string path_out, int start_frame, int end_frame) throw (IOException) {
		if (soundFile != NULL) {			
			// Sample buffer is the size of one hop * #channels.
			int samples_per_hop = getSamplesPerHop() * getChannels();
			int samples_per_frame = getSamplesPerFrame() * getChannels();
			double* hop_samples = new double[samples_per_hop];
			
			// Open the segment file.
			SNDFILE* segment = sf_open(path_out.c_str(), SFM_WRITE, &soundInfo);
			
			if (segment) {
				// Get the last frame in its entirety.
				int first_sample = samples_per_hop * start_frame;
				int last_sample = samples_per_hop * (end_frame - 1) + samples_per_frame;
				
				int samples_read = 0;
				int readcount = 0;
			
				// Reset the file pointer.
				sf_seek(soundFile, 0, SEEK_SET);
			
				// Begin reading data from the sound, one hop at a time.
				while (readcount = sf_read_double(soundFile, hop_samples, samples_per_hop)) {					
					if (samples_read >= first_sample && samples_read < last_sample) {
						int samples_to_write = samples_per_hop;
						int samples_left = last_sample - samples_read;
					
						if (samples_left < samples_per_hop)
							samples_to_write = samples_left;
					
						sf_write_double(segment, hop_samples, samples_per_hop);
					} else if (samples_read >= last_sample)
						break;
				
					samples_read += readcount;
				}
			
				sf_close(segment);
			}
		} else
			throw SoundNotLoadedException();
	}
	
	void Sound::close() throw (IOException) {
		if (soundFile != NULL) {
			if (sf_close(soundFile) != 0)
				throw_io_error(soundFile);
			
			soundFile = NULL;
		}
	}
	
	/*--------------------------*
	 * Basic sound information. *
	 *--------------------------*/
	
	int Sound::getSampleCount() {
		return soundInfo.frames;
	}

	int Sound::getSampleRate() {
		return soundInfo.samplerate;
	}

	int Sound::getChannels() {
		return soundInfo.channels;
	}
	
	double Sound::getHopLength() {
		return hopLength;
	}
	
	double Sound::getFrameLength() {
		return frameLength;
	}
	
	void Sound::setHopLength(double hop_length) {
		hopLength = hop_length;
	}
	
	void Sound::setFrameLength(double frame_length) {
		frameLength = frame_length;
	}
	
	void Sound::setChannelOption(int channel_option) {
		if (channel_option > soundInfo.channels || channel_option < 0)
			channelOption = 0;
		else
			channelOption = channel_option;
	}
	
	int Sound::getChannelOption() {
		return channelOption;
	}
	
	string Sound::getPath() {
		return path;
	}
	
	/*-------------------------------*
	 * Calculated sound information. *
	 *-------------------------------*/
	
	int Sound::getSamplesPerFrame() {
		return int(frameLength * double(getSampleRate()));
	}
	
	int Sound::getSamplesPerHop() {
		return int(hopLength * double(getSampleRate()));
	}
	
	int Sound::getFrameCount() {
		int frames = getSampleCount() / getSamplesPerHop();
		
		if (getSampleCount() % getSamplesPerHop() < getSamplesPerFrame())
			frames --;
		
		return frames;
	}
	
	int Sound::getFFTSize() {
		return next_pow(getSamplesPerFrame(), 2);
	}
	
	int Sound::getSpectrumSize() {
		return getFFTSize() / 2 + 1;
	}
	
	/*-----------*
	 * Features. *
	 *-----------*/
	
	FeatureSet* Sound::getFeatureSet() {
		return featureSet;
	}
	
	void Sound::setFeatureSet(FeatureSet* feature_set) {
		featureSet = feature_set;
	}
	
	void Sound::extractFeatures() throw (AnalysisException) {
		if (soundFile != NULL) {
			CircularArray sample_array(getSamplesPerFrame());					// Samples of the current frame.
			CircularArray windowed_array(getSamplesPerFrame(), getFFTSize());	// Windowed samples of the current frame, pad with 0s for STFT.
			CircularArray spectrum_array(getSpectrumSize());					// STFT spectrum magnitudes of the current frame.
		
			// Hamming window for STFT.
			double* window = create_hamming_window(getSamplesPerFrame());
		
			FFT fft(getFFTSize(), windowed_array.getData());
		
			// Start reading in frames.
			int readcount = 0;
	 		long frame_number = 0;
			int samples_per_hop = getSamplesPerHop() * getChannels();
			double* hop_samples = new double[samples_per_hop];
		
			for (int i = 0; i < samples_per_hop; i++)
				hop_samples[i] = 0;
		
			// Reset the file pointer.
			sf_seek(soundFile, 0, SEEK_SET);
		
			while (readcount = sf_read_double(soundFile, hop_samples, samples_per_hop)) {
				// Similar to the FFT, it's necessary to copy read values element-by-element to allow CircularArray to be thead-safe.
				double* sample_value = hop_samples;
				double average_sample = 0;
			
				// if channelOption == 0, samples will be averaged. Otherwise, the channelOption'th sample will be used.
				if (channelOption) {
					for (int i = 0; i < (channelOption - 1); i++)
						sample_value ++; 
					for (int i = (channelOption - 1); i < readcount; i += soundInfo.channels) {
						sample_array.addValue(*sample_value);
					
						for (int j = 0; j < soundInfo.channels; j++)
							sample_value ++;
					}
				} else {
					for (int i = 0; i < readcount; i += soundInfo.channels) {
						average_sample = 0;
					
						for (int j = 0; j < soundInfo.channels; j++) {
							average_sample += *sample_value;
							sample_value ++;
						}
					
						average_sample /= double(soundInfo.channels);
						sample_array.addValue(average_sample);
					}
				}
			
				// The first hop or two will not necessarily be a full frame's worth of data.
				if (sample_array.getSize() == sample_array.getMaxSize() && readcount == samples_per_hop) {
					// Calculate sample features.
					featureSet->calculateSampleFeatures(&sample_array);
				
					// Window the time-domain signal for STFT.
					for (int i = 0; i < getSamplesPerFrame(); i++)
						windowed_array.addValue(sample_array.getValue(i) * window[i]);
				
					// Perform STFT.
					fft.calculate();
				
					for (int i = 0; i < fft.getOutputSize(); i++) {		
						double first = fft.getOutput()[i][0];
						double second = fft.getOutput()[i][1];
					
						spectrum_array.addValue(sqrt(first * first + second * second));
					}
				
					// Calculate spectral features.
					featureSet->calculateSpectralFeatures(&spectrum_array);
				
					frame_number = frame_number + 1;
				}
			}
		
			// Cleanup.
			delete [] hop_samples;
			delete [] window;
		} else
			throw SoundNotLoadedException();
	}
}
