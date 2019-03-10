/***************************************************************************//**
 * @file    kepeq.h
 * @brief   evaluate Kepler Equations for different cases of eccentricity
 * @author  Bazso Akos
 * @version 2019.02
 *
 * @copyright
 * Copyright (C) 2019 Bazso Akos
 *
 * This file is part of libkes.
 *
 * libkes is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * libkes is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with libkes.  If not, see <https://www.gnu.org/licenses/>.
 *
 ******************************************************************************/
#pragma once
#ifndef KES_KEPEQ__H
#define KES_KEPEQ__H

/******************************************************************************/

/*** function declarations ***/

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * @brief evaluate elliptic Kepler Equation
 * @param[in] ecc eccentricity
 * @param[in] ma mean anomaly (in radians)
 * @param[in] x elliptic eccentric anomaly (in radians)
 * @return result of expression \f$ x - e \sin x - M \f$
 */
double kes_keq_ell(
    const double ecc,
    const double ma,
    const double x
);


/*!
 * @brief evaluate hyperbolic Kepler Equation
 * @param[in] ecc eccentricity
 * @param[in] ma mean anomaly (in radians)
 * @param[in] x hyperbolic eccentric anomaly (in radians)
 * @return result of expression \f$ e \sinh x - x - M \f$
 */
double kes_keq_hyp(
    const double ecc,
    const double ma,
    const double x
);


/*!
 * @brief evaluate parabolic Kepler Equation (Barker's Equation)
 * @param[in] ecc eccentricity (NOTE unused)
 * @param[in] ma mean anomaly (in radians)
 * @param[in] x true anomaly (in radians)
 * @return result of expression \f$ s + (1/3) s^3 - M \f$ with s = tan(x/2)
 */
double kes_keq_par(
    const double ecc,
    const double ma,
    const double x
);

#ifdef __cplusplus
}
#endif

/******************************************************************************/

# endif  /* KES_KEPEQ__H */
