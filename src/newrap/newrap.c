/*******************************************************************************
 * MODULE  : newrap.c
 * PURPOSE : solver method KES_SOL_NEWRAP = Newton-Raphson iteration
 * AUTHOR  : Bazso Akos
 * VERSION : 1.0, 21 Aug 2012
 *           1.1, 17 Feb 2019
 ******************************************************************************/
/* include standard headers */
#include <math.h>

/* include module headers */
#include "newrap.h"
#include "debug.h"
#include "kepeq.h"

/******************************************************************************/

/*******************************************************************************
 *  FUNCTION    : newrap
 *  DESCRIPTION : Newton-Raphson iteration
 *  INPUT       : - eccentricity "ecc"
 *                - mean anomaly "ma" in radians
 *                - "starter" value for first iteration
 *                - "res" for results
 *  OUTPUT      : number of iterations
 ******************************************************************************/
int newrap(
    const double ecc,
    const double ma,
    const double starter,
    kes_input_t* res
    )
{
    register int count = 0;        /* iteration counter */
    double       deltax, deltaf;   /* error residuals */
    double       x, xsave, dx, fx; /* iteration variables */

    /* correction factor for absolute error deltaf */
    const double corr = ecc / (1.0 - ecc);

    /* reset error residuals */
    deltax = deltaf = 0.0;
#if KES_SHOW_ITER_STATS
    res->nbrSinEval = res->nbrCosEval = res->nbrFktEval = 0;
#endif

#if KES_ITER_DEBUG
    KES_DEBUG_STM("newrap", ecc, ma, starter);
#endif

    /* use starter for initializing iteration */
    x = starter;

    /* main iteration loop */
    do
    {
        xsave = x;

        /* TODO FIXME use function for first derivative of kes_keq_ell */
        dx = kes_keq_ell( ecc, ma, x ) / (1.0 - ecc * cos(x));
#if KES_SHOW_ITER_STATS
        ++(res->nbrSinEval); ++(res->nbrCosEval); ++(res->nbrFktEval);
#endif

        /* TODO FIXME
         * - use compensated summation
         * - check range 0 <= x < M_PI, reduce x
         */
        x -= dx;
        //x += dx; // TODO FIXME + or -

        fx = kes_keq_ell( ecc, ma, x );
#if KES_SHOW_ITER_STATS
        ++(res->nbrSinEval); ++(res->nbrFktEval);
#endif

        ++count;

        /* update error residuals */
        deltax = fabs(x - xsave);
        deltaf = fabs(fx) * corr;

#if KES_ITER_DEBUG
    KES_DEBUG_SOL("newrap", count, deltax, deltaf);
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
} // end newrap

/******************************************************************************/
