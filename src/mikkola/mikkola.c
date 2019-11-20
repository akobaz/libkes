/*******************************************************************************
 * @file    mikkola.c
 * @brief   solver method KES_SOL_MIKKOLA = Mikkola iteration
 * @author  Bazso Akos
 * @version 1.0, 21 Aug 2012
 *          1.1, 20 Nov 2019
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
#include "mikkola.h"
#include "const.h"
#include "debug.h"
#include "itercore.h"
#include "kepeq.h"

/******************************************************************************/

/*******************************************************************************
 *  FUNCTION    : mikkola
 *  DESCRIPTION : iteration function for Mikkola's method of order 5
 *  INPUT       : - eccentricity "ecc"
 *                - mean anomaly "ma" in radians
 *                - "starter" value for first iteration
 *                - "res" for results
 *  OUTPUT      : number of iterations
 ******************************************************************************/
int mikkola(
    const double ecc,
    const double ma,
    const double starter,
    kes_input_t* res
    )
{
    register int count = 1;      /* iteration counter */
    double       deltax, deltaf; /* error residuals */

    /* correction factor for absolute error deltaf */
    const double corr = ecc / (1.0 - ecc);

    /* reset error residuals */
    deltax = deltaf = 0.0;
#if KES_SHOW_ITER_STATS
    res->nbrSinEval = res->nbrCosEval = res->nbrFktEval = 0;
#endif

#if KES_ITER_DEBUG
    KES_DEBUG_STM("mikkola", ecc, ma, starter);
#endif

    /*** STEP #1: find starter from cubic approximation ***/

    /* solve cubic equation s^3 + 3*a*s - 2*b = 0 */
    double       a = 1.0 / (0.5 + 4.0 * ecc);       /* common denominator */
    const double b = 0.5 * ma * a;                  /* coeff. b */
    a *= (1.0 - ecc);                               /* coeff. a */
    const double c = cbrt( sqrt(a*a*a + b*b) + b ); /* cube root */

    double s = 0.0, s2 = 0.0;
    if (c > 0.0)
    {
        /* sol. of cubic approx.
         * TODO FIXME use Nijenhuis' formula to prevent cancellation
         */
        s = c - a / c;
    } // end if
    s2 = s*s;

    /* correction term O(s^5) */
    deltax = -0.078 * s * s2 * s2 / (1.0 + ecc);
    s += deltax;
    s2 = s*s;

    /* starting approx. */
    double x = ma + ecc * s * (3.0 - 4.0 * s2);
#if KES_ITER_DEBUG
    KES_DEBUG_SOL("mikkola", 1, c, x);
#endif

    /* save starter */
    res->starter = x;

    /*** STEP #2: fifth order correction ***/

    /* call fifth order method */
    x = kes_itercore5( ecc, ma, x );
#if KES_SHOW_ITER_STATS
    ++(res->nbrSinEval); ++(res->nbrCosEval); ++(res->nbrFktEval);
#endif

    /* update error residuals */
    deltax = fabs( x - res->starter );
    deltaf = fabs( kes_keq_ell( ecc, ma, x ) ) * corr;
#if KES_SHOW_ITER_STATS
    ++(res->nbrSinEval); ++(res->nbrFktEval);
#endif

#if KES_ITER_DEBUG
    KES_DEBUG_SOL("mikkola", 2, deltax, deltaf);
#endif

    /* results */
    res->result = x;
    res->errDF  = deltaf;
    res->errDX  = deltax;

    return( count );
} // end mikkola

/******************************************************************************/
