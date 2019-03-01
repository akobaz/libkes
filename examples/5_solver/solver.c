/*******************************************************************************
 * PROGRAM : solver.c
 * PURPOSE : a more elaborate example on the use of the kesolver() function
 *           from the Kepler Equation Solver Library
 * AUTHOR  : Bazso Akos
 * VERSION : 1.0, 24 Feb 2019
 * COMPILE : use Makefile
 ******************************************************************************/
/* include library header file */
#include "libkes.h"

#include <stdio.h>

int main(void)
{
    /* structure for input/output: default (blank) initialization
     * NOTE possible compiler warning: 'missing initializer for field'
     * can be ignored
     */
    kes_input_t data = { 0 };

    /* prepare variable to receive error code from function */
    kes_err_e err;

    /* solve elliptic Kepler Equation */
    const double sol = kesolver(
        0.567,          // eccentricity
        1.234,          // mean anomaly
        KES_STM_ES01,   // starter method
        KES_SOL_NEWRAP, // solver method
        &data,          // input parameters
        &err            // error code
    );

    /* show results */
    if ( err == KES_ERR_NOERR )
    {
        printf(
            "starter  value        = %g\n"
            "solution value        = %g (%g)\n"
            "number of iterations  = %d\n"
            "residual error deltax = %e\n"
            "residual error deltaf = %e\n\n",
            data.starter,
            data.result, sol, // NOTE alternative ways to get result
            data.iterations,
            data.errDX,
            data.errDF
        );
    } // end if

    return 0;
} // end main
