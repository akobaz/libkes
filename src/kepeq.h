/***************************************************************************//**
 * @file    kepeq.h
 * @brief   evaluate Kepler Equations for different cases of eccentricity
 * @author  Bazso Akos
 * @version 2019.02
 ******************************************************************************/
#pragma once
#ifndef KES_KEPEQ__H
#define KES_KEPEQ__H

/******************************************************************************/

/*** function declarations ***/

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * @brief evaluate elliptic Kepler Equation
 * @param[in] ecc eccentricity
 * @param[in] ma mean anomaly (in radians)
 * @param[in] x elliptic eccentric anomaly (in radians)
 * @return result of expression \f$ x - e \sin x - M \f$
 */
double kes_keq_ell(
    const double ecc,
    const double ma,
    const double x
);


/*!
 * @brief evaluate hyperbolic Kepler Equation
 * @param[in] ecc eccentricity
 * @param[in] ma mean anomaly (in radians)
 * @param[in] x hyperbolic eccentric anomaly (in radians)
 * @return result of expression \f$ e \sinh x - x - M \f$
 */
double kes_keq_hyp(
    const double ecc,
    const double ma,
    const double x
);


/*!
 * @brief evaluate parabolic Kepler Equation (Barker's Equation)
 * @param[in] ecc eccentricity (NOTE unused)
 * @param[in] ma mean anomaly (in radians)
 * @param[in] x true anomaly (in radians)
 * @return result of expression \f$ s + (1/3) s^3 - M \f$ with s = tan(x/2)
 */
double kes_keq_par(
    const double ecc,
    const double ma,
    const double x
);

#ifdef __cplusplus
}
#endif

/******************************************************************************/

# endif  /* KES_KEPEQ__H */
