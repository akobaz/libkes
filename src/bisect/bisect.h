/***************************************************************************//**
 * @file    bisect.h
 * @brief   bisection method (interval halving)
 * @details reference(s)
 * @author  Bazso Akos
 ******************************************************************************/
#pragma once
#ifndef KES_SOL_BISECT__H
#define KES_SOL_BISECT__H

/* include module headers */
#include "solver.h"

/******************************************************************************/

/*** function declarations ***/

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * @brief iteration function for bisection method (interval halving method)
 * @param[in] ecc eccentricity
 * @param[in] ma mean anomaly (in radians)
 * @param[in] starter starting value for first iteration (ignored)
 * @param[out] res iteration results and statistics
 * @return number of iterations
 */
int bisect(
    const double ecc,
    const double ma,
    const double starter,
    kes_input_t* res
);

#ifdef __cplusplus
}
#endif

/******************************************************************************/

#endif  /* KES_SOL_BISECT__H */
