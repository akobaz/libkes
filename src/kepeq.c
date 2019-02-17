/*******************************************************************************
 * MODULE  : kepeq.c
 * PURPOSE : evaluate Kepler Equations for different cases of eccentricity
 * AUTHOR  : Bazso Akos
 * VERSION : 1.0, 16 Feb 2019
 *
 * NOTE derived from corefunc.c (V1.2)
 ******************************************************************************/
/* include standard headers */
#include <math.h>

/* include module header */
#include "kepeq.h"

/******************************************************************************/

/* use verbose debug output? 0 = no (default), 1 = yes */
#define KES_KEPEQ_DEBUG 0

/******************************************************************************/

/*******************************************************************************
 *  FUNCTION    : kes_keq_ell
 *  DESCRIPTION : evaluate elliptic Kepler Equation
 *  INPUT       : - eccentricity ecc (0 <= ecc < 1)
 *                - mean anomaly ma (in radians)
 *                - elliptic eccentric anomaly x (in radians)
 *  OUTPUT      : result of x - ecc*sin(x) - ma
 ******************************************************************************/
inline double kes_keq_ell(
    const double ecc,
    const double ma,
    const double x
    )
{
    return( x - ecc * sin(x) - ma );
} // end kes_keq_ell

/******************************************************************************/

/*******************************************************************************
 *  FUNCTION    : kes_keq_hyp
 *  DESCRIPTION : evaluate hyperbolic Kepler Equation
 *  INPUT       : - eccentricity ecc (ecc > 1)
 *                - mean anomaly ma (in radians)
 *                - hyperbolic eccentric anomaly x (in radians)
 *  OUTPUT      : result of ecc*sinh(x) - x - ma
 ******************************************************************************/
inline double kes_keq_hyp(
    const double ecc,
    const double ma,
    const double x
    )
{
    return( ecc * sinh(x) - x - ma );
} // end kes_keq_hyp

/******************************************************************************/

/*******************************************************************************
 *  FUNCTION    : kes_keq_par
 *  DESCRIPTION : evaluate parabolic Kepler Equation (Barker's Equation)
 *  INPUT       : - eccentricity ecc (ecc = 1), NOTE unused
 *                - mean anomaly ma (in radians)
 *                - true anomaly x (in radians)
 *  OUTPUT      : result of s + s^3 / 3 - ma, with s = tan(x/2)
 ******************************************************************************/
inline double kes_keq_par(
    const double ecc,
    const double ma,
    const double x
    )
{
    /* TODO FIXME check case x = pi */
    const double s = tan( 0.5 * x );
    return( s + s*s*s / 3.0 - ma );
} // end kes_keq_par

/******************************************************************************/
