/*******************************************************************************
 * MODULE  : starter.c
 * PURPOSE : starting value functions for Kepler Equation Solver
 * AUTHOR  : Bazso Akos
 * VERSION : 1.0, 28 Jul 2011
 *           1.1, 21 Aug 2012
 *           1.2, 30 Sep 2013
 *           1.3, 16 Feb 2019
 *
 * NOTE based on iterfunc.c (V1.2)
 *
 * NOTE currently only implemented starters for elliptic case
 *
 * NOTE abbreviation for references:
 * OG86 = Odell & Gooding (1986), Celestial Mechanics 38, pages 307-334
 ******************************************************************************/
/* include module headers */
#include "starter.h"
#include "const.h"
#include "utils.h"

/******************************************************************************/

/* show additional debugging information ? 0 = no (default), 1 = yes */
#define KES_STARTER_DEBUG 0

#if KES_STARTER_DEBUG
  #include "debug.h"
#endif

/******************************************************************************/

/*******************************************************************************
 *  STARTER   : KES_STM_ES00
 *  ORDER     : O(e^0)
 *  FORMULA   : E0 = pi
 *  REFERENCE : TODO
 ******************************************************************************/
static inline double stm_s0(
    const double ecc,
    const double ma
    )
{
    return( M_PI );
} // end stm_s0

/******************************************************************************/

/*******************************************************************************
 *  STARTER   : KES_STM_ES01
 *  ORDER     : O(e^1)
 *  FORMULA   : E0 = M
 *  REFERENCE : OG86
 ******************************************************************************/
static inline double stm_s1(
    const double ecc,
    const double ma
    )
{
    return( ma );
} // end stm_s1

/******************************************************************************/

/*******************************************************************************
 *  STARTER   : KES_STM_ES02
 *  ORDER     : O(e^2)
 *  FORMULA   : E0 = M + e * sin(M)
 *  REFERENCE : OG86
 ******************************************************************************/
static inline double stm_s2(
    const double ecc,
    const double ma
    )
{
    return( ma + ecc * sin( ma ) );
} // end stm_s2

/******************************************************************************/

/*******************************************************************************
 *  STARTER   : KES_STM_ES03
 *  ORDER     : O(e^3)
 *  FORMULA   : E0 = M + e * sin(M) * (1 + e * cos(M))
 *  REFERENCE : OG86
 ******************************************************************************/
static inline double stm_s3(
    const double ecc,
    const double ma
    )
{
    double ecosx, esinx;

    /* calculate sin, cos */
    kes_sincos(&esinx, &ecosx, ma, ecc);

    return( ma + esinx * (1.0 + ecosx) );
} // end stm_s3

/******************************************************************************/

/*******************************************************************************
 *  STARTER   : KES_STM_ES04
 *  ORDER     : O(e^1)
 *  FORMULA   : E0 = M + e
 *  REFERENCE : OG86
 ******************************************************************************/
static inline double stm_s4(
    const double ecc,
    const double ma
    )
{
    return( ma + ecc );
} // end stm_s4

/******************************************************************************/

/*******************************************************************************
 *  STARTER   : KES_STM_ES05
 *  ORDER     : O(e^3)
 *  FORMULA   : E0 = M + e * sin(M) / (1 - sin(M + e) + sin(M))
 *  REFERENCE : Smith (1979), Celestial Mechanics 19, p.163-166
 ******************************************************************************/
static inline double stm_s5(
    const double ecc,
    const double ma
    )
{
    const double sinx = sin(ma);

    return( ma + ecc * sinx / (1.0 - sin(ma + ecc) + sinx) );
} // end stm_s5

/******************************************************************************/

/*******************************************************************************
 *  STARTER   : KES_STM_ES06
 *  ORDER     : O(e^1)
 *  FORMULA   : E0 = M + e * (pi - M) / (1 + e) = (M + e * pi) / (1 + e)
 *  REFERENCE : OG86
 ******************************************************************************/
static inline double stm_s6(
    const double ecc,
    const double ma
    )
{
    return( (ma + ecc * M_PI) / (1.0 + ecc) );
} // end stm_s6

/******************************************************************************/

/*******************************************************************************
 *  STARTER   : KES_STM_ES07
 *  ORDER     : O(e^1)
 *  FORMULA   : E0 = min{M/(1-e), S4, S6}
 *  REFERENCE : OG86
 ******************************************************************************/
