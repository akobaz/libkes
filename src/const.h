/***************************************************************************//**
 * @file    const.h
 * @brief   define additional constants for strict C99 and MSVC mode
 * @details numbers from Wolfram Alpha with extended precision;
 *          NOTE using math library prefix "M_" for constants
 *          instead of prefix "KES_"
 * @author  Bazso Akos
 * @version 2019.02
 ******************************************************************************/
#pragma once
#ifndef KES_CONST__H
#define KES_CONST__H

/* include standard headers */
#include <math.h>

/******************************************************************************/

/* use verbose debug output? 0 = no (default), 1 = yes */
#define KES_CONST_DEBUG 0

/******************************************************************************/

/*!
 * @brief value of pi
 * \f$ \pi \f$
 */
#ifndef M_PI
  #if KES_CONST_DEBUG
    #warning "redefinition of M_PI to internal value"
  #endif

  #define M_PI     3.14159265358979323846264338327950288419716939937510582
#endif

/******************************************************************************/

/*!
 * @brief value of 1/pi
 * \f$ \frac{1}{\pi} \f$
 */
#ifndef M_1_PI
  #if KES_CONST_DEBUG
    #warning "redefinition of M_1_PI to internal value"
  #endif

  #define M_1_PI   0.318309886183790671537767526745028724068919291480912897
//#define M_1_PI   (1.0 / M_PI)
#endif

/******************************************************************************/

/*!
 * @brief value of 2*pi
 * \f$ 2 \pi \f$
 */
#ifndef M_2PI
  #if KES_CONST_DEBUG
    #warning "redefinition of M_2PI to internal value"
  #endif

  #define M_2PI    6.28318530717958647692528676655900576839433879875021164
//#define M_2PI    (2.0 * M_PI)
#endif

/******************************************************************************/

/*!
 * @brief value of pi/2
 * \f$ \frac{\pi}{2} \f$
 */
#ifndef M_PI_2
  #if KES_CONST_DEBUG
    #warning "redefinition of M_PI_2 to internal value"
  #endif

  #define M_PI_2   1.57079632679489661923132169163975144209858469968755291
//#define M_PI_2   (0.5 * M_PI)
#endif

/******************************************************************************/

/*!
 * @brief value of pi^2
 * \f$ pi^{2} \f$
 */
#ifndef M_PISQ
  #if KES_CONST_DEBUG
    #warning "redefinition of M_PISQ to internal value"
  #endif

  #define M_PISQ   9.86960440108935861883449099987615113531369940724079063
//#define M_PISQ   (M_PI * M_PI)
#endif

/******************************************************************************/

#endif  /* KES_CONST__H */
