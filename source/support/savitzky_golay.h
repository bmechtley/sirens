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


#ifndef __SAVITZKYGOLAY_H__
#define __SAVITZKYGOLAY_H__

#include "boost/numeric/ublas/vector.hpp"
using namespace boost::numeric;

// First two differentiation filters for a 4th order Sazitzky-Golay filter for 41 frames
namespace Sirens {
	static const int savitzky_golay_k = 4;
	static const int savitzky_golay_f = 41;
	
	void fill_savitzky_golay(ublas::vector<double>& first, ublas::vector<double>& second);
}
#endif
