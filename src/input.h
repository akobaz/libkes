/***************************************************************************//**
 * @file    input.h
 * @brief   data structure and utility functions for input data
 * @author  Bazso Akos
 * @version 2019.09
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
#ifndef KES_INPUT__H
#define KES_INPUT__H

/* include module headers */
#include "error.h"

/******************************************************************************/

/*** declare macro constants ***/

/* default tolerance for absolute/relative error */
#define KES_STD_TOL     1E-15

/* default maximum number of iterations */
#define KES_STD_MAXITER 100

/******************************************************************************/

/*** declare data structures ***/

/*!
 * @brief structure containing iteration data
 * @details use this structure as interface to function kesolver()
 */
typedef struct
{
    /* user can get/set these parameters */
    double tolf;       ///< iteration tolerance for f(x(n))
    double tolx;       ///< iteration tolerance for x(n)
    int    maxiter;    ///< maximum number of iterations

    /* output from iteration function */
    double result;     ///< returns solution of Kepler Equation
    double starter;    ///< starting value for first iteration
                       // NOTE solver functions may override starting value
    double errDF;      ///< residual error for |f(x(n+1))|
    double errDX;      ///< residual error for |x(n+1)-x(n)|
    int    iterations; ///< number of iterations performed until convergence

    /* additional counters for trigonometric function evaluations
     * NOTE only for internal use
     */
    int    nbrSinEval; ///< number of evaluations of sin()
    int    nbrCosEval; ///< number of evaluations of cos()
    int    nbrFktEval; ///< number of evaluations of kes_keq_xxx()

    /* TODO FIXME add function pointer to kes_keq_xxx ? */
} kes_input_t;

/******************************************************************************/

/*** function declarations ***/

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * @brief get current value of parameter "tolf"
 * @param[in] in pointer to structure of type #kes_input_t
 * @return value of tolf
 */
double kes_get_tolf(const kes_input_t* const in);


/*!
 * @brief get current value of parameter "tolx"
 * @param[in] in pointer to structure of type #kes_input_t
 * @return value of tolx
 */
double kes_get_tolx(const kes_input_t* const in);


/*!
 * @brief get current value of parameter "maxiter"
 * @param[in] in pointer to structure of type #kes_input_t
 * @return value of maxiter
 */
int kes_get_maxiter(const kes_input_t* const in);


/*!
 * @brief set new value for parameter "tolf"
 * @details default value for tolf is 1E-15
 * @param[in] in pointer to structure of type #kes_input_t
 * @param[in] tol new error tolerance for |f(x(n+1))|
 * @return error code from enum #kes_err_e
 */
kes_err_e kes_set_tolf(
    kes_input_t* in,
    const double tol
);


/*!
 * @brief set new value for parameter "tolx"
 * @details default value for tolx is 1E-15
 * @param[in] in pointer to structure of type #kes_input_t
 * @param[in] tol new error tolerance for |x(n+1) - x(n)|
 * @return error code from enum #kes_err_e
 */
kes_err_e kes_set_tolx(
    kes_input_t* in,
    const double tol
);


/*!
 * @brief set new value for parameter "maxiter"
 * @details default value for maxiter is 100
 * @param[in] in pointer to structure of type #kes_input_t
 * @param[in] maxiter new max. number of iterations
 * @return error code from enum #kes_err_e
 */
kes_err_e kes_set_maxiter(
    kes_input_t* in,
    const int    maxiter
);


/*!
 * @brief check input parameters and set default values if needed
 * @param[in,out] data pointer to structure of type #kes_input_t
 * @return 0 in case of success (no parameter replacements needed),
 *         >0 in case of warning (if any parameter replaced by default value)
 */
int kes_check_input(kes_input_t* data);


/*!
 * @brief dummy function that returns input data structure
 */
kes_input_t kes_new_input_dummy(kes_input_t in);


/*!
 * @brief initialize and return a new instance of the data structure
 * @details create a new instance of data structure from variable number
 * of input arguments, omitted arguments are replaced by default values;
 * function prototype: kes_input_t kes_new_input(args);
 * @return structure of type #kes_input_t
 */
#define kes_new_input(...) kes_new_input_dummy(                                \
                             (kes_input_t){                                    \
                               .tolf    = KES_STD_TOL,                         \
                               .tolx    = KES_STD_TOL,                         \
                               .maxiter = KES_STD_MAXITER,                     \
                               __VA_ARGS__ } )

#ifdef __cplusplus
}
#endif

/******************************************************************************/

#endif  /* KES_INPUT__H */
