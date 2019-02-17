/***************************************************************************//**
 * @file    solver.h
 * @brief   main function for Kepler Equation Solver Library
 * @author  Bazso Akos
 * @version 2019.02
 ******************************************************************************/
#pragma once
#ifndef KES_SOLVER__H
#define KES_SOLVER__H

/* include module headers */
#include "error.h"
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
    KES_SOL_BISECT,   ///< bisection method
    KES_SOL_FIXEDP,   ///< fixed-point-iteration
    KES_SOL_NEWRAP,   ///< Newton-Raphson

    KES_SOL_TOTAL     ///< total number of available solver methods
} kes_sol_e;


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
    double result;     ///< for returning solution of Kepler Equation
    double starter;    ///< starting value for first iteration
    // NOTE some solver methods may override this starter value
    double errDF;      ///< residual error for |f(x(n+1))|
    double errDX;      ///< residual error for |x(n+1)-x(n)|
    int    iterations; ///< number of iterations performed until convergence

    /* additional counters for trigonometric function evaluations
     * NOTE only internal use
     */
    int    nbrSinEval; ///< number of evaluations of sin()
    int    nbrCosEval; ///< number of evaluations of cos()
    int    nbrFktEval; ///< number of evaluations of kes_keq_xxx()

    /* TODO FIXME add function pointer to kes_keq_xxx ? */
} kes_input_t;


/*!
 * @brief prototype declaration for iteration functions
 * @details functions return number of iterations performed until convergence,
 * the resulting solution is returned in \a stats->result
 */
typedef int kes_iter_t(
    const double ecc,
    const double ma,
    const double starter,
    kes_input_t* stats
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
 * @brief main solver function for Kepler Equation
 * @details user can specify additional parameters via structure \a data
 * @param[in] ecc eccentricity
 * @param[in] ma mean anomaly (in radians)
 * @param[in] init choose a starter method from enum #kes_stm_e
 * @param[in] iter choose a solver method from enum #kes_sol_e
 * @param[in,out] data pointer to structure of iteration data
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
