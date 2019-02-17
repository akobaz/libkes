/*******************************************************************************
 * MODULE  : utils.c
 * PURPOSE : utility functions for Kepler Equation Solver Library
 * AUTHOR  : Bazso Akos
 * VERSION : 1.0, 16 Feb 2019
 *
 * NOTE derived from corefunc.c (V1.2)
 ******************************************************************************/
/* include module headers */
#include "utils.h"
#include "const.h"

/******************************************************************************/

/* use verbose debug output? 0 = no (default), 1 = yes */
#define KES_UTILS_DEBUG 0

/******************************************************************************/

/*******************************************************************************
 *  FUNCTION    : kes_check_ecc
 *  DESCRIPTION : check input to be a valid eccentricity value
 *  INPUT       : - eccentricity "ecc"
 *                - pointer for error "status" code (see error.h)
 *  OUTPUT      : code from kes_ecc_e
 ******************************************************************************/
inline kes_ecc_e kes_check_ecc(
    const double ecc,
    kes_err_e*   status
    )
{
    /* minimum threshold for eccentricity
     * to discriminate between domains
     */
    static const double ecc_min = 1e-10;

    /* default: invalid case */
    kes_ecc_e ret = KES_ECC_NEG;

    /* check for valid floating point number */
    if ( kes_check_val(ecc) == KES_ERR_NOERR )
    {
        /* elliptic, hyperbolic, or parabolic cases */
        if ( ecc > ecc_min )
        {
            if ( ecc < (1.0 - ecc_min) )
            {
                /* elliptic case */
                ret = KES_ECC_ELL;
            } // end if
            else if ( ecc > (1.0 + ecc_min) )
            {
                /* hyperbolic case */
                ret = KES_ECC_HYP;
            } // end if
            else
            {
                /* parabolic case */
                ret = KES_ECC_PAR;
            } // end else
        } // end if
        else if ( ecc < 0.0 )
        {
            /* negative case: invalid eccentricity */
            ret = KES_ECC_NEG;
        } // end if
        else
        {
            /* circular case */
            ret = KES_ECC_CIRC;
        } // end else
    } // end if

    /* signal error code */
    if ( ret != KES_ECC_NEG )
    {
        /* no error, valid eccentricity */
        *status = KES_ERR_NOERR;
    } // end if
    else
    {
        /* error, invalid eccentricity */
        *status = KES_ERR_BADECC;
    } // end else

    return( ret );
} // end kes_check_ecc

/******************************************************************************/

/*******************************************************************************
 *  FUNCTION    : kes_check_val
 *  DESCRIPTION : check input to be a valid number (i.e. not NAN or INF)
 *  INPUT       : real value
 *  OUTPUT      : code from kes_err_e {KES_ERR_NOERR, KES_ERR_BADVAL}
 ******************************************************************************/
inline kes_err_e kes_check_val(const double x)
{
    /* check for valid input (x != INF AND x != NAN) */
    if ( isfinite(x) )
    {
        return( KES_ERR_NOERR );
    } // end if
    else
    {
        return( KES_ERR_BADVAL );
    } // end else
} // end kes_check_val

/******************************************************************************/

/*******************************************************************************
 *  FUNCTION    : kes_reduce
 *  DESCRIPTION : reduce mean anomaly to interval [-pi:pi)
 *  INPUT       : angle x (in radians)
 *  OUTPUT      : angle x modulo 2*pi (in radians)
 ******************************************************************************/
inline double kes_reduce(double x)
{
    if ( kes_check_val(x) == KES_ERR_NOERR )
    {
        /* x modulo (2*pi) */
        x -= floor(x / M_2PI) * M_2PI;

        // if x >  pi : x - 2*pi
        x = ((x >  M_PI) ? x - M_2PI : x);

        // if x < -pi : x + 2*pi
        x = ((x < -M_PI) ? x + M_2PI : x);

        /*** TODO FIXME after rebound/tools.c
         * while (x < -M_PI) x += M_2PI;
         * while (x >  M_PI) x -= M_2PI;
         ***/
    } // end if

    return( x );
} // end kes_reduce

/******************************************************************************/

/*******************************************************************************
 *  FUNCTION    : kes_sincos
 *  DESCRIPTION : calculate the values of functions sin(x) and cos(x) by
 *                a single call to tan(x/2),
 *  NOTE          if "ecc" <  0, then sin(x), cos(x) are returned,
 *                if "ecc" >= 0, then ecc*sin(x), ecc*cos(x) are returned
 *  INPUT       : - pointer to sin(x), cos(x)
 *                - angle x (in radians)
 *                - eccentricity ecc
 *  OUTPUT      : none
 *  REFERENCE   : Press et al., Numerical Recipes
 ******************************************************************************/
inline void kes_sincos(
    double*      sx,
    double*      cx,
    const double x,
    const double ecc
    )
{
    const double tx = tan(0.5 * x);          // tan(x/2)
    const double cd = 1.0 / (1.0 + tx * tx); // common denominator

    *cx = (1.0 - tx * tx) * cd;              // cos(x)
    *sx = 2.0 * tx * cd;                     // sin(x)

    if ( ecc >= 0.0 )
    {
        *cx *= ecc;                          // ecc*cos(x)
        *sx *= ecc;                          // ecc*sin(x)
    } // end if

    return;
} // end kes_sincos

/******************************************************************************/

/*******************************************************************************
 *  FUNCTION    : kes_trueanom
 *  DESCRIPTION : calculate the true anomaly for elliptic or hyperbolic case
 *  INPUT       : - eccentricity ecc
 *                - angle x (in radians) ... solution of Kepler's equation for
 *                  elliptic or hyperbolic eccentric anomaly
 *  OUTPUT      : true anomaly (in radians)
 ******************************************************************************/
inline double kes_trueanom(
    const double ecc,
    const double x
    )
{
    /* TODO FIXME kes_check_ecc() for KES_ECC_CIRC or KES_ECC_PAR */

    if ( ecc < 1.0 )
    {
        /* reference: Stumpff (1958), eq. (II; 14) */
        return( 2.0 * atan(sqrt((1.0 + ecc) / (1.0 - ecc)) * tan(0.5 * x)) );
    } // end if
    else
    {
        /* reference: Stumpff (1958), eq. (III; 50) */
        return( 2.0 * atan(sqrt((ecc + 1.0) / (ecc - 1.0)) * tanh(0.5 * x)) );
    } // end  else
} // end kes_trueanom

/******************************************************************************/
