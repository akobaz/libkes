/*******************************************************************************
 * PROGRAM : errors.c
 * PURPOSE : a small example on how to use the Kepler Equation Solver Library
 *           to handle different error codes
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

    /**************************************************************************/

    /* print an error message to stderr for selected error code */
    puts("Case #1: KES_ERR_NOERR ...");
    kes_show_error( KES_ERR_NOERR );
    puts("");

    /**************************************************************************/

    /* force an eccentricity error and receive error KES_ERR_BADECC
     * NOTE ignoring return value of function kesolver()
     */
    puts("Case #2: KES_ERR_BADECC ...");
    (void)kesolver(
        -100.,          // eccentricity
        1.234,          // mean anomaly in radians
        KES_STM_ES01,   // starter function
        KES_SOL_NEWRAP, // solver function
        &input,
        &err
    );
    if ( err == KES_ERR_BADECC )
    {
        kes_show_error( err );
    } // end if
    puts("");

    /**************************************************************************/

    /* force an invalid number error and receive error KES_ERR_BADVAL
     * NOTE possible compiler warning: division by zero
     */
    puts("Case #3: KES_ERR_BADVAL ...");
    (void)kesolver(
        0.567,          // eccentricity
        1./0,           // mean anomaly in radians
        KES_STM_ES01,   // starter function
        KES_SOL_NEWRAP, // solver function
        &input,
        &err
    );
    if ( err == KES_ERR_BADVAL )
    {
        kes_show_error( err );
    } // end if
    puts("");

    /**************************************************************************/

    /* force an invalid starter error and receive error KES_ERR_BADSTM
     * NOTE invalid starter is replaced by default starter,
     * and function would return the solution
     */
    puts("Case #4: KES_ERR_BADSTM ...");
    (void)kesolver(
        0.567,          // eccentricity
        1.234,          // mean anomaly in radians
        0,              // starter function
        KES_SOL_NEWRAP, // solver function
        &input,
        &err
    );
    if ( err == KES_ERR_BADSTM )
    {
        kes_show_error( err );
    } // end if
    puts("");

    /**************************************************************************/

    /* force an invalid solver error and receive error KES_ERR_BADSOL */
    puts("Case #5: KES_ERR_BADSOL ...");
    (void)kesolver(
        0.567,          // eccentricity
        1.234,          // mean anomaly in radians
        KES_STM_ES01,   // starter function
        -1,             // solver function
        &input,
        &err
    );
    if ( err == KES_ERR_BADSOL )
    {
        kes_show_error( err );
    } // end if
    puts("");

    /**************************************************************************/

    /* show default value for error tolerance */
    printf(
        "default error tolerance = %e\n\n",
        kes_get_tolf( &input )
    );

    /* set an invalid error tolerance and receive error KES_ERR_BADTOL */
    puts("Case #6: KES_ERR_BADTOL ...");
    err = kes_set_tolf( &input, -1.0e-3 );
    if ( err == KES_ERR_BADTOL )
    {
        kes_show_error( err );
    } // end if
    puts("");

    /**************************************************************************/

    return 0;
} // end main
