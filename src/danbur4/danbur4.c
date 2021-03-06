/*******************************************************************************
 * @file    danbur4.c
 * @brief   solver method KES_SOL_DANBUR4 = Danby-Burkardt method of order 4
 * @author  Bazso Akos
 * @version 1.0, 21 Aug 2012
 *          1.1, 11 May 2019
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
/* include standard headers */
#include <math.h>

/* include module headers */
#include "danbur4.h"
#include "debug.h"
#include "itercore.h"
#include "kepeq.h"

/******************************************************************************/

/*******************************************************************************
 *  FUNCTION    : danbur4
 *  DESCRIPTION : iteration function for Danby-Burkardt method of order 4
 *  INPUT       : - eccentricity "ecc"
 *                - mean anomaly "ma" in radians
 *                - "starter" value for first iteration
 *                - "res" for results
 *  OUTPUT      : number of iterations
 ******************************************************************************/
int danbur4(
    const double ecc,
    const double ma,
    const double starter,
    kes_input_t* res
    )
{
    register int count = 0;      /* iteration counter */
    double       deltax, deltaf; /* error residuals */
    double       xnew, xold, fx; /* iteration variables */

    /* correction factor for absolute error deltaf */
    const double corr = ecc / (1.0 - ecc);

    /* reset error residuals */
    deltax = deltaf = 0.0;
#if KES_SHOW_ITER_STATS
    res->nbrSinEval = res->nbrCosEval = res->nbrFktEval = 0;
#endif

#if KES_ITER_DEBUG
    KES_DEBUG_STM("danbur4", ecc, ma, starter);
#endif

    /* use starter for initializing iteration */
    xnew = starter;

    /* main iteration loop */
    do
    {
        xold = xnew;

        /* call DANBUR4 iteration method */
        xnew = kes_itercore4( ecc, ma, xold );
#if KES_SHOW_ITER_STATS
        ++(res->nbrSinEval); ++(res->nbrCosEval); ++(res->nbrFktEval);
#endif

        fx = kes_keq_ell( ecc, ma, xnew );
#if KES_SHOW_ITER_STATS
        ++(res->nbrSinEval); ++(res->nbrFktEval);
#endif

        ++count;

        /* update error residuals */
        deltax = fabs(xnew - xold);
        deltaf = fabs(fx) * corr;

#if KES_ITER_DEBUG
    KES_DEBUG_SOL("danbur4", count, deltax, deltaf);
#endif
    } while (
        (deltax > res->tolx) &&
        (deltaf > res->tolf) &&
        (count  < res->maxiter)
    ); // end do

    /* results */
    res->result = xnew;
    res->errDF  = deltaf;
    res->errDX  = deltax;

    return( count );
} // end danbur4

/******************************************************************************/
