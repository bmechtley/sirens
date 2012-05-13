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

#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <exception>
#include <string>
using namespace std;

class IOException : public exception {
};

class AnalysisException : public exception {
};

//End of File
class UnrecognizedFormatException : public IOException {
	virtual const char* what() const throw() {
    	return "Unrecognized sound file format.";
  	}
};

class SystemException : public IOException {
	virtual const char* what() const throw() {
    	return "System error loading sound file.";
  	}
};

class MalformedFileException : public IOException {
	virtual const char* what() const throw() {
    	return "Malformed sound file.";
  	}
};

class UnsupportedEncodingException : public IOException {
	virtual const char* what() const throw() {
    	return "Unsupported encoding in sound file.";
  	}
};

class SoundNotLoadedException : public AnalysisException {
	virtual const char* what() const throw() {
		return "Sound file not loaded.";
	}
};

#endif
