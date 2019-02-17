/***************************************************************************//**
 * @file    error.h
 * @brief   error handling functions for Kepler Equation Solver Library
 * @author  Bazso Akos
 * @version 2019.02
 ******************************************************************************/
#pragma once
#ifndef KES_ERROR__H
#define KES_ERROR__H

/******************************************************************************/

/*** declare data structures ***/

/*!
 * @brief enumeration of error codes
 * @details list of error codes returned by some functions;
 * the user should check the function's return value,
 * and display the error message with kes_show_error() if necessary
 */
typedef enum
{
    KES_ERR_NOERR = 0, ///< no error, successful return

    /* different error codes */
    KES_ERR_BADECC,    ///< bad value for eccentricity in kes_check_ecc()
    KES_ERR_BADVAL,    ///< bad value for parameter (INF or NAN) in kes_check_val()
    KES_ERR_BADSTM,    ///< bad starter method in kes_starter()
    KES_ERR_BADSOL,    ///< bad solver method in kesolver()
    KES_ERR_BADTOL,    ///< bad value for error tolerance

    KES_ERR_TOTAL      ///< total number of error entries
} kes_err_e;

/******************************************************************************/

/*** function declarations ***/

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * @brief print error message to \a stderr
 * @param[in] type error code from enum #kes_err_e
 * @return none
 */
void kes_show_error(const kes_err_e type);

#ifdef __cplusplus
}
#endif

/******************************************************************************/

#endif  /* KES_ERROR__H */
