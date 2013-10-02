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

#ifndef __MATRIX_SUPPORT_H__
#define __MATRIX_SUPPORT_H__

#include "../boost/vector.hpp"
#include "../boost/vector_proxy.hpp"
#include "../boost/matrix.hpp"
#include "../boost/matrix_proxy.hpp"
#include "../boost/triangular.hpp"
#include "../boost/operation.hpp"
#include "../boost/lu.hpp"
using namespace boost::numeric;

namespace Sirens {
    ublas::matrix<double> invert(ublas::matrix<double> input);
    double determinant(ublas::matrix<double> input);
    ublas::matrix<double> normalize_affinity(ublas::matrix<double> input);
}

#endif
