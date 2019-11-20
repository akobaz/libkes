/*******************************************************************************
 * @file    solver.c
 * @brief   main function for Kepler Equation Solver Library
 * @author  Bazso Akos
 * @version 1.0, 17 Feb 2019
 *          1.1, 01 Mar 2019
 *          1.2, 11 May 2019
 *          1.3, 20 Sep 2019
 *          1.4, 18 Nov 2019
 *          NOTE based on kesolver.c (V1.0)
 *
 * NOTE currently only implemented solver for elliptic case
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
#include <stdio.h>

/* include module headers */
#include "solver.h"
#include "const.h"
#include "utils.h"

/* include headers for solver modules */
#include "bisect.h"
#include "danbur4.h"
#include "danbur5.h"
#include "fixedp.h"
#include "halley.h"
#include "lagcon.h"
#include "markley.h"
#include "mikkola.h"
#include "newrap.h"
#include "nijenh.h"
#include "secant.h"
#include "wegsec.h"

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
    {KES_SOL_NONE,    NULL,     "invalid solver method"},
    {KES_SOL_BISECT,  &bisect,  "Bisection method (interval halving)"},
    {KES_SOL_DANBUR4, &danbur4, "Danby-Burkardt method of order 4"},
    {KES_SOL_DANBUR5, &danbur5, "Danby-Burkardt method of order 5"},
    {KES_SOL_FIXEDP,  &fixedp,  "Fixed-point iteration"},
    {KES_SOL_HALLEY,  &halley,  "Halley method"},
    {KES_SOL_LAGCON,  &lagcon,  "Laguerre-Conway method"},
    {KES_SOL_MARKLEY, &markley, "Markley method"},
    {KES_SOL_MIKKOLA, &mikkola, "Mikkola method"},
    {KES_SOL_NEWRAP,  &newrap,  "Newton-Raphson method"},
    {KES_SOL_NIJENH,  &nijenh,  "Nijenhuis method"},
    {KES_SOL_SECANT,  &secant,  "Secant method"},
    {KES_SOL_WEGSEC,  &wegsec,  "Wegstein's secant modification"}
};

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
 * FUNCTION    : kes_solver_ell
 * DESCRIPTION : specialized solver function for elliptic case
 * INPUT       : - eccentricity "ecc"
 *               - mean anomaly "ma" (in radians)
 *               - starter method "init" from enum kes_stm_e
 *               - solver method "iter" from enum kes_sol_e
 *               - pointer "data" to data structure of type "kes_input_t"
 *               - pointer "status" to return error code (see error.h)
 * OUTPUT      : solution of elliptic Kepler Equation (in radians)
 ******************************************************************************/
static double kes_solver_ell(
    const double ecc,
    const double ma,
    const kes_stm_e init,
    const kes_sol_e iter,
    kes_input_t*    data,
    kes_err_e*      status
    )
{
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

    /* call starter function
     * NOTE use special starter for Nijenhuis method
     * TODO FIXME check that starter method really applies to elliptic case
     */
    if ( iter != KES_SOL_NIJENH )
    {
        data->starter = kes_starter( ecc, redma, init, status );
    } // end if
    else
    {
        data->starter = kes_starter( ecc, redma, KES_STM_ES07, status );
    } // end else

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
    *status += KES_ERR_NOERR;

    /* finally return result */
    return( data->result );
} // end kes_solver_ell

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
    /* reset error status */
    *status = KES_ERR_NOERR;

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

    /* check input parameters: {tolf, tolx, maxiter} */
    kes_check_input( data );

    /* check eccentricity domain */
    const kes_ecc_e type = kes_check_ecc( ecc, status );

    /* choose specialized solver based on eccentricity */
    data->result = 0.0;
    switch ( type )
    {
        /* exception: invalid eccentricity */
        case KES_ECC_NEG:
            /* error handling */
            *status = KES_ERR_BADECC;
#if KES_SOLVER_DEBUG
    kes_show_error( *status );
#endif
            data->result = 0.0;
            break;

        /* circular case: return solution immediately */
        case KES_ECC_CIRC:
            *status = KES_ERR_NOERR;
#if KES_SOLVER_DEBUG
    kes_show_error( *status );
#endif
            data->result = ma;
            break;

        /* elliptic case: call iterator and return solution */
        case KES_ECC_ELL:
            *status = KES_ERR_NOERR;
#if KES_SOLVER_DEBUG
    kes_show_error( *status );
#endif
            data->result = kes_solver_ell(
                ecc, ma, init, iter, data, status
            );
            break;

        /* TODO FIXME hyperbolic & parabolic cases not yet implemented */
        case KES_ECC_PAR:
        case KES_ECC_HYP:
        default:
            *status = KES_ERR_BADECC;
#if KES_SOLVER_DEBUG
    kes_show_error( *status );
#endif
            data->result = 0.0;
            break;
    } // end switch

    /* return final result */
    return( data->result );
} // end kesolver

/******************************************************************************/
