/***************************************************************************//**
 * @file    fixedp.h
 * @brief   fixed-point iteration method
 * @details reference(s)
 * @author  Bazso Akos
 ******************************************************************************/
#pragma once
#ifndef KES_SOL_FIXEDP__H
#define KES_SOL_FIXEDP__H

/* include module headers */
#include "solver.h"

/******************************************************************************/

/*** function declarations ***/

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * @brief iteration function for fixed-point iteration
 * @param[in] ecc eccentricity
 * @param[in] ma mean anomaly (in radians)
 * @param[in] starter starting value for first iteration (ignored)
 * @param[out] res iteration results and statistics
 * @return number of iterations
 */
int fixedp(
    const double ecc,
    const double ma,
    const double starter,
    kes_input_t* res
);

#ifdef __cplusplus
}
#endif

/******************************************************************************/

#endif  /* KES_SOL_FIXEDP__H */
