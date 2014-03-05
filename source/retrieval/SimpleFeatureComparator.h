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

#ifndef SIRENS_SIMPLEFEATURECOMPARATOR_H
#define SIRENS_SIMPLEFEATURECOMPARATOR_H

#include "../Feature.h"

namespace Sirens {
    
    // Class that creates a simple single Gaussian non-dynamic retrieval model
    // for an individual feature trajectory.
    class SimpleFeatureComparator {
    private:
        // feature trajectory that this model represents.
        Feature* feature;
        bool initialized;
        
        // Gaussian Model
        double featureMean;
        double featureVariance;
        
        // Initialization.

        // compute the mean and variance of the feature trajectory
        void fitGaussian();
    
    public:
        SimpleFeatureComparator(Feature* feature_in);
        ~SimpleFeatureComparator();
        
        // General attributes.
        Feature* getFeature();
        bool isInitialized();
        
        // Gaussian attributes.
        double getMean();
        double getVariance();
        
        // Operations.
        void initialize();
        double compare(SimpleFeatureComparator* model);
    };
}

#endif
