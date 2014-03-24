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

#include "CircularArray.h"

#include "math_support.h"
#include "string_support.h"

namespace Sirens {
    CircularArray::CircularArray(int max_size, int allocated_size) {
        start = 0;
        size = 0;
        index = 0;
        maxSize = max_size;

        if (allocated_size == -1)
            allocated_size = maxSize;

        data = new double[allocated_size];

        for (int i = 0; i < allocated_size; i++)
            data[i] = 0;
    }

    CircularArray::~CircularArray() {
        delete [] data;
    }

    void CircularArray::addValue(double value) {
        data[index] = value;

        if (size == maxSize)
            start = (start + 1) % maxSize;
        else
            size ++;

        index = (index + 1) % maxSize;
    }

    int CircularArray::getSize() {
        return size;
    }

    int CircularArray::getMaxSize() {
        return maxSize;
    }

    int CircularArray::getStart() {
        return start;
    }

    int CircularArray::getIndex() {
        return index;
    }

    double CircularArray::getValue(int offset) {
        return data[(start + offset) % maxSize];
    }
    
    double CircularArray::getUnorderedValue(int offset) {
        return data[offset % maxSize];
    }
    
    string CircularArray::toString() {
        string history_string;

        history_string =
            "Max size: " + double_to_string(maxSize) +
            ", Size: " + double_to_string(size) +
            ", Start: " + double_to_string(start) + "\n\t";

        for (int i = 0; i < size; i++)
            history_string += double_to_string(getValue(i)) + " ";

        return history_string;
    }
}
