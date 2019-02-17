/*******************************************************************************
 * MODULE  : solver.c
 * PURPOSE : main function for Kepler Equation Solver Library
 * AUTHOR  : Bazso Akos
 * VERSION : 1.0, 17 Feb 2019
 *
 * NOTE based on kesolver.c (v1)
 *
 * NOTE currently only implemented solver for elliptic case
 ******************************************************************************/
/* include standard headers */
#include <stdio.h>

/* include module headers */
#include "solver.h"
#include "const.h"
#include "utils.h"

/* include headers for solver modules */
#include "bisect.h"
#include "fixedp.h"
#include "newrap.h"

/******************************************************************************/

/* show additional debugging information ? 0 = no (default), 1 = yes */
#define KES_SOLVER_DEBUG 0

/******************************************************************************/

/* structure for indirect access to iteration functions */
typedef struct
{
    kes_sol_e         id;
    kes_iter_t* const func;
    /* TODO FIXME does not work with const kes_iter_t* const func; */
    const char*       text;
} kes_iter_list_t;

/* list of iterator functions and pointers to corresponding functions */
static const kes_iter_list_t list_of_iterators[KES_SOL_TOTAL] = {
    {KES_SOL_NONE,    NULL,    "invalid solver method"},
    {KES_SOL_BISECT,  &bisect, "Bisection method (interval halving)"},
    {KES_SOL_FIXEDP,  &fixedp, "Fixed-point iteration"},
    {KES_SOL_NEWRAP,  &newrap, "Newton-Raphson method"}
};

/* default error tolerance for solver functions */
static const double std_tol     = 1e-15;

/* default upper limit for number of iterations */
static const int    std_maxiter = 100;

/******************************************************************************/

/*******************************************************************************
 * FUNCTION    : kes_show_solver
 * DESCRIPTION : print information about chosen solver method
 * INPUT       : solver type from enum kes_sol_e
 * OUTPUT      : none
 ******************************************************************************/
inline void kes_show_solver(const kes_sol_e type)
{
    for (register kes_sol_e s = KES_SOL_NONE; s < KES_SOL_TOTAL; s++)
    {
        if ( list_of_iterators[s].id == type )
        {
            fprintf(
                stdout,
                "%s: solver method #%d = %s\n",
                __func__, (int)type, list_of_iterators[s].text
            );
            break;
        } // end if
    } // end for
} // end kes_show_solver

/******************************************************************************/

/*******************************************************************************
 * FUNCTION    : kes_check_input
 * DESCRIPTION : check input parameters and set default values if needed
 * INPUT       : structure of type kes_input_t as input
 * OUTPUT      : none
 ******************************************************************************/
static inline void kes_check_input(kes_input_t* data)
{
    /* override useless settings */
    if ( data->tolf < std_tol ) data->tolf    = std_tol;
    if ( data->tolx < std_tol ) data->tolx    = std_tol;
    if ( data->maxiter == 0 )   data->maxiter = std_maxiter;
    return;
} // end kes_check_input

/******************************************************************************/

/*******************************************************************************
 * FUNCTION    : kes_get_tolf
 * DESCRIPTION : return current value of parameter "tolf" in data structure
 * INPUT       : structure of type kes_input_t as input
 * OUTPUT      : value tolf
 ******************************************************************************/
inline double kes_get_tolf(const kes_input_t* const in)
{
    if ( in != NULL )
        return( in->tolf );
    else
        return 0.0;
} // end kes_get_tolf

/*******************************************************************************
 * FUNCTION    : kes_get_tolx
 * DESCRIPTION : return current value of parameter "tolx" in data structure
 * INPUT       : structure of type kes_input_t as input
 * OUTPUT      : value tolx
 ******************************************************************************/
inline double kes_get_tolx(const kes_input_t* const in)
{
    if ( in != NULL )
        return( in->tolx );
    else
        return 0.0;
} // end kes_get_tolx

/*******************************************************************************
 * FUNCTION    : kes_get_maxiter
 * DESCRIPTION : return current value of parameter "maxiter" in data structure
 * INPUT       : structure of type kes_input_t as input
 * OUTPUT      : value maxiter
 ******************************************************************************/
inline int kes_get_maxiter(const kes_input_t* const in)
{
    if ( in != NULL )
        return( in->maxiter );
    else
        return 0;
} // end kes_get_maxiter

/******************************************************************************/

/*******************************************************************************
 * FUNCTION    : kes_set_tolf
 * DESCRIPTION : set new value for parameter "tolf" in data structure
 * INPUT       : - structure of type kes_input_t
 *               - value "tolf" (ignored if negative)
 * OUTPUT      : error code from kes_err_e
 ******************************************************************************/
inline kes_err_e kes_set_tolf(
    kes_input_t* in,
    const double tolf
    )
{
    /* default value: error */
    kes_err_e err = KES_ERR_BADTOL;

    /* check input error tolerance to be a valid number */
    if (
        (kes_check_val(tolf) == KES_ERR_NOERR) &&
        (tolf > std_tol) &&
        (tolf < 1.0)
    ) {
        in->tolf = tolf;
        err = KES_ERR_NOERR;
    } // end if

    return( err );
} // end kes_set_tolf

/******************************************************************************/

