/*******************************************************************************
 * MODULE  : fixpkt.c
 * PURPOSE : solver method KES_SOL_FIXEDP = fixed-point iteration
 * AUTHOR  : Bazso Akos
 * VERSION : 1.0, 21 Aug 2012
 *           1.1, 17 Feb 2019
 ******************************************************************************/
/* include standard headers */
#include <math.h>

/* include module headers */
#include "fixedp.h"
#include "debug.h"
#include "kepeq.h"

/******************************************************************************/

/*******************************************************************************
 *  FUNCTION    : fixedp
 *  DESCRIPTION : fixed-point iteration
 *  INPUT       : - eccentricity "ecc"
 *                - mean anomaly "ma" in radians
 *                - "starter" value for first iteration
 *                - "res" for results
 *  OUTPUT      : number of iterations
 ******************************************************************************/
int fixedp(
    const double ecc,
    const double ma,
    const double starter,
    kes_input_t* res
    )
{
    register int count = 0;      /* iteration counter */
    double       deltax, deltaf; /* error residuals */
    double       x, xsave, fx;   /* new and old approximation, f(x) */

    /* correction factor for absolute error deltaf */
    const double corr = ecc / (1.0 - ecc);

    /* reset error residuals */
    deltax = deltaf = 0.0;
#if KES_SHOW_ITER_STATS
    res->nbrSinEval = res->nbrCosEval = res->nbrFktEval = 0;
#endif

#if KES_ITER_DEBUG
    KES_DEBUG_STM("fixedp", ecc, ma, starter);
#endif

    /* use starter for initializing iteration */
    x = starter;

    /* main iteration loop */
    do
    {
        xsave = x;

        /* new approximation */
        x = ma + ecc * sin(xsave);

        /* function value at new point */
        fx = kes_keq_ell( ecc, ma, x );
#if KES_SHOW_ITER_STATS
        ++(res->nbrSinEval); ++(res->nbrFktEval);
#endif

        ++count;

        /* update error residuals
         * NOTE deltax(n+1) = deltaf(n), lags one iteration behind
         */
        deltax = fabs(x - xsave);
        deltaf = fabs(fx) * corr;

#if KES_ITER_DEBUG
    KES_DEBUG_SOL("fixedp", count, deltax, deltaf);
#endif
    } while (
        (deltaf > res->tolf) &&
        (count  < res->maxiter)
    ); // end do

    /* results */
    res->result = x;
    res->errDF  = deltaf;
    res->errDX  = deltax;

    return( count );
} // end fixedp

/******************************************************************************/
