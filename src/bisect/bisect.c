/*******************************************************************************
 * MODULE  : bisect.c
 * PURPOSE : solver method KES_SOL_BISECT = bisection method
 * AUTHOR  : Bazso Akos
 * VERSION : 1.0, 21 Aug 2012
 *           1.1, 17 Feb 2019
 ******************************************************************************/
/* include standard headers */
#include <math.h>

/* include module headers */
#include "bisect.h"
#include "debug.h"
#include "kepeq.h"

/******************************************************************************/

/*******************************************************************************
 *  FUNCTION    : bisect
 *  DESCRIPTION : bisection method = interval halving
 *  INPUT       : - eccentricity "ecc"
 *                - mean anomaly "ma" in radians
 *                - "starter" value for first iteration
 *                - "res" for results
 *  OUTPUT      : number of iterations
 ******************************************************************************/
int bisect(
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
    KES_DEBUG_STM("bisect", ecc, ma, starter);
#endif

    /* define lower and upper limits for solution, override given starter */
    xl = ma;
    xr = ma + ecc;
    deltax = fabs(xr - xl);
#if KES_ITER_DEBUG
    KES_DEBUG_SOL("bisect", count, deltax, 0.0);
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
        /* new interval midpoint */
        x = 0.5 * (xl + xr);

        /* function value at midpoint */
        fx = kes_keq_ell( ecc, ma, x );
#if KES_SHOW_ITER_STATS
        ++(res->nbrSinEval); ++(res->nbrFktEval);
#endif

        /* modify interval border */
        ((fl * fx < 0.0) ? (xr = x, fr = fx) : (xl = x, fl = fx));

        ++count;

        /* update error residuals */
        deltax = fabs(xr - xl);
        deltaf = fabs(fx) * corr;

#if KES_ITER_DEBUG
    KES_DEBUG_SOL("bisect", count, deltax, deltaf);
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
} // end bisect

/******************************************************************************/
