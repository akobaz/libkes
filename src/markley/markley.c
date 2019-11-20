/*******************************************************************************
 * @file    markley.c
 * @brief   solver method KES_SOL_MARKLEY = Markley iteration
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
#include "markley.h"
#include "const.h"
#include "debug.h"
#include "itercore.h"
#include "kepeq.h"

/******************************************************************************/

/*******************************************************************************
 *  FUNCTION    : markley
 *  DESCRIPTION : iteration function for Markley's method of order 5
 *  INPUT       : - eccentricity "ecc"
 *                - mean anomaly "ma" in radians
 *                - "starter" value for first iteration
 *                - "res" for results
 *  OUTPUT      : number of iterations
 ******************************************************************************/
int markley(
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
    KES_DEBUG_STM("markley", ecc, ma, starter);
#endif

    /* calculate parameters */
    double       ad = 1.0 / (M_PISQ - 6.0);
    const double ak = 1.6 * M_PI * ad;
    ad *= 3.0 * M_PISQ;

    /* alpha(e,M) : eq. (20) */
    const double a  = ad + ak * (M_PI - ma) / (1.0 + ecc);

    /* d : eq. (5) */
    const double d  = 3.0 * (1.0 - ecc) + a * ecc;

    /* q : eq. (9) */
    const double q  = 2.0 * a * d * (1.0 - ecc) - ma * ma;

    /* r : eq. (10) */
    const double r  = 3.0 * a * d * (d - 1.0 + ecc) * ma + ma * ma * ma;

    /* w^(2/3) : eq. (14) */
    double       w = cbrt( fabs(r) + sqrt(q * q * q + r * r) );
    w *= w;

    /*** STEP #1: find starter from Pade approximation ***/

    /* calculate starter */
    double x = 0.0;
    if (w > 0.0)
    {
        /* starter value x0 */
        x = (2.0 * r * w / (w * w + q * w + q * q) + ma) / d;
    } // end if
#if KES_ITER_DEBUG
    KES_DEBUG_SOL("markley", 1, w, x);
#endif

    /* save starter */
    res->starter = x;

    /*** STEP #2: fifth order correction ***/

    /* call fifth order method */
    x = kes_itercore5( ecc, ma, x );
#if KES_SHOW_ITER_STATS
    ++(res->nbrSinEval); ++(res->nbrCosEval); ++(res->nbrCosEval);
#endif

    /* update error residuals */
    deltax = fabs( x - res->starter );
    deltaf = fabs( kes_keq_ell( ecc, ma, x ) ) * corr;
#if KES_SHOW_ITER_STATS
    ++(res->nbrSinEval); ++(res->nbrFktEval);
#endif

#if KES_ITER_DEBUG
    KES_DEBUG_SOL("markley", 2, deltax, deltaf);
#endif

    /* results */
    res->result = x;
    res->errDF  = deltaf;
    res->errDX  = deltax;

    return( count );
} // end markley

/******************************************************************************/