static inline double stm_s7(
    const double ecc,
    const double ma
    )
{
    const double tmp = ma / (1.0 - ecc); /* = M / (1 - e)            */
    const double s4  = stm_s4(ecc, ma);  /* = M + e                  */
    const double s6  = stm_s6(ecc, ma);  /* = (M + e * pi) / (1 + e) */

    /* choose minimal starter */
    double min = fmin(tmp, s4);
    //tmp = ((tmp < s4) ? tmp : s4);
    min = fmin(min, s6);
    //tmp = ((tmp < s6) ? tmp : s6);

    return( min );
} // end stm_s7

/******************************************************************************/

/*******************************************************************************
 *  STARTER   : KES_STM_ES08
 *  ORDER     : O(e^3)
 *  FORMULA   : E0 = S03 + lambda * e^4 * (pi - S03),
 *              lambda = 1 / (20 * pi)
 *  REFERENCE : OG86
 ******************************************************************************/
static inline double stm_s8(
    const double ecc,
    const double ma
    )
{
    static const double lambda = (0.05 * M_1_PI); // = 1/(20*pi)
    const double x = stm_s3(ecc, ma);

    return( x + lambda * ecc * ecc * ecc * ecc * (M_PI - x) );
} // end stm_s8

/******************************************************************************/

/*******************************************************************************
 *  STARTER   : KES_STM_ES09
 *  ORDER     : O(e^4)
 *  FORMULA   : E0 = M + e * sin(M) / (1 - 2 * e * cos(M) + e^2)^(1/2)
 *  REFERENCE : OG86
 ******************************************************************************/
static inline double stm_s9(
    const double ecc,
    const double ma
    )
{
    double ecosx, esinx;

    /* avoid singularity at (e,M) = (1,0) */
    if ( (ecc < 1.0) && (ma > 0.0) )
    {
        /* calculate sin, cos */
        kes_sincos(&esinx, &ecosx, ma, ecc);

        return( ma + esinx / sqrt(1.0 - 2.0 * ecosx + ecc * ecc) );
    } // end if
    else return( ma );
} // end stm_s9

/******************************************************************************/

/*******************************************************************************
 *  STARTER   : KES_STM_ES10
 *  ORDER     : O(e^0)
 *  FORMULA   : E0 = s - q / s,
 *              q = 2 * (1 - e) / e,
 *              r = 3 * M / e,
 *              s = cbrt(sqrt(r^2 + q^3) + r),
 *  REFERENCE : Ng (1979), Celestial Mechanics 20, p.243-249
 ******************************************************************************/
static inline double stm_s10(
    const double ecc,
    const double ma
    )
{
    /* avoid division by e=0 */
    if ( ecc > 0.0 )
    {
        const double q = 2.0 * (1.0 - ecc) / ecc;
        const double r = 3.0 * ma / ecc;
        const double s = cbrt(sqrt(q*q*q + r*r) + r);

        return( s - q / s );
    } // end if
    else return( ma );
} // end stm_s10

/******************************************************************************/

/*******************************************************************************
 *  STARTER   : KES_STM_ES11
 *  ORDER     : O(e^4)
 *  FORMULA   : E0 = M + e * sin(M) *
 *                   [ 1 +
 *                     e * 2 * cos(M) / 3 +
 *                     e^2 * (1 - 48 * cos(M) + 19 * cos(2 * M)) / 36 +
 *                     e^3 * (a + b * cos(M) + c * cos(2 * M))
 *                   ] / {1 - [1 + e * e1 * (1 + e1)^2] * e * cos(M)}^1/3,
 *              e1 = 1 - e, (a,b,c) = -(3^(1/3)-8/9)/6*(1,-9,2)
 *  REFERENCE : OG86
 ******************************************************************************/
static inline double stm_s11(
    const double ecc,
    const double ma
    )
{
    static const double a  = -0.922267802364199155721E-1;
    static const double b  =  0.830041022127779240149E+0; /* b = -9*a; */
    static const double c  = -0.184453560472839831144E+0; /* c =  2*a; */

    /* avoid singularity at e=1 */
    if ( ecc < 1.0 )
    {
        double cosx, sinx;

        /* evaluate only sin(x), cos(x): use neg. ecc. */
        kes_sincos(&sinx, &cosx, ma, -1.0);

        const double e1    = 1.0 - ecc;
        const double cos2x = (2.0 * cosx * cosx - 1.0);
        const double ecosx = ecc * cosx;
        const double esinx = ecc * sinx;

        return(
            ma + esinx * (\
                1.0 +\
                ecosx * 2.0 / 3.0 +\
                ecc * ecc * (1.0 - 48.0 * cosx + 19.0 * cos2x) / 36.0 +\
                ecc * ecc * ecc * (a + b * cosx + c * cos2x)
            ) / cbrt(1.0 - (1.0 + ecc * e1 * (1.0 + e1) * (1.0 + e1)) * ecosx)
        );
    } // end if
    else return( ma );
} // end stm_s11

