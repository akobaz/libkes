/***************************************************************************//**
 * @file    itercore.h
 * @brief   iteration functions for Kepler Equation Solver Library
 * @author  Bazso Akos
 * @version 2019.02
 *
 * @copyright
 * Copyright (C) 2012-2019 Bazso Akos
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
#ifndef KES_ITERCORE__H
#define KES_ITERCORE__H

/******************************************************************************/

/*** function declarations ***/

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * @brief Newton-Raphson iteration method
 * @details quadratic convergence
 * @param[in] ecc eccentricity
 * @param[in] ma mean anomaly (in radians)
 * @param[in] x0 iteration variable
 * @return refined iteration variable x0 + dx
 */
double kes_itercore2(
    const double ecc,
    const double ma,
    const double x0
);


/*!
 * @brief Halley iteration method
 * @details cubic convergence
 * @param[in] ecc eccentricity
 * @param[in] ma mean anomaly (in radians)
 * @param[in] x0 iteration variable
 * @return refined iteration variable x0 + dx
 */
double kes_itercore3(
    const double ecc,
    const double ma,
    const double x0
);


/*!
 * @brief Danby-Burkardt 4th order iteration method
 * @details quartic convergence
 * @param[in] ecc eccentricity
 * @param[in] ma mean anomaly (in radians)
 * @param[in] x0 iteration variable
 * @return refined iteration variable x0 + dx
 */
double kes_itercore4(
    const double ecc,
    const double ma,
    const double x0
);


/*!
 * @brief Danby-Burkardt 5th order iteration method
 * @details quintic convergence
 * @param[in] ecc eccentricity
 * @param[in] ma mean anomaly (in radians)
 * @param[in] x0 iteration variable
 * @return refined iteration variable x0 + dx
 */
double kes_itercore5(
    const double ecc,
    const double ma,
    const double x0
);

#ifdef __cplusplus
}
#endif

/******************************************************************************/

#endif  /* KES_ITERCORE__H */
