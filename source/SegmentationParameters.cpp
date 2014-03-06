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

#include "SegmentationParameters.h"

namespace Sirens {
    SegmentationParameters::SegmentationParameters() {
        xInit[0] = 0;
        xInit[1] = 0;
        
        pInit[0][0] = 1;
        pInit[0][1] = 0;
        pInit[1][0] = 0;
        pInit[1][1] = 1;
        
        initialized = false;
    }
    
    SegmentationParameters::~SegmentationParameters() {
    }
    
    void SegmentationParameters::createFusionLogic() {
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                for (int k = 0; k < 3; k++) {
                    for (int l = 0; l < 3; l++)
                        fusion[i][j][k][l] = 0;
                }
            }
        }   
        
        fusion[0][0][0][0] = 1;
    
        fusion[0][0][1][0] = 1 - pLagMinus;
        fusion[0][0][1][2] = pLagMinus;

        fusion[0][0][2][0] = 1 - pLagMinus;
        fusion[0][0][2][2] = pLagMinus;

        fusion[0][1][0][0] = pLagPlus;
        fusion[0][1][0][1] = 1 - pLagPlus;
    
        fusion[0][1][1][0] = pLagPlus - pLagPlus * pLagMinus;
        fusion[0][1][1][1] = 1 - pLagPlus;
        fusion[0][1][1][2] = pLagPlus * pLagMinus;

        fusion[0][1][2][0] = pLagPlus - pLagPlus * pLagMinus;
        fusion[0][1][2][1] = 1 - pLagPlus;
        fusion[0][1][2][2] = pLagPlus * pLagMinus;

        fusion[1][2][0][0] = pLagPlus;
        fusion[1][2][0][1] = 1 - pLagPlus;

        fusion[1][2][1][2] = 1;

        fusion[1][2][2][0] = pLagPlus - pLagPlus * pLagMinus;
        fusion[1][2][2][1] = 1 - pLagPlus;
        fusion[1][2][2][2] = pLagPlus * pLagMinus;

        fusion[2][0][0][0] = 1;
    
        fusion[2][0][1][0] = 1 - pLagMinus;
        fusion[2][0][1][2] = pLagMinus;

        fusion[2][0][2][0] = 1 - pLagMinus;
        fusion[2][0][2][2] = pLagMinus;

        fusion[2][1][0][0] = pLagPlus;
        fusion[2][1][0][1] = 1 - pLagPlus;

        fusion[2][1][1][0] = pLagPlus - pLagPlus * pLagMinus;
        fusion[2][1][1][1] = 1 - pLagPlus;
        fusion[2][1][1][2] = pLagPlus * pLagMinus;

        fusion[2][1][2][0] = pLagPlus - pLagPlus * pLagMinus;
        fusion[2][1][2][1] = 1 - pLagPlus;
        fusion[2][1][2][2] = pLagPlus * pLagMinus;

        fusion[2][2][0][0] = pLagPlus;
        fusion[2][2][0][1] = 1 - pLagPlus;

        fusion[2][2][1][2] = 1;

        fusion[2][2][2][2] = 1;
    }
    
    /*
     * q(state[t - 1], state[t]) is the process variance for state transitions
     * in the Kalman Filter. Some values are irrelevant, because the prior mode
     * transition probabilities do not allow for them (see 
     * Segmenter::createModeLogic.)
     * 
     * In general, change (turnOn/TurnOff/newSegment) variances should be about
     * equal and should be much larger than those for no change
     * (stayOff/StayOn).
     */
    void SegmentationParameters::createQTable() {
        q[0][0] = cStayOff;    // off -> off
        q[0][1] = cTurnOn;     // off -> onset
        q[0][2] = 0;           // off -> on is impossible (irrelevant)
        q[1][0] = 0;           // onset -> off is impossible (irrelevant)
        q[1][1] = 0;           // onset -> onset is impossible (irrelevant)
        q[1][2] = 99999999999; // onset -> on always happens.
        q[2][0] = cTurnOff;    // on -> off
        q[2][1] = cNewSegment; // on -> onset (overlapping segments)
        q[2][2] = cStayOn;     // on -> on
    }
    
    void SegmentationParameters::initialize() {
        if (!initialized) {
            createFusionLogic();
            createQTable();
            
            initialized = true;
        }
    }
}