/******************************************************************************/

/*******************************************************************************
 *  STARTER   : KES_STM_ES12
 *  ORDER     : O(e^1)
 *  FORMULA   : E0 = e * E(0,e=1) + (1 - e) * M
 *  REFERENCE : OG86
 ******************************************************************************/
static inline double stm_s12(
    const double ecc,
    const double ma
    )
{
    static const double a = (M_PI-1.0)*(M_PI-1.0)/(M_PI+2.0/3.0);
    static const double b = 2.0*(M_PI-1.0/6.0)*(M_PI-1.0/6.0)/(M_PI+2.0/3.0);
    const double        w = M_PI - ma;

    return( ecc * (M_PI - a * w / (b - w)) + (1.0 - ecc) * ma );
} // end stm_s12

/******************************************************************************/

/*******************************************************************************
 *  STARTER   : KES_STM_ES13
 *  ORDER     : O(e^6)
 *  FORMULA   : E0 = arctan( sin(y) / (cos(y) - e) )
 *              y = M + sin(x) - x
 *              x = arctan( e*sin(M) / (1 - e*cos(M)) )
 *  REFERENCE : Encke (1850), Astron. Nachr. 30, p.277-292
 *  see also  : Neutsch & Scherer (1992), Celestial Mechanics, BI-Wiss.-Verl.
 ******************************************************************************/
static inline double stm_s13(
    const double ecc,
    const double ma
    )
{
    double x, esinx, ecosx;

    kes_sincos(&esinx, &ecosx, ma, ecc); /* e*sin(M), e*cos(M) */
    x = atan2(esinx, 1.0 - ecosx);       /* x = arctan(e*sin(M)/(1-cos(M))) */
    x = ma + sin(x) - x;                 /* y = M + e*sin(x) - x */
    kes_sincos(&esinx, &ecosx, x, -1.0); /* sin(y), cos(y) */

    return( atan2(esinx, ecosx - ecc) );
} // end stm_s13

/******************************************************************************/

/*******************************************************************************
 *  STARTER   : KES_STM_ES14
 *  ORDER     : O(e^1)
 *  FORMULA   : E0 = M + e * [ (pi^2*M)^(1/3) - pi*sin(M)/15 - M ]
 *  REFERENCE : Charles & Tatum (1998), Cel. Mech. Dyn. Astron. 69, p.357-372
 ******************************************************************************/
static inline double stm_s14(
    const double ecc,
    const double ma
    )
{
    return( ma + ecc * (cbrt(M_PISQ * ma) - M_PI * sin(ma) / 15.0 - ma) );
} // end stm_s14

/******************************************************************************/

/*******************************************************************************
 *  FUNCTION    : kes_starter
 *  DESCRIPTION : evaluate chosen starter method and return starting value for
 *                use in iteration methods
 *  INPUT       : - eccentricity "ecc"
 *                - mean anomaly "ma" in radians
 *                - starter method "type" from list kes_stm_e
 *                - pointer "status" to return error code (see error.h)
 *  OUTPUT      : starting value
 ******************************************************************************/
