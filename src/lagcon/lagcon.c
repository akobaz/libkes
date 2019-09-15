/*******************************************************************************
 * @file    lagcon.c
 * @brief   solver method KES_SOL_LAGCON = Laguerre-Conway iteration
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
#include "lagcon.h"
#include "debug.h"
#include "itercore.h"
#include "kepeq.h"
#include "utils.h"

/******************************************************************************/

/*******************************************************************************
 *  FUNCTION    : lagcon
 *  DESCRIPTION : iteration function for Laguerre-Conway method of order 3
 *  INPUT       : - eccentricity "ecc"
 *                - mean anomaly "ma" in radians
 *                - "starter" value for first iteration
 *                - "res" for results
 *  OUTPUT      : number of iterations
 ******************************************************************************/
int lagcon(
    const double ecc,
    const double ma,
    const double starter,
    kes_input_t* res
    )
{
    register int count = 0;
    double       deltax, deltaf; /* error residuals */
    double       x, dx, f0, f1;  /* */
	double       ecosx, esinx;   /* */

    /* correction factor for absolute error deltaf */
    const double corr = ecc / (1.0 - ecc);

    /* reset error residuals */
    deltax = deltaf = 0.0;
#if KES_SHOW_ITER_STATS
    res->nbrSinEval = res->nbrCosEval = res->nbrFktEval = 0;
#endif

#if KES_ITER_DEBUG
    KES_DEBUG_STM("lagcon", ecc, ma, starter);
#endif

    /* use starter to initialize iteration */
    x = starter;

    /* main iteration loop */
    do
    {
        /* calculate sin, cos */
        kes_sincos( &esinx, &ecosx, x, ecc );

        /* evaluate Kepler Equation and its first derivative */
        f0 = x - esinx - ma;
        f1 = 1.0 - ecosx;
#if KES_SHOW_ITER_STATS
        ++(res->nbrSinEval); ++(res->nbrCosEval); ++(res->nbrFktEval);
#endif

        /* increment, eq. (16) */
        dx = 5.0 * f0 / (f1 + sqrt( fabs(16.0 * f1 * f1 - 20.0 * f0 * esinx) ));

        /* TODO FIXME use compensated summation */
        x -= dx;

        ++count;

        /* update error residuals */
        deltax = fabs(dx);
        deltaf = fabs(f0) * corr;

#if KES_ITER_DEBUG
    KES_DEBUG_SOL("lagcon", count, deltax, deltaf);
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
} // end lagcon

/******************************************************************************/
