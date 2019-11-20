/*******************************************************************************
 * @file    nijenh.c
 * @brief   solver method KES_SOL_NIJENH = Nijenhuis iteration
 * @author  Bazso Akos
 * @version 1.0, 21 Aug 2012
 *          1.1, 18 Nov 2019
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
#include "nijenh.h"
#include "const.h"
#include "debug.h"
#include "itercore.h"
#include "kepeq.h"
#include "utils.h"

/******************************************************************************/

/* how many iteration steps to use for generalized Newton method */
#define NIJENHUIS_MAX_ITER 3

/******************************************************************************/

/* function sn(x) for approximation of sin(x) - use only in interval [0:pi] */
static inline double snx(const double x)
{
    static const double a = -0.16605; /* sn(x) Taylor series coeff. O(x^3) */
    static const double b =  0.00761; /* sn(x) Taylor series coeff. O(x^5) */
    double x2;

    if (x > M_PI_2)
    {
        return( snx(M_PI - x) );
    } // end if
    else
    {
        x2 = x*x;
        return( x * (1.0 + x2 * (a + b * x2)) );
    } // end else
} // end snx

/******************************************************************************/

/* function for derivative of sn(x) */
static inline double snxd(const double x)
{
    static const double a = -0.49815; /* sn'(x) Taylor series coeff. O(x^2) */
    static const double b =  0.03805; /* sn'(x) Taylor series coeff. O(x^4) */
    double x2;

    if (x > M_PI_2)
    {
        return( -snxd(M_PI - x) );
    } // end if
    else
    {
        x2 = x*x;
        return( 1.0 + x2 * (a + b * x2) );
    } // end else
} // end snxd

/******************************************************************************/

/*******************************************************************************
 *  FUNCTION    : nijenh
 *  DESCRIPTION : iteration function for Nijenhuis' method of order 3-4
 *  INPUT       : - eccentricity "ecc"
 *                - mean anomaly "ma" in radians
 *                - "starter" value for first iteration
 *                - "res" for results
 *  OUTPUT      : number of iterations
 ******************************************************************************/
int nijenh(
    const double ecc,
    const double ma,
    const double starter,
    kes_input_t* res
    )
{
    register int count = 1;      /* iteration counter */
    double       deltax, deltaf; /* error residuals */
    const double e1 = 1.0 - ecc; /* eccentricity complement */

    /* correction factor for absolute error deltaf */
    const double corr = ecc / e1;

    double x, ecosx, esinx;      /* for iteration */
    double f[NIJENHUIS_MAX_ITER+1], h[NIJENHUIS_MAX_ITER+1];

    /* reset error residuals */
    deltax = deltaf = 0.0;
#if KES_SHOW_ITER_STATS
    res->nbrSinEval = res->nbrCosEval = res->nbrFktEval = 0;
#endif

#if KES_ITER_DEBUG
    KES_DEBUG_STM("nijenh", ecc, ma, starter);
#endif

    /* apply Mikkola starter in region (D) -- using ad hoc boundary values */
    if ( (ma < 0.4) && (ecc > 0.6) )
    {
        /*** STEP #1: rough starter ***/

        /* solve cubic equation s^3 + 3*p*s - 2*q = 0 */
        double frac = 1.0 / (0.5 + 4.0 * ecc);     /* common denominator */
        double    p = e1 * frac;                   /* coeff. p */
        double    q = 0.5 * ma * frac;             /* coeff. q */
        double    z = cbrt(sqrt(p*p*p + q*q) + q); /* cube root */
        z = z*z;

        double s = 0.0;
        if (z > 0.0)
        {
            s = 2.0 * q / (z + p + p*p/z);         /* rough starter */
        } // end if

        /*** STEP #2: refined starter ***/

        /* Newton-Raphson method for s(new) = s - g(s) / g'(s),
         * g(s) = (3/40)*s^5 + ((4e+0.5)/3)*s^3 + (1-e)*s - M/3 = 0
         */
        double s2 = s*s;
        if (s > 0.0)
        {
            s -= 0.075 * s * s2 * s2 / (e1 + s2 * (1.0/frac + 0.375 * s2));
            /* TODO use fma(): fma(fma(0.375, s2, 1.0/frac), s2, e1) */
        } // end if
        s2 = s*s;

        /* refined starter */
        x = ma + ecc * s * (3.0 - 4.0 * s2);
        /* TODO use fma(): fma(fma(-4.0, s2, 3.0), ecc*s, ma) */
    } // end if
    /* apply simple starters in regions (A), (B), (C) */
    else
    {
        /*** STEP #1: rough starter ***/

        /* need to call starter method S07 before !! */
        x = starter;                               /* rough starter */

        /*** STEP #2: refined starter ***/

        f[2] = ecc * snx(x);                       /* evaluate sn(x) */
        f[0] = x - f[2] - ma;                      /* modified KE */
        f[1] = 1.0 - ecc * snxd(x);                /* derivative of mod. KE */

        /* single iteration with Halley method for refined starter */
        x -= f[0] / (f[1] - 0.5 * f[0] * f[2] / f[1]);
    } // end else

    /* save refined starter */
    res->starter = x;
#if KES_ITER_DEBUG
    KES_DEBUG_SOL("nijenh", 1, starter, x);
#endif

    /*** STEP #3: final correction ***/

    /* calculate sin, cos */
    kes_sincos(&esinx, &ecosx, x, ecc);
#if KES_SHOW_ITER_STATS
    ++(res->nbrSinEval); ++(res->nbrCosEval);
#endif

    /* calculate function and derivatives */
    f[0] = ma - x + esinx; // f(x)
    f[1] = 1.0 - ecosx;    // f'(x)
    f[2] = 0.5 * esinx;    // f''(x)/2!
    f[3] = ecosx / 6.0;    // f'''(x)/3!
    /* and so on ... f[4] = ... */

    for (register int i = 1; i <= NIJENHUIS_MAX_ITER; ++i)
    {
        deltax = f[i];

        for (register int j = 1; j <= i-1; ++j)
        {
            deltax = deltax * h[j] + f[i-j];
        } // end for(j)

        h[i] = f[0] / deltax;
    } // end for(i)

    if (x > 0.0)
    {
        x += h[NIJENHUIS_MAX_ITER];
    } // end if

    /* update error residuals */
    deltax = fabs( x - res->starter );
    deltaf = fabs( kes_keq_ell(ecc, ma, x) ) * corr;
#if KES_SHOW_ITER_STATS
    ++(res->nbrSinEval); ++(res->nbrFktEval);
#endif

#if KES_ITER_DEBUG
    KES_DEBUG_SOL("nijenh", 2, deltax, deltaf);
#endif

    /* results */
    res->result = x;
    res->errDF  = deltaf;
    res->errDX  = deltax;

    return( count );
} // end nijenh

/******************************************************************************/