double kes_starter(
    const double    ecc,
    const double    ma,
    const kes_stm_e type,
    kes_err_e*      status
    )
{
    /* default return value */
    double ret = 0.0;
    /* default error code: no error */
    *status    = KES_ERR_NOERR;

    /* choose which starting method to use */
    switch ( type )
    {
        /*** BEGIN starters O(e^0) ***/

        /* Starter S00 */
        case KES_STM_ES00:
        {
            ret = stm_s0(ecc, ma);
#if KES_STARTER_DEBUG
    KES_DEBUG_STM("KES_STM_ES00", ecc, ma, ret);
#endif
            break;
        } // end case

        /* Starter S10 */
        case KES_STM_ES10:
        {
            ret = stm_s10(ecc, ma);
#if KES_STARTER_DEBUG
    KES_DEBUG_STM("KES_STM_ES10", ecc, ma, ret);
#endif
            break;
        } // end case

        /*** END starters O(e^0) ***/


        /*** BEGIN starters O(e^1) ***/

        /* Starter S01 */
        case KES_STM_ES01:
        {
            ret = stm_s1(ecc, ma);
#if KES_STARTER_DEBUG
    KES_DEBUG_STM("KES_STM_ES01", ecc, ma, ret);
#endif
            break;
        } // end case

        /* Starter S04 */
        case KES_STM_ES04:
        {
            ret = stm_s4(ecc, ma);
#if KES_STARTER_DEBUG
    KES_DEBUG_STM("KES_STM_ES04", ecc, ma, ret);
#endif
            break;
        } // end case

        /* Starter S06 */
        case KES_STM_ES06:
        {
            ret = stm_s6(ecc, ma);
#if KES_STARTER_DEBUG
    KES_DEBUG_STM("KES_STM_ES06", ecc, ma, ret);
#endif
            break;
        } // end case

        /* Starter S07 */
        case KES_STM_ES07:
        {
            ret = stm_s7(ecc, ma);
#if KES_STARTER_DEBUG
    KES_DEBUG_STM("KES_STM_ES07", ecc, ma, ret);
#endif
            break;
        } // end case

        /* Starter S12 */
        case KES_STM_ES12:
        {
            ret = stm_s12(ecc, ma);
#if KES_STARTER_DEBUG
    KES_DEBUG_STM("KES_STM_ES12", ecc, ma, ret);
#endif
            break;
        } // end case

        /* Starter S14 */
        case KES_STM_ES14:
        {
            ret = stm_s14(ecc, ma);
#if KES_STARTER_DEBUG
    KES_DEBUG_STM("KES_STM_ES14", ecc, ma, ret);
#endif
            break;
        } // end case

        /*** END starters O(e^1) ***/


        /*** BEGIN starters O(e^2) ***/

        /* Starter S02 */
        case KES_STM_ES02:
        {
            ret = stm_s2(ecc, ma);
#if KES_STARTER_DEBUG
    KES_DEBUG_STM("KES_STM_ES02", ecc, ma, ret);
#endif
            break;
        } // end case

        /*** END starters O(e^2) ***/


        /*** BEGIN starters O(e^3) **/

        /* Starter S03 */
        case KES_STM_ES03:
        {
            ret = stm_s3(ecc, ma);
#if KES_STARTER_DEBUG
    KES_DEBUG_STM("KES_STM_ES03", ecc, ma, ret);
#endif
            break;
        } // end case

        /* Starter S05 */
        case KES_STM_ES05:
        {
            ret = stm_s5(ecc, ma);
#if KES_STARTER_DEBUG
    KES_DEBUG_STM("KES_STM_ES05", ecc, ma, ret);
#endif
            break;
        } // end case

        /* Starter S08 */
        case KES_STM_ES08:
        {
            ret = stm_s8(ecc, ma);
#if KES_STARTER_DEBUG
    KES_DEBUG_STM("KES_STM_ES08", ecc, ma, ret);
#endif
            break;
        } // end case

        /*** END starters O(e^3) ***/


        /*** BEGIN starters O(e^4) ***/

        /* Starter S09 */
        case KES_STM_ES09:
        {
            ret = stm_s9(ecc, ma);
#if KES_STARTER_DEBUG
    KES_DEBUG_STM("KES_STM_ES09", ecc, ma, ret);
#endif
            break;
        } // end case

        /* Starter S11 */
        case KES_STM_ES11:
        {
            ret = stm_s11(ecc, ma);
#if KES_STARTER_DEBUG
    KES_DEBUG_STM("KES_STM_ES11", ecc, ma, ret);
#endif
            break;
        } // end case

        /*** END starters O(e^4) ***/


        /*** BEGIN starters O(e^6) ***/

        /* Starter S13 */
        case KES_STM_ES13:
        {
            ret = stm_s13(ecc, ma);
#if KES_STARTER_DEBUG
    KES_DEBUG_STM("KES_STM_ES13", ecc, ma, ret);
#endif
            break;
        } // end case

        /*** END starters O(e^6) ***/


        /* fallback case */
        default:
        {
            ret     = 0.0;
            *status = KES_ERR_BADSTM;
            break;
        } // end case
    } // end switch

    return( ret );
} // end kes_starter

/******************************************************************************/
