/*******************************************************************************
 * PROGRAM : basic.c
 * PURPOSE : a basic example on how to use the Kepler Equation Solver Library
 *           main solver function kesolver()
 * AUTHOR  : Bazso Akos
 * VERSION : 1.0, 24 Feb 2019
 * COMPILE : use Makefile
 ******************************************************************************/
/* include library header file */
#include "libkes.h"

#include <stdio.h>

int main(void)
{
    /* prepare variable for input parameters,
     * initialize with default settings
     */
    kes_input_t input = { 0 };

    /* prepare variable to receive error code from function */
    kes_err_e err;

    /* call solver function with some input and store result */
    double result = kesolver(
        0.567,          // eccentricity
        1.234,          // mean anomaly in radians
        KES_STM_ES01,   // starter function
        KES_SOL_NEWRAP, // solver function
        &input,         // input parameters
        &err            // error code
    );

    /* show result */
    printf("solution of Kepler equation = %g\n", result);

    return 0;
} // end main
