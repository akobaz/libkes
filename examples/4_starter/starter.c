/*******************************************************************************
 * PROGRAM : starter.c
 * PURPOSE : a small example on how to use the Kepler Equation Solver Library
 *           to generate starting values (initial guesses of the solution)
 *           for use in the iteration process
 * AUTHOR  : Bazso Akos
 * VERSION : 1.0, 25 Feb 2019
 * COMPILE : use Makefile
 ******************************************************************************/
/* include library header file */
#include "libkes.h"

#include <stdio.h>

int main(void)
{
    /* prepare variable to receive error code from function */
    kes_err_e err;

    /* generate starter value;
     * NOTE should be equal to input mean anomaly for KES_STM_ES01
     */
    const double starter = kes_starter(
        0.567,          // eccentricity
        1.234,          // mean anomaly
        KES_STM_ES01,   // starter method, choose from enum kes_stm_e
        &err            // error code
    );

    if ( err == KES_ERR_NOERR )
    {
        printf("starter value = %g\n", starter);
    } // end if

    /* loop through all available starter methods */
    for (kes_stm_e stm = KES_STM_NONE+1; stm < KES_STM_TOTAL; stm++)
    {
        /* ignore error code */
        const double ret = kes_starter( 0.567, 1.234, stm, &err );
        printf("starter value #%02d = %f\n", (int)stm, ret);
    } // end for

    return 0;
} // end main
