/*******************************************************************************
 * PROGRAM : anomaly.c
 * PURPOSE : a small example on the relations between the different anomalies
 *           (mean, eccentric, true) and how to find them using the
 *           Kepler Equation Solver Library
 * AUTHOR  : Bazso Akos
 * VERSION : 1.0, 25 Feb 2019
 * COMPILE : use Makefile
 ******************************************************************************/
/* include library header file */
#include "libkes.h"

#include <stdio.h>
#include <math.h>

int main(void)
{
    /* prepare variable for input parameters,
     * initialize with default settings
     */
    kes_input_t input = { 0 };

    /* prepare variable to receive error code from function */
    kes_err_e err;

    /* set eccentricity */
    const double ecc = 0.567;

    /* start with reference value for eccentric anomaly */
    const double ea_ref = 1.234;

    /* calculate corresponding mean anomaly;
     * trick: evaluating Kepler's Equation but setting 2nd parameter to zero
     * returns residual mean anomaly: ma = x - ecc*sin(x)
     */
    const double ma = kes_keq_ell( ecc, 0.0, ea_ref );
    printf("mean anomaly = %g\n", ma);

    /* find numerical approximation to eccentric anomaly */
    const double ea_num = kesolver(
        ecc, ma, KES_STM_ES04, KES_SOL_NEWRAP, &input, &err
    );
    printf(
        "numerical eccentric anomaly = %.15f\n"
        "reference eccentric anomaly = %.15f\n"
        "absolute error              = %e\n"
        "relative error              = %e\n",
        ea_num, ea_ref, fabs(ea_num-ea_ref), fabs(ea_num-ea_ref)/ea_ref
    );

    /* calculate corresponding true anomaly */
    const double ta = kes_trueanom( ecc, ea_num );
    printf("true anomaly = %g\n", ta);

    return 0;
} // end main
