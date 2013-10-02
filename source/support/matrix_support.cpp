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

#include "matrix_support.h"

namespace Sirens {
    ublas::matrix<double> invert(ublas::matrix<double> input) {
        typedef ublas::permutation_matrix<std::size_t> pmatrix;

        ublas::matrix<double> tmp(input);
        ublas::permutation_matrix<std::size_t> pm(tmp.size1());

        int res = ublas::lu_factorize(tmp,pm);

        if (res == 0) {
            ublas::matrix<double> inverse = ublas::identity_matrix<double>(
                tmp.size1()
            );

            ublas::lu_substitute(tmp, pm, inverse);

            return inverse;
        } else {
            // Easy check for singularity, as nothing inverted = 0.
            // In the future, exceptions would be more prudent.
            return ublas::zero_matrix<double>(1, 1);
        }
    }

    double determinant(ublas::matrix<double> input) {
        ublas::permutation_matrix<std::size_t> pivots(input.size1());

        ublas::lu_factorize(input, pivots);

        double det = 1.0;

        for (std::size_t i = 0; i < pivots.size(); i++) {
            if (pivots(i) != i)
                det *= -1.0;

            det *= input(i, i);
        }

        return det;
    }

    ublas::matrix<double> normalize_affinity(ublas::matrix<double> input) {
        ublas::vector<double> ones = ublas::scalar_vector<double>(
            input.size1(),
            1
        );

        ublas::vector<double> diag(input.size1());

        for (int i = 0; i < input.size1(); i++)
            diag[i] = input(i, i);

        return ublas::outer_prod(diag, ones) +
            ublas::outer_prod(
                ublas::trans(ones),
                ublas::trans(diag)
            ) - input - ublas::trans(input);
    }
}
