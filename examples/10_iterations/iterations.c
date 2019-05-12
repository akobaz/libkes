/*******************************************************************************
 * PROGRAM : iterations.c
 * PURPOSE : examples program that applies the Kepler Equation Solver
 *           library functions to evaluate Kepler's Equation (ellitic case)
 *           on a 2D grid in (mean anomaly, eccentricity) to count the number
 *           of iterations until convergence to a given tolerance is achieved
 * AUTHOR  : Bazso Akos
 * VERSION : 1.0, 12 May 2019
 * COMPILE : use Makefile
 ******************************************************************************/
/* include library header file */
#include "libkes.h"

#include <stdio.h>
#include <math.h>

int main(void)
{
    static const kes_stm_e init = KES_STM_ES04;   // starter method
    static const kes_sol_e iter = KES_SOL_NEWRAP; // solver method
    static const double    pi   = 3.141592653589793;
    static const int       imax = 256; // resolution along x-axis = mean anomaly
    static const int       jmax =  64; // resolution along y-axis = eccentricity
    static kes_err_e       status;
    static kes_input_t     data;

    /* show info header */
    fprintf(
        stdout,
        "# %s: starter = %d, solver = %d\n"
        "# resolution in (M,e) = (%d, %d)\n"
        "# E_ref (rad), M (rad), ecc, E_out (rad), rel.err, number of iterations\n",
        __FILE__, init, iter, imax, jmax
    );

    /* loop over reference eccentric anomaly */
    for (register int i = 0; i <= imax; i++)
    {
        /* reference value for eccentric anomaly */
        const double ref = pi* (double)i / imax;

        /* loop over eccentricity */
        for (register int j = 0; j < jmax; j++)
        {
            /* current value of eccentricity */
            const double ecc = (double)j / jmax;

            /* current value of mean anomaly */
            const double ma  = ref - ecc * sin(ref);

            /* solution from kesolver() */
            const double sol = kesolver(ecc, ma, init, iter, &data, &status);

            /* relative error of numerical solution */
            const double abserr = fabs(ref - sol);
            const double relerr = ((ref > 0.0) ? abserr / ref : abserr);

            fprintf(
                stdout,
                "%lf %lf %lf %lf %e, %3d\n",
                ref, ma, ecc, sol, relerr, data.iterations
            );
        } // end for(j)

        /* add empty line to start new data block */
        fprintf(stdout, "\n");
    } // end for(i)

    return 0;
} // end main
