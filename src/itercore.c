/*******************************************************************************
 * MODULE  : itercore.c
 * PURPOSE : iteration functions for Kepler Equation Solver Library
 * AUTHOR  : Bazso Akos
 * VERSION : 1.0, 21 Aug 2012
 *           1.1, 30 Sep 2013
 *           1.2, 23 Feb 2019
 * REFERENCE : Danby & Burkardt (1983), Celestial Mechanics 31, p.95-107
 ******************************************************************************/
/* include standard headers */
#include <math.h>

/* include module headers */
#include "itercore.h"
#include "utils.h"

/* TODO FIXME use functions from module kepeq.c */
//#include "kepeq.h"

/******************************************************************************/

/* use verbose debug output? 0 = no (default), 1 = yes */
#define KES_ITERCORE_DEBUG 0

/******************************************************************************/

/* tiny increment to denominator, fixes divide by zero problem
 * for vanishing first derivative at (x0,e) = (0,1)
 */
static const double addzero = 1.0e-19;

/******************************************************************************/

/*******************************************************************************
 *  FUNCTION    : itercore2
 *  DESCRIPTION : iteration method: Newton-Raphson, quadratic convergence
 *  INPUT       : - eccentricity "ecc"
 *                - mean anomaly "ma" (in radians)
 *                - old value of iteration variable "x0"
 *  OUTPUT      : update to iteration variable x0+dx
 ******************************************************************************/
inline double kes_itercore2(
    const double ecc,
    const double ma,
    const double x0
    )
{
    /* ecc*cos(x), ecc*sin(x) */
    double ecx, esx;

    /* calculate sin, cos */
    kes_sincos( &esx, &ecx, x0, ecc );

    /* evaluate Kepler Equation: 0th derivative */
    const double f0 = ma - x0 + esx;

    /* evaluate Kepler Equation: 1st derivative */
    const double f1 = 1.0 - ecx + addzero;

    /* delta1: eq. (16) */
    const double dx = f0 / f1;

    return( x0 + dx );
} // end kes_itercore2

/******************************************************************************/

/*******************************************************************************
 *  FUNCTION    : itercore3
 *  DESCRIPTION : iteration method: Halley, cubic convergence
 *  INPUT       : - eccentricity "ecc"
 *                - mean anomaly "ma" (in radians)
 *                - old value of iteration variable "x0"
 *  OUTPUT      : update to iteration variable x0+dx
 ******************************************************************************/
inline double kes_itercore3(
    const double ecc,
    const double ma,
    const double x0
    )
{
    double ecx, esx; /* ecc*cos(x), ecc*sin(x) */
    double dx;       /* increment deltax */

    /* calculate sin, cos */
    kes_sincos( &esx, &ecx, x0, ecc );

    /* evaluate Kepler Equation: 0th derivative */
    const double f0 = ma - x0 + esx;

    /* evaluate Kepler Equation: 1st derivative */
    const double f1 = 1.0 - ecx + addzero;

    /* delta1: eq. (16) */
    dx = f0 / f1;

    /* evaluate Kepler Equation: scaled 2nd derivative */
    const double f2 = esx / 2.0;

    /* delta2: eq. (17) */
    dx = f0 / (f1 + f2 * dx);
    /* TODO use floating-point multiply and add function
     * fma(x,y,z) = x * y + z
     */
    //dx = f0 / fma(dx, f2, f1);

    return( x0 + dx );
} // end kes_itercore3

/******************************************************************************/

/*******************************************************************************
 *  FUNCTION    : itercore4
 *  DESCRIPTION : iteration method: Danby-Burkardt, quartic convergence
 *  INPUT       : - eccentricity "ecc"
 *                - mean anomaly "ma" (in radians)
 *                - old value of iteration variable "x0"
 *  OUTPUT      : update to iteration variable x0+dx
 ******************************************************************************/
inline double kes_itercore4(
    const double ecc,
    const double ma,
    const double x0
    )
{
    double ecx, esx; /* ecc*cos(x), ecc*sin(x) */
    double dx;       /* increment deltax */

    /* calculate sin, cos */
    kes_sincos( &esx, &ecx, x0, ecc );

    /* evaluate Kepler Equation: 0th derivative */
    const double f0 = ma - x0 + esx;

    /* evaluate Kepler Equation: 1st derivative */
    const double f1 = 1.0 - ecx + addzero;

    /* delta1: eq. (16) */
    dx = f0 / f1;

    /* evaluate Kepler Equation: scaled 2nd derivative */
    const double f2 = esx / 2.0;

    /* delta2: eq. (17) */
    dx = f0 / (f1 + f2 * dx);
    // TODO dx = f0 / fma(dx, f2, f1);

    /* evaluate Kepler Equation: scaled 3rd derivative */
    const double f3 = ecx / 6.0;

    /* delta3: eq. (18) */
    dx = f0 / (f1 + f2 * dx + f3 * dx * dx);
    // TODO dx = f0 / fma(dx, fma(dx, f3, f2), f1);

    return( x0 + dx );
} // end kes_itercore4

/******************************************************************************/

/*******************************************************************************
 *  FUNCTION    : itercore5
 *  DESCRIPTION : iteration method: Danby-Burkardt, quintic convergence
 *  INPUT       : - eccentricity "ecc"
 *                - mean anomaly "ma" (in radians)
 *                - old value of iteration variable "x0"
 *  OUTPUT      : update to iteration variable x0+dx
 ******************************************************************************/
inline double kes_itercore5(
    const double ecc,
    const double ma,
    const double x0
    )
{
    double ecx, esx; /* ecc*cos(x), ecc*sin(x) */
    double dx;       /* increment deltax */

    /* calculate sin, cos */
    kes_sincos( &esx, &ecx, x0, ecc );

    /* evaluate Kepler Equation: 0th derivative */
    const double f0 = ma - x0 + esx;

    /* evaluate Kepler Equation: 1st derivative */
    const double f1 = 1.0 - ecx + addzero;

    /* delta1: eq. (16) */
    dx = f0 / f1;

    /* evaluate Kepler Equation: scaled 2nd derivative */
    const double f2 = esx / 2.0;

    /* delta2: eq. (17) */
    //dx = f0 / (f1 + f2 * dx);
    dx = f0 / fma(dx, f2, f1);

    /* evaluate Kepler Equation: 3rd derivative scaled */
    const double f3 = ecx / 6.0;

    /* delta3: eq. (18) */
    //dx = f0 / (f1 + f2 * dx + f3 * dx * dx);
    dx = f0 / fma(dx, fma(dx, f3, f2), f1);

    /* evaluate Kepler Equation: scaled 4th derivative */
    const double f4 = -esx / 24.0;

    /* delta4: eq. (19) */
    //dx = f0 / (f1 + f2 * dx + f3 * dx * dx + f4 * dx * dx * dx);
    dx = f0 / fma(dx, fma(dx, fma(dx, f4, f3), f2), f1);

    return( x0 + dx );
} // end kes_itercore5

/******************************************************************************/
