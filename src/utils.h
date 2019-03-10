/***************************************************************************//**
 * @file    utils.h
 * @brief   utility functions for Kepler Equation Solver Library
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
#ifndef KES_UTILS__H
#define KES_UTILS__H

/* include module headers */
#include "error.h"

/******************************************************************************/

/*** declare data structures ***/

/*!
 * @brief cases for eccentricity domain
 * @details discriminate between different eccentricity domains
 * based on value of the \a eccentricity (must be positive or zero)
 */
typedef enum
{
    KES_ECC_NEG = 0, ///< invalid case    : e < 0
    KES_ECC_CIRC,    ///< circular case   : e = 0
    KES_ECC_ELL,     ///< elliptic case   : 0 < e < 1
    KES_ECC_PAR,     ///< parabolic case  : e = 1
    KES_ECC_HYP      ///< hyperbolic case : e > 1
} kes_ecc_e;

/******************************************************************************/

/*** function declarations ***/

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * @brief check input to be a valid eccentricity value
 * @param[in] ecc eccentricity (real number >= 0)
 * @param[out] status error code from enum #kes_err_e (see error.h)
 * @return code from enum #kes_ecc_e (> 0 for success)
 */
kes_ecc_e kes_check_ecc(
    const double ecc,
    kes_err_e*   status
);


/*!
 * @brief check input to be a valid floating point number
 * @param[in] x real number
 * @return error code from enum #kes_err_e (= 0 for success)
 */
kes_err_e kes_check_val(const double x);


/*!
 * @brief reduce input angle to range [-pi:pi]
 * @param x angle (in radians)
 * @return reduced angle
 */
double kes_reduce(double x);


/*!
 * @brief calculate sin(x) and cos(x) simultaneously
 * @details if parameter \a ecc < 0 then returns {sin(x), cos(x)};
 * if parameter \a ecc >= 0 then returns {ecc*sin(x), ecc*cos(x)}
 * @param[out] sx pointer for sin(x)
 * @param[out] cx pointer for cos(x)
 * @param[in] x input value
 * @param[in] ecc eccentricity
 * @return sin(x), cos(x)
 */
void kes_sincos(
    double*      sx,
    double*      cx,
    const double x,
    const double ecc
);


/*!
 * @brief calculate the true anomaly for the elliptic or hyperbolic case
 * @param[in] ecc eccentricity
 * @param[in] x elliptic or hyperbolic eccentric anomaly (in radians)
 * @return true anomaly (in radians)
 */
double kes_trueanom(
    const double ecc,
    const double x
);

#ifdef __cplusplus
}
#endif

/******************************************************************************/

#endif  /* KES_UTILS__H */
