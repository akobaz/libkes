/*******************************************************************************
 * PROGRAM : input.c
 * PURPOSE : some examples on how to use the Kepler Equation Solver Library
 *           functions to set parameters in the input parameter structure
 * AUTHOR  : Bazso Akos
 * VERSION : 1.0, 27 Feb 2019
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

    /* call solver with default parameters */
    const double sol = kesolver(
        0.567,          // eccentricity
        1.234,          // mean anomaly
        KES_STM_ES01,   // starter method
        KES_SOL_NEWRAP, // solver method
        &data,          // input parameters
        &err            // error code
    );
    printf(
        "kesolver() returns ...\n"
        "\tsolution        = %g\n"
        "\terror residuals = %e %e\n"
        "\terror code      = %d (0 is OK)\n\n",
        sol, data.errDX, data.errDF, err
    );

    /* get default values for error tolerances */
    double tolf = kes_get_tolf( &data );
    double tolx = kes_get_tolx( &data );

    /* get default value for max. number of iterations */
    int maxiter = kes_get_maxiter( &data );

    /* show default values */
    printf(
        "error tolerance for variable x    = %e\n"
        "error tolerance for function f(x) = %e\n"
        "maximum number of iterations      = %d\n\n",
        tolx, tolf, maxiter
    );

    /* set different error tolerance */
    err = kes_set_tolf( &data, 1e-10 );
    if ( err == KES_ERR_NOERR )
    {
        printf("new tolerance set to 1e-10 ... OK\n");
    } // end if

    /* try to set an invalid tolerance: tolerance > 1 */
    err = kes_set_tolx( &data, 1e12 );
    if ( err == KES_ERR_BADTOL )
    {
        printf("new tolerance set to 1e+12 ... Error\n");
    } // end if

    /* set new max. number of iterations */
    err = kes_set_maxiter( &data, 10 );
    if ( err == KES_ERR_NOERR )
    {
        printf("new maxiter set to 10 ... OK\n");
    } // end if

    /* try to set an invalid maxiter: maxiter < 0 */
    err = kes_set_maxiter( &data, -5 );
    if ( err == KES_ERR_BADVAL )
    {
        printf("new maxiter set to -5 ... Error\n");
    } // end if

    /* set new parameters manually */
    data.tolx = data.tolf = 1e-10;
    data.maxiter = 10;
        printf(
        "\n"
        "error tolerance for variable x    = %e\n"
        "error tolerance for function f(x) = %e\n"
        "maximum number of iterations      = %d\n\n",
        data.tolx, data.tolf, data.maxiter
    );

    /* call solver again with these parameters */
    const double solnew = kesolver(
        0.567,          // eccentricity
        1.234,          // mean anomaly
        KES_STM_ES01,   // starter method
        KES_SOL_NEWRAP, // solver method
        &data,          // input parameters
        &err            // error code
    );
    printf(
        "kesolver() returns ...\n"
        "\tnew solution    = %.9f\n"
        "\told solution    = %.9f\n"
        "\terror residuals = %e %e\n"
        "\terror code      = %d (0 is OK)\n\n",
        solnew, sol, data.errDX, data.errDF, err
    );

    return 0;
} // end main
