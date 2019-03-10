/***************************************************************************//**
 * @file    error.h
 * @brief   error handling functions for Kepler Equation Solver Library
 * @author  Bazso Akos
 * @version 2019.02
 *
 * @copyright
 * Copyright (C) 2011-2019 Bazso Akos
 *
 * This file is part of libkes.
 *
 * libkes is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * libkes is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with libkes.  If not, see <https://www.gnu.org/licenses/>.
 *
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
