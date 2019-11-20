/*******************************************************************************
 * @file    wegsec.c
 * @brief   solver method KES_SOL_WEGSEC = Wegstein secant iteration method
 * @author  Bazso Akos
 * @version 1.0, 21 Aug 2012
 *          1.1, 20 Sep 2019
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
#include "wegsec.h"
#include "debug.h"
#include "kepeq.h"

/******************************************************************************/

/*******************************************************************************
 *  FUNCTION    : wegsec
 *  DESCRIPTION : Wegstein's secant iteration method
 *  INPUT       : - eccentricity "ecc"
 *                - mean anomaly "ma" in radians
 *                - "starter" value for first iteration
 *                - "res" for results
 *  OUTPUT      : number of iterations
 ******************************************************************************/
int
wegsec(
    const double ecc,
    const double ma,
    const double starter,
    kes_input_t* res
    )
{
    register int count = 0;      /* iteration counter */
    double       deltax, deltaf; /* error residuals */
    double       x0, x1, x2;     /* successive approximations */
    double       y0, y1, y2;     /* function evaluation y=f(x) */
    //swap;

    /* correction factor for absolute error deltaf */
    const double corr = ecc / (1.0 - ecc);

    /* reset error residuals */
    deltax = deltaf = 0.0;
#if KES_SHOW_ITER_STATS
    res->nbrSinEval = res->nbrCosEval = res->nbrFktEval = 0;
#endif

#if KES_ITER_DEBUG
    KES_DEBUG_STM("wegsec", ecc, ma, starter);
#endif

    /* first starting point (x0,y0) */
    x0 = starter;
    y0 = ma + ecc * sin(x0); // f(x0)
#if KES_SHOW_ITER_STATS
    ++(res->nbrSinEval);
#endif

    /* second starting point (x1,y1) */
    x1 = y0;                 // next point
    y1 = ma + ecc * sin(x1); // f(x1)
#if KES_SHOW_ITER_STATS
    ++(res->nbrSinEval);
#endif

    /*** TODO FIXME
     * - check if f(x1) is a solution
     * - check if interval width x0-x1 is small enough
     ***/

    /* main iteration loop */
    do
    {
        /* new refinement step via equations on page 176 */
        x2 = x1 + (x1 - x0) / ((x0 - y0)/(x1 - y1) - 1.0);
        y2 = ma + ecc * sin(x2); // f(x2)
#if KES_SHOW_ITER_STATS
    ++(res->nbrSinEval);
#endif

        ++count;

        /* update error residuals */
        deltax = fabs(x1 - x2);
        deltaf = fabs( kes_keq_ell(ecc, ma, x2) ) * corr;
#if KES_SHOW_ITER_STATS
        ++(res->nbrSinEval); ++(res->nbrFktEval);
#endif

#if KES_ITER_DEBUG
    KES_DEBUG_SOL("wegsec", count, deltax, deltaf);
#endif

        /* shift old/new variables */
        x0 = x1; x1 = x2;
        y0 = y1; y1 = y2;
    } while (
        (deltax > res->tolx) &&
        (deltaf > res->tolf) &&
        (count  < res->maxiter)
    ); // end do

    /* results */
    res->result = x2;
    res->errDF  = deltaf;
    res->errDX  = deltax;

    return( count );
} // end wegsec

/******************************************************************************/