/*******************************************************************************
 * FUNCTION    : kes_set_tolx
 * DESCRIPTION : set new value for parameter "tolx" in data structure
 * INPUT       : - structure of type kes_input_t
 *               - value "tolx" (ignored if negative)
 * OUTPUT      : error code from kes_err_e
 ******************************************************************************/
inline kes_err_e kes_set_tolx(
    kes_input_t* in,
    const double tolx
    )
{
    /* default value: error */
    kes_err_e err = KES_ERR_BADTOL;

    /* check input error tolerance to be a valid number */
    if (
        (kes_check_val(tolx) == KES_ERR_NOERR) &&
        (tolx > std_tol) &&
        (tolx < 1.0)
    ) {
        in->tolx = tolx;
        err = KES_ERR_NOERR;
    } // end if

    return( err );
} // end kes_set_tolx

/******************************************************************************/

/*******************************************************************************
 * FUNCTION    : kes_set_maxiter
 * DESCRIPTION : set new value for parameter "maxiter" in data structure
 * INPUT       : - structure of type kes_input_t
 *               - value "maxiter"
 * OUTPUT      : error code from kes_err_e
 ******************************************************************************/
inline kes_err_e kes_set_maxiter(
    kes_input_t* in,
    const int    maxiter
    )
{
    /* default value: error */
    kes_err_e err = KES_ERR_BADVAL;

    /* check input value to be a suitable number
     * TODO FIXME limit to how many times std_maxiter ???
     */
    if ( (maxiter > 0) && (maxiter < 10 * std_maxiter) )
    {
        in->maxiter = maxiter;
        err = KES_ERR_NOERR;
    } // end if

    return( err );
} // end kes_set_maxiter

/******************************************************************************/

/*******************************************************************************
 *  FUNCTION    : kesolver
 *  DESCRIPTION : main solver function for Kepler Equation
 *  INPUT       : - eccentricity "ecc"
 *                - mean anomaly "ma" (in radians)
 *                - starter method "init" from enum kes_stm_e
 *                - solver method "iter" from enum kes_sol_e
 *                - pointer "data" to data structure of type "kes_input_t"
 *                - pointer "status" to return error code (see error.h)
 *  OUTPUT      : solution of Kepler Equation (in radians)
 ******************************************************************************/
double kesolver(
    const double    ecc,
    const double    ma,
    const kes_stm_e init,
    const kes_sol_e iter,
    kes_input_t*    data,
    kes_err_e*      status
    )
{
    /* exception: input contains invalid numbers */
    if (
        (kes_check_val( ecc ) != KES_ERR_NOERR) ||
        (kes_check_val( ma )  != KES_ERR_NOERR)
    ) {
        /* error handling */
        *status = KES_ERR_BADVAL;
#if KES_SOLVER_DEBUG
    kes_show_error( *status );
#endif
        return( 0.0 );
    } // end if

    /* check eccentricity domain */
    const kes_ecc_e type = kes_check_ecc(ecc, status);

    /* exception: invalid eccentricity */
    if ( type == KES_ECC_NEG )
    {
        /* error handling */
        *status = KES_ERR_BADECC;
#if KES_SOLVER_DEBUG
    kes_show_error( *status );
#endif
        return( 0.0 );
    } // end if

    /* circular case: return solution immediately */
    if ( type == KES_ECC_CIRC )
    {
        *status = KES_ERR_NOERR;
        return( ma );
    } // end if

    /* TODO FIXME hyperbolic & parabolic cases not yet implemented */
    if ( type != KES_ECC_ELL )
    {
        /* error handling */
        *status = KES_ERR_BADECC;
#if KES_SOLVER_DEBUG
    kes_show_error( *status );
#endif
        return( 0.0 );
    } // end if

    /* reduce mean anomaly to range -pi <= redma < pi */
    double redma = kes_reduce( ma );

    /* side = +1 if redma >= 0, -1 if redma < 0 */
    int    side  = 1;

    /* check for negative value after reduction */
    if ( redma < 0.0 )
    {
        side  = -1;
        redma = -redma;
    } // end if

    /* TODO FIXME check for special values of mean anomaly,
     * e.g. {0, pi/2, pi}
     * and return solution immediately
     */

    /* check input parameters */
    kes_check_input( data );

    /* call starter function */
    data->starter = kes_starter(ecc, redma, init, status);

    /* exception: check starter function error code */
    if ( *status != KES_ERR_NOERR )
    {
        /* error handling */
        *status = KES_ERR_BADSTM;
        /* fallback in case of wrong starter method */
        data->starter = redma + ecc;
#if KES_SOLVER_DEBUG
    kes_show_error( *status );
#endif
    } // end if

    /* TODO FIXME check convergence for starter value */

    /* exception: check solver method type */
    if (
        (iter <= KES_SOL_NONE) ||
        (iter >= KES_SOL_TOTAL)
    ) {
        /* error handling */
        *status = KES_ERR_BADSOL;
#if KES_SOLVER_DEBUG
    kes_show_error( *status );
#endif
        return( 0.0 );
    } // end if

    /* call solver/iterator method */
    data->iterations = list_of_iterators[iter].func(
        ecc, redma, data->starter, data
    );

    /* modify solution if necessary */
    if ( side < 0 )
    {
        data->result = M_2PI - data->result;
    } // end if

    /* kesolver() has been successful */
    *status = KES_ERR_NOERR;

    /* finally return result */
    return( data->result );
} // end kesolver

/******************************************************************************/
