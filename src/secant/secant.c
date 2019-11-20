/*******************************************************************************
 * @file    secant.c
 * @brief   solver method KES_SOL_SECANT = secant iteration
 * @author  Bazso Akos
 * @version 1.0, 21 Aug 2012
 *          1.1, 15 Sep 2019
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
#include "secant.h"
#include "debug.h"
#include "kepeq.h"

/******************************************************************************/

/*******************************************************************************
 *  FUNCTION    : secant
 *  DESCRIPTION : iteration function for secant method
 *  INPUT       : - eccentricity "ecc"
 *                - mean anomaly "ma" in radians
 *                - "starter" value for first iteration
 *                - "res" for results
 *  OUTPUT      : number of iterations
 ******************************************************************************/
int secant(
    const double ecc,
    const double ma,
    const double starter,
    kes_input_t* res
    )
{
    register int count = 0;      /* iteration counter */
    double       deltax, deltaf; /* error residuals */
    double       xl, xr, x;      /* interval: xl <= x <= xr */
    double       fl, fr, fx;     /* fl = f(xl), fr = f(xr), fx = f(x) */

    /* correction factor for absolute error deltaf */
    const double corr = ecc / (1.0 - ecc);

    /* reset error residuals */
    deltax = deltaf = 0.0;
#if KES_SHOW_ITER_STATS
    res->nbrSinEval = res->nbrCosEval = res->nbrFktEval = 0;
#endif

#if KES_ITER_DEBUG
    KES_DEBUG_STM("secant", ecc, ma, starter);
#endif

    /* define lower and upper limits for solution interval;
     * override given starter
     */
    xl = ma;
    xr = ma + ecc;
    deltax = fabs(xr - xl);
#if KES_ITER_DEBUG
    KES_DEBUG_SOL("secant", count, deltax, 0.0);
#endif

    /* check if interval width is small enough */
    if ( deltax < res->tolx )
    {
        res->result = 0.5 * (xl + xr);
        return( 1 );
    } // end if

    /* check if f(xl) is a solution */
    fl = kes_keq_ell( ecc, ma, xl );
#if KES_SHOW_ITER_STATS
    ++(res->nbrSinEval); ++(res->nbrFktEval);
#endif
    if ( fabs(fl) < res->tolf )
    {
        res->result = xl;
        return( 1 );
    } // end if

    /* check if f(xr) is a solution */
    fr = kes_keq_ell( ecc, ma, xr );
#if KES_SHOW_ITER_STATS
    ++(res->nbrSinEval); ++(res->nbrFktEval);
#endif
    if ( fabs(fr) < res->tolf )
    {
        res->result = xr;
        return( 1 );
    } // end if

    /* main iteration loop */
    do
    {
        /* new approximation point */
        x = (fr * xl - fl * xr) / (fr - fl);

        /* function value at new point */
        fx = kes_keq_ell( ecc, ma, x );
#if KES_SHOW_ITER_STATS
        ++(res->nbrSinEval); ++(res->nbrFktEval);
#endif

        /* modify interval borders */
        xl = xr; fl = fr;
        xr = x;  fr = fx;

        ++count;

        /* update error residuals */
        deltax = fabs(xr - xl);
        deltaf = fabs(fx) * corr;

#if KES_ITER_DEBUG
    KES_DEBUG_SOL("secant", count, deltax, deltaf);
#endif
    } while (
        (deltax > res->tolx) &&
        (deltaf > res->tolf) &&
        (count  < res->maxiter)
    ); // end do

    /* results */
    res->result = x;
    res->errDF  = deltaf;
    res->errDX  = deltax;

    return( count );
} // end secant

/******************************************************************************/
