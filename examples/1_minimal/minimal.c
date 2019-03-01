/*******************************************************************************
 * PROGRAM : minimal.c
 * PURPOSE : a minimal example on how to use the Kepler Equation Solver Library
 *              1) include the header file "libkes.h", and
 *              2) link to the static library "libkes.a"
 * AUTHOR  : Bazso Akos
 * VERSION : 1.0, 23 Feb 2019
 * COMPILE : use Makefile
 ******************************************************************************/
/* include library header file */
#include "libkes.h"

int main(void)
{
    /* print the version string of the library and exit */
    kes_show_version();
    return 0;
} // end main
