/*
    Copyright 2009 Arizona State University

    This file is part of Sirens.

    Sirens is free software: you can redistribute it and/or modify it under the
    terms of the GNU Lesser General Public License as  published by the Free
    Software Foundation, either version 3 of the License, or (at your option)
    any later version.

    Sirens is distributed in the hope that it will be useful, but WITHOUT ANY
    WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
    FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
    details.

    You should have received a copy of the GNU Lesser General Public License
    along with Sirens. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef SIRENS_CIRCULARARRAY_H
#define SIRENS_CIRCULARARRAY_H

#include <string>
using namespace std;

#include <pthread.h>

// Circular array allows values to be added and simply replace older values if
// the maximum size is reached.
namespace Sirens {
    class CircularArray {
    private:
        double* data;

        // Current size of the array.
        int size;

        // Maximum size before wrapping to the front of the array.
        int maxSize;

        // Total space allocated for the array (>= maxSize), for zero-padding.
        int allocatedSize;

        // Pointer to the start of the array. Not always zero.
        int start;

        // Pointer to the last element of the array.
        int index;

    public:
        CircularArray(int max_size = 1, int allocated_size = -1);
        ~CircularArray();

        void addValue(double value);

        int getSize();
        int getMaxSize();
        int getStart();
        int getIndex();

        double getUnorderedValue(int offset);
        double getValue(int offset);
        double getLatestValue();
        
        double* getData() {
            return data;
        }

        string toString();
    };
}

#endif
