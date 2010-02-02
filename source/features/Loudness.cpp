/*
	Copyright 2007, 2008, 2009 Arizona State University
	
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

#include "Loudness.h"

#include <string>
using namespace std;

#include "../support/math_support.h"

namespace Sirens {
	void Loudness::performCalculation() {
		double rms_val=signal_rms(input);
		if (rms_val)
			value = 20 * log10(rms_val);
		else
			value = 20 * log10(10e-6);
	}

	string Loudness::toString() {
		return string("Loudness");
	}
}
