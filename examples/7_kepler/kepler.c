/*******************************************************************************
 * PROGRAM : kepler.c
 * PURPOSE : some small examples on how to use the Kepler Equation Solver
 *           library functions to evaluate Kepler's Equations for the
 *           (a) elliptic, (b) parabolic, and (c) hyperbolic cases
 * AUTHOR  : Bazso Akos
 * VERSION : 1.0, 25 Feb 2019
 * COMPILE : use Makefile
 ******************************************************************************/
/* include library header file */
#include "libkes.h"

#include <stdio.h>

int main(void)
{
    /* elliptic case: 0 < e < 1 */
    const double ell = kes_keq_ell(
        0.567, // eccentricity
        1.234, // mean anomaly
        1.789  // elliptic eccentric anomaly
    );

    /* parabolic case: e = 1 */
    const double par = kes_keq_par(
        1.0,   // eccentricity, effectively ignored
        1.234, // mean anomaly
        1.789  // true anomaly !!!
    );

    /* hyperbolic case: e > 1 */
    const double hyp = kes_keq_hyp(
        1.567, // eccentricity
        1.234, // mean anomaly
        1.789  // hyperbolic eccentric anomaly
    );

    /* show results from different function evaluations;
     * NOTE these are arbitrary numbers, not the 'true' solutions
     */
    printf(
        "result from elliptic   Kepler equation = %g\n"
        "result from parabolic  Kepler equation = %g\n"
        "result from hyperbolic Kepler equation = %g\n",
        ell, par, hyp
    );

    return 0;
} // end main
