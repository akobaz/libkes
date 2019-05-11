/***************************************************************************//**
 * @file    halley.h
 * @brief   Halley method
 * @details cubic convergence; after description in:
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
#ifndef KES_SOL_HALLEY__H
#define KES_SOL_HALLEY__H

/* include module headers */
#include "solver.h"

/******************************************************************************/

/*** function declarations ***/

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * @brief iteration function for Halley method of order 3
 * @param[in] ecc eccentricity
 * @param[in] ma mean anomaly (in radians)
 * @param[in] starter starting value for first iteration (ignored)
 * @param[out] res iteration results and statistics
 * @return number of iterations
 */
int halley(
    const double ecc,
    const double ma,
    const double starter,
    kes_input_t* res
);

#ifdef __cplusplus
}
#endif

/******************************************************************************/

#endif  /* KES_SOL_HALLEY__H */
