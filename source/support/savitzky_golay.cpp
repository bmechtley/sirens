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

#include "savitzky_golay.h"

namespace Sirens {
    void fill_savitzky_golay(
        ublas::vector<double>& first,
        ublas::vector<double>& second
    ) {
        first.resize(41);
        second.resize(41);

        first[0] = 0.0323312535450935;
        first[1] = 0.00850822461712981;
        first[2] = -0.00829006501156256;
        first[3] = -0.0189798856843667;
        first[4] = -0.0244279796179967;
        first[5] = -0.0254515609024968;
        first[6] = -0.0228183155012423;
        first[7] = -0.0172464012509389;
        first[8] = -0.00940444786162312;
        first[9] = 8.84430833381321e-05;
        first[10] = 0.0106626981272472;
        first[11] = 0.0217982719400757;
        first[12] = 0.0330246473184647;
        first[13] = 0.0439208351857245;
        first[14] = 0.0541153745918351;
        first[15] = 0.0632863327134455;
        first[16] = 0.0711613048538743;
        first[17] = 0.0775174144431092;
        first[18] = 0.0821813130378076;
        first[19] = 0.0850291803212959;
        first[20] = 0.0859867241035703;
        first[21] = 0.0850291803212959;
        first[22] = 0.0821813130378076;
        first[23] = 0.0775174144431092;
        first[24] = 0.0711613048538743;
        first[25] = 0.0632863327134455;
        first[26] = 0.0541153745918351;
        first[27] = 0.0439208351857245;
        first[28] = 0.0330246473184647;
        first[29] = 0.0217982719400757;
        first[30] = 0.0106626981272472;
        first[31] = 8.84430833381564e-05;
        first[32] = -0.00940444786162310;
        first[33] = -0.0172464012509389;
        first[34] = -0.0228183155012423;
        first[35] = -0.0254515609024968;
        first[36] = -0.0244279796179966;
        first[37] = -0.0189798856843667;
        first[38] = -0.00829006501156248;
        first[39] = 0.00850822461712988;
        first[40] = 0.0323312535450935;

        second[0] = 0.00733573478893954;
        second[1] = 0.00426393421288486;
        second[2] = 0.00160828961169289;
        second[3] = -0.000653101960681753;
        second[4] = -0.00254214345028448;
        second[5] = -0.00408073780316071;
        second[6] = -0.00529078796535584;
        second[7] = -0.00619419688291528;
        second[8] = -0.00681286750188443;
        second[9] = -0.00716870276830871;
        second[10] = -0.00728360562823351;
        second[11] = -0.00717947902770424;
        second[12] = -0.00687822591276632;
        second[13] = -0.00640174922946515;
        second[14] = -0.00577195192384613;
        second[15] = -0.00501073694195466;
        second[16] = -0.00414000722983617;
        second[17] = -0.00318166573353605;
        second[18] = -0.00215761539909971;
        second[19] = -0.00108975917257256;
        second[20] = -4.35165292826184e-19;
        second[21] = 0.00108975917257256;
        second[22] = 0.00215761539909971;
        second[23] = 0.00318166573353605;
        second[24] = 0.00414000722983617;
        second[25] = 0.00501073694195467;
        second[26] = 0.00577195192384613;
        second[27] = 0.00640174922946515;
        second[28] = 0.00687822591276632;
        second[29] = 0.00717947902770425;
        second[30] = 0.00728360562823351;
        second[31] = 0.00716870276830871;
        second[32] = 0.00681286750188444;
        second[33] = 0.00619419688291529;
        second[34] = 0.00529078796535585;
        second[35] = 0.00408073780316072;
        second[36] = 0.00254214345028449;
        second[37] = 0.000653101960681753;
        second[38] = -0.00160828961169289;
        second[39] = -0.00426393421288486;
        second[40] = -0.00733573478893955;
    }
}
