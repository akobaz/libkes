/***************************************************************************//**
 * @file    danbur4.h
 * @brief   Danby-Burkardt method of order 4
 * @details quartic convergence; after description in:
 * Danby & Burkardt (1983), Celestial Mechanics 31, p.95-107
 * @author  Bazso Akos
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
#ifndef KES_SOL_DANBUR4__H
#define KES_SOL_DANBUR4__H

/* include module headers */
#include "input.h"

/******************************************************************************/

/*** function declarations ***/

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * @brief iteration function for Danby-Burkardt method of order 4
 * @param[in] ecc eccentricity
 * @param[in] ma mean anomaly (in radians)
 * @param[in] starter starting value for first iteration
 * @param[out] res iteration results and statistics
 * @return number of iterations
 */
int danbur4(
    const double ecc,
    const double ma,
    const double starter,
    kes_input_t* res
);

#ifdef __cplusplus
}
#endif

/******************************************************************************/

#endif  /* KES_SOL_DANBUR4__H */
