/***************************************************************************//**
 * @file    libkes.h
 * @brief   main header and API definition for Kepler Equation Solver Library
 * @author  Bazso Akos
 * @version 2019.02
 * @copyright Bazso Akos, 2011-2019, see LICENSE
 ******************************************************************************/
#pragma once
#ifndef LIBKES__H
#define LIBKES__H

/******************************************************************************/

/*** declare data structures ***/

/*!
 * @brief enumeration of error codes
 * @details list of error codes returned by some functions;
 * the user should check the function's return value,
 * and display the error message with kes_show_error() if necessary
 */
typedef enum
{
    KES_ERR_NOERR = 0, ///< no error, successful return

    /* different error codes */
    KES_ERR_BADECC,    ///< bad value for eccentricity in kes_check_ecc()
    KES_ERR_BADVAL,    ///< bad value for parameter (INF or NAN) in kes_check_val()
    KES_ERR_BADSTM,    ///< bad starter method in kes_starter()
    KES_ERR_BADSOL,    ///< bad solver method in kesolver()
    KES_ERR_BADTOL,    ///< bad value for error tolerance

    KES_ERR_TOTAL      ///< total number of error entries
} kes_err_e;


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


/*!
 * @brief enumeration of starter methods
 * @details reference list of starter methods:
 * Odell & Gooding (1986), Celestial Mechanics, Volume 38, Issue 4, p.307-334
 * [ADS link](http://adsabs.harvard.edu/abs/1986CeMec..38..307O)
 */
typedef enum
{
    KES_STM_NONE = 0, ///< unidentified (void) starting value method

    /* different starter methods for elliptic case */
    KES_STM_ES00,     ///< starting value "pi"
    KES_STM_ES01,     ///< starter S1 from Odell & Gooding (1986)
    KES_STM_ES02,     ///< starter S2 from Odell & Gooding (1986)
    KES_STM_ES03,     ///< starter S3 from Odell & Gooding (1986)
    KES_STM_ES04,     ///< starter S4 from Odell & Gooding (1986)
    KES_STM_ES05,     ///< starter S5 from Odell & Gooding (1986)
    KES_STM_ES06,     ///< starter S6 from Odell & Gooding (1986)
    KES_STM_ES07,     ///< starter S7 from Odell & Gooding (1986)
    KES_STM_ES08,     ///< starter S8 from Odell & Gooding (1986)
    KES_STM_ES09,     ///< starter S9 from Odell & Gooding (1986)
    KES_STM_ES10,     ///< starter S10 from Odell & Gooding (1986)
    KES_STM_ES11,     ///< starter S11 from Odell & Gooding (1986)
    KES_STM_ES12,     ///< starter S12 from Odell & Gooding (1986)
    KES_STM_ES13,     ///< starter from Encke (1850)
    KES_STM_ES14,     ///< starter from Charles & Tatum (1998)

    KES_STM_TOTAL     ///< total number of available starting value methods
} kes_stm_e;


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
    double errDF;      ///< residual error for |f(x(n+1))|
    double errDX;      ///< residual error for |x(n+1)-x(n)|
    int    iterations; ///< number of iterations performed until convergence

    /* additional counters for trigonometric function evaluations
     * NOTE only internal use
     */
    int    nbrSinEval; ///< number of evaluations of sin()
    int    nbrCosEval; ///< number of evaluations of cos()
    int    nbrFktEval; ///< number of evaluations of kes_keq_xxx()
} kes_input_t;

/******************************************************************************/

/*** function declarations ***/

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * @brief print error message to \a stderr
 * @param[in] type error code from enum #kes_err_e
 * @return none
 */
void kes_show_error(const kes_err_e type);


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
 * @brief calculate the true anomaly for the elliptic or hyperbolic case
 * @param[in] ecc eccentricity
 * @param[in] x elliptic or hyperbolic eccentric anomaly (in radians)
 * @return true anomaly (in radians)
 */
double kes_trueanom(
    const double ecc,
    const double x
);


/*!
 * @brief print the Library's version number
 * @return none
 */
void kes_version(void);


/*!
 * @brief starting value for iteration methods
 * @param[in] ecc eccentricity
 * @param[in] ma mean anomaly (in radians)
 * @param[in] type choose a starter method from enum #kes_stm_e
 * @param[out] status pointer to return error code from enum #kes_err_e
 * @return starting value x0
 */
double kes_starter(
    const double    ecc,
    const double    ma,
    const kes_stm_e type,
    kes_err_e*      status
);


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

#endif  /* LIBKES__H */
