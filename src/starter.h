/***************************************************************************//**
 * @file    starter.h
 * @brief   starting value functions for Kepler Equation Solver Library
 * @author  Bazso Akos
 * @version 2019.02
 ******************************************************************************/
#pragma once
#ifndef KES_STARTER__H
#define KES_STARTER__H

/* include module headers */
#include "error.h"

/******************************************************************************/

/*** declare data structures ***/

/*!
 * @brief enumeration of starter methods
 * @details reference list of starter methods:
 * Odell & Gooding (1986), Celestial Mechanics, Volume 38, Issue 4, p.307-334
 * [ADS link](http://adsabs.harvard.edu/abs/1986CeMec..38..307O)
 */
typedef enum
{
    KES_STM_NONE = 0, ///< unidentified (void) starting value method

    /* different starter methods for elliptic case */
    KES_STM_ES00,     ///< starting value "pi"
    KES_STM_ES01,     ///< starter S1 from Odell & Gooding (1986)
    KES_STM_ES02,     ///< starter S2 from Odell & Gooding (1986)
    KES_STM_ES03,     ///< starter S3 from Odell & Gooding (1986)
    KES_STM_ES04,     ///< starter S4 from Odell & Gooding (1986)
    KES_STM_ES05,     ///< starter S5 from Odell & Gooding (1986)
    KES_STM_ES06,     ///< starter S6 from Odell & Gooding (1986)
    KES_STM_ES07,     ///< starter S7 from Odell & Gooding (1986)
    KES_STM_ES08,     ///< starter S8 from Odell & Gooding (1986)
    KES_STM_ES09,     ///< starter S9 from Odell & Gooding (1986)
    KES_STM_ES10,     ///< starter S10 from Odell & Gooding (1986)
    KES_STM_ES11,     ///< starter S11 from Odell & Gooding (1986)
    KES_STM_ES12,     ///< starter S12 from Odell & Gooding (1986)
    KES_STM_ES13,     ///< starter from Encke (1850)
    KES_STM_ES14,     ///< starter from Charles & Tatum (1998)

    /* TODO FIXME different starter methods for hyperbolic case */
    //KES_STM_HS00,

    /* TODO FIXME different starter methods for parabolic case */
    //KES_STM_PS00,

    KES_STM_TOTAL     ///< total number of available starting value methods
} kes_stm_e;

/******************************************************************************/

/*** function declarations ***/

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * @brief starting value for iteration methods
 * @param[in] ecc eccentricity
 * @param[in] ma mean anomaly (in radians)
 * @param[in] type choose a starter method from enum #kes_stm_e
 * @param[out] status pointer to return error code from enum #kes_err_e
 * @return starting value x0
 */
double kes_starter(
    const double    ecc,
    const double    ma,
    const kes_stm_e type,
    kes_err_e*      status
);

#ifdef __cplusplus
}
#endif

/******************************************************************************/

#endif  /* KES_STARTER__H */
