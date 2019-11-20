/*******************************************************************************
 * PROGRAM : input2.c
 * PURPOSE : some more examples on the use of the Kepler Equation Solver Library
 *           input parameter structure utility functions
 * AUTHOR  : Bazso Akos
 * VERSION : 1.0, 28 Sep 2019
 * COMPILE : use Makefile
 ******************************************************************************/
/* include library header file */
#include "libkes.h"

#include <stdio.h>

/******************************************************************************/

/* print accessible fields */
void print_input(const kes_input_t in)
{
    fprintf(
        stdout,
        "tolf = %g, tolx = %g, maxiter = %d\n\n",
        in.tolf, in.tolx, in.maxiter
    );
    return;
} // end print_input

/******************************************************************************/

/* print all relevant fields */
void print_all(const kes_input_t in)
{
    fprintf(
        stdout,
        "tolf = %g, tolx = %g, maxiter = %d\n"
        "result = %g, starter = %g\n"
        "errDF = %e, errDX = %e\n"
        "iter = %d, nbrSinEval = %d, nbrCosEval = %d, nbrFktEval = %d\n\n",
        in.tolf, in.tolx, in.maxiter,
        in.result, in.starter,
        in.errDF, in.errDX,
        in.iterations, in.nbrSinEval, in.nbrCosEval, in.nbrFktEval
    );
    return;
} // end print_all

/******************************************************************************/

int main(void)
{
    /* create structure with default arguments */
    puts("kes_new_input test case #1");
    kes_input_t data1 = kes_new_input();
    print_input( data1 );

    /* create structure with some user-defined arguments */
    puts("kes_new_input test case #2");
    kes_input_t data2 = kes_new_input( .maxiter = 123 );
    print_input( data2 );

    /* create structure with only user-defined arguments */
    puts("kes_new_input test case #3");
    kes_input_t data3 = kes_new_input( .tolf=1e-11, .tolx=1e-22, .maxiter=33 );
    print_input( data3 );

    /* prepare variable to receive error code from function */
    kes_err_e err;

    /* call solver with default parameters */
    const double sol = kesolver(
        0.567,          // eccentricity
        1.234,          // mean anomaly
        KES_STM_ES01,   // starter method
        KES_SOL_NEWRAP, // solver method
        &data1,         // input parameters
        &err            // error code
    );

    /* print all relevant data fields */
    print_all( data1 );

    return 0;
} // end main
