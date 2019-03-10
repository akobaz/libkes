/*******************************************************************************
 * @file    kepeq.c
 * @brief   evaluate Kepler Equations for different cases of eccentricity
 * @author  Bazso Akos
 * @version 1.0, 16 Feb 2019
 *          NOTE derived from corefunc.c (V1.2)
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
/* include standard headers */
#include <math.h>

/* include module headers */
#include "kepeq.h"

/******************************************************************************/

/* use verbose debug output? 0 = no (default), 1 = yes */
#define KES_KEPEQ_DEBUG 0

/******************************************************************************/

/*******************************************************************************
 *  FUNCTION    : kes_keq_ell
 *  DESCRIPTION : evaluate elliptic Kepler Equation
 *  INPUT       : - eccentricity ecc (0 <= ecc < 1)
 *                - mean anomaly ma (in radians)
 *                - elliptic eccentric anomaly x (in radians)
 *  OUTPUT      : result of x - ecc*sin(x) - ma
 ******************************************************************************/
inline double kes_keq_ell(
    const double ecc,
    const double ma,
    const double x
    )
{
    return( x - ecc * sin(x) - ma );
} // end kes_keq_ell

/******************************************************************************/

/*******************************************************************************
 *  FUNCTION    : kes_keq_hyp
 *  DESCRIPTION : evaluate hyperbolic Kepler Equation
 *  INPUT       : - eccentricity ecc (ecc > 1)
 *                - mean anomaly ma (in radians)
 *                - hyperbolic eccentric anomaly x (in radians)
 *  OUTPUT      : result of ecc*sinh(x) - x - ma
 ******************************************************************************/
inline double kes_keq_hyp(
    const double ecc,
    const double ma,
    const double x
    )
{
    return( ecc * sinh(x) - x - ma );
} // end kes_keq_hyp

/******************************************************************************/

/*******************************************************************************
 *  FUNCTION    : kes_keq_par
 *  DESCRIPTION : evaluate parabolic Kepler Equation (Barker's Equation)
 *  INPUT       : - eccentricity ecc (ecc = 1), NOTE unused
 *                - mean anomaly ma (in radians)
 *                - true anomaly x (in radians)
 *  OUTPUT      : result of s + s^3 / 3 - ma, with s = tan(x/2)
 ******************************************************************************/
inline double kes_keq_par(
    const double ecc,
    const double ma,
    const double x
    )
{
    /* TODO FIXME check case x = pi */
    const double s = tan( 0.5 * x );
    return( s + s*s*s / 3.0 - ma );
} // end kes_keq_par

/******************************************************************************/
