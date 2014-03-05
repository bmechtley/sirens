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

#ifndef SIRENS_SIMPLESOUNDCOMPARATOR_H
#define SIRENS_SIMPLESOUNDCOMPARATOR_H

#include "SimpleFeatureComparator.h"

#include "../FeatureSet.h"

#include <vector>
using namespace std;

// Class for query-by-example on sounds.
namespace Sirens {
    class SimpleSoundComparator {
    private:
        FeatureSet* featureSet;

        // The sound's features' comparators.
        vector<SimpleFeatureComparator*> featureComparators;
        
        void freeMemory();
    public:
        SimpleSoundComparator(FeatureSet* feature_set = NULL);
        ~SimpleSoundComparator();
        
        void setFeatureSet(FeatureSet* feature_set);
        FeatureSet* getFeatureSet();
        
        vector<SimpleFeatureComparator*> getFeatureComparators();
        
        double compare(SimpleSoundComparator* sound_comparator);
        
        void initialize();
    };
}

#endif
