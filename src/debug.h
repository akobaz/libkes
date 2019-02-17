/***************************************************************************//**
 * @file    debug.h
 * @brief   define macros for debugging
 * @author  Bazso Akos
 * @version 2019.02
 ******************************************************************************/
#pragma once
#ifndef KES_DEBUG__H
#define KES_DEBUG__H

/* include standard headers */
#include <stdio.h>

/******************************************************************************/

/*!
 * @brief enable monitoring of function evaluations?
 * @details 0 = no (default), 1 = yes;
 * if enabled, #kes_input_t provides counters for the number of
 * (1) trigonometric function (sin, cos) evaluations,
 * (2) Kepler Equation function evaluations
 */
#define KES_SHOW_ITER_STATS 0

/******************************************************************************/

/*!
 * @brief enable verbose debug output during iteration process?
 * @details 0 = no (default), 1 = yes;
 * if enabled, solver functions trace the progress of iterations to stdout
 */
#define KES_ITER_DEBUG 1

/******************************************************************************/

/*!
 * @brief debug a starter method
 * @details show values of input parameters
 */
#define KES_DEBUG_STM(NAME,ARG1,ARG2,ARG3)                                     \
        printf("%s: Input ... e = %lf, M = %lf, E0 = %lf\n",                   \
               NAME, (ARG1), (ARG2), (ARG3) )

/******************************************************************************/

/*!
 * @brief debug a solver method
 * @details show remaining error values during iteration process
 */
#define KES_DEBUG_SOL(NAME,ARG1,ARG2,ARG3)                                     \
        printf("%s: Errors ... iter = %3d, Deltax = %e, Deltaf = %e\n",        \
               NAME, (ARG1), (ARG2), (ARG3) )

/******************************************************************************/

#endif  /* KES_DEBUG__H */
