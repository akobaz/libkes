/*******************************************************************************
 * PROGRAM : basic.cpp
 * PURPOSE : a basic example on including and using the Kepler Equation Solver
 *           Library from a C++ based program
 * AUTHOR  : Bazso Akos
 * VERSION : 1.0, 28 Feb 2019
 * COMPILE : use Makefile
 ******************************************************************************/
/* include library header file */
#include "libkes.h"

#include <iostream>

int main(void)
{
    /* prepare variable for input parameters,
     * initialize with default settings
     */
    kes_input_t input = { 0 };

    /* prepare variable to receive error code from function */
    kes_err_e err;

    /* call solver function with some input and store result */
    const double result = kesolver(
        0.567,          // eccentricity
        1.234,          // mean anomaly in radians
        KES_STM_ES01,   // starter function
        KES_SOL_NEWRAP, // solver function
        &input,         // input parameters
        &err            // error code
    );

    /* show result */
    std::cout << "solution of Kepler equation = " << result << std::endl;

    return 0;
} // end main
