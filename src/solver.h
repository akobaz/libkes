/***************************************************************************//**
 * @file    solver.h
 * @brief   main function for Kepler Equation Solver Library
 * @author  Bazso Akos
 * @version 2019.11
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
#ifndef KES_SOLVER__H
#define KES_SOLVER__H

/* include module headers */
#include "error.h"
#include "input.h"
#include "starter.h"

/******************************************************************************/

/*** declare data structures ***/

/*!
 * @brief enumeration of solver types
 * @details list of all available iteration methods
 */
typedef enum
{
    KES_SOL_NONE = 0, ///< unidentified (void) solver method

    /* different solver methods */
    KES_SOL_BISECT,   ///< Bisection method
    KES_SOL_DANBUR4,  ///< Danby-Burkardt method (order 4)
    KES_SOL_DANBUR5,  ///< Danby-Burkardt method (order 5)
    KES_SOL_FIXEDP,   ///< Fixed-point method
    KES_SOL_HALLEY,   ///< Halley method
    KES_SOL_LAGCON,   ///< Laguerre-Conway method
    KES_SOL_MARKLEY,  ///< Markley method
    KES_SOL_MIKKOLA,  ///< Mikkola method
    KES_SOL_NEWRAP,   ///< Newton-Raphson method
    KES_SOL_NIJENH,   ///< Nijenhuis method
    KES_SOL_SECANT,   ///< Secant method
    KES_SOL_WEGSEC,   ///< Wegstein's secant modification

    KES_SOL_TOTAL     ///< total number of available solver methods
} kes_sol_e;


/*!
 * @brief prototype declaration for iteration functions
 * @details functions return number of iterations performed until convergence,
 * the resulting solution is returned in \a data->result
 */
typedef int kes_iter_t(
    const double ecc,
    const double ma,
    const double starter,
    kes_input_t* data
);

/******************************************************************************/

/*** function declarations ***/

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * @brief print information about a solver method
 * @param[in] type a method from enum #kes_sol_e
 * @return none
 */
void kes_show_solver(const kes_sol_e type);


/*!
 * @brief main solver function for Kepler Equation
 * @details user can specify additional parameters via structure \a data
 * @param[in] ecc eccentricity
 * @param[in] ma mean anomaly (in radians)
 * @param[in] init choose a starter method from enum #kes_stm_e
 * @param[in] iter choose a solver method from enum #kes_sol_e
 * @param[in,out] data pointer to data structure of type #kes_input_t
 * @param[out] status pointer to return error code from enum #kes_err_e
 * @return solution to Kepler Equation
 */
double kesolver(
    const double    ecc,
    const double    ma,
    const kes_stm_e init,
    const kes_sol_e iter,
    kes_input_t*    data,
    kes_err_e*      status
);

#ifdef __cplusplus
}
#endif

/******************************************************************************/

#endif  /* KES_SOLVER__H */
