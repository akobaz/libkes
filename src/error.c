/*******************************************************************************
 * @file    error.c
 * @brief   error handling functions for Kepler Equation Solver Library
 * @author  Bazso Akos
 * @version 1.0, 28 Dec 2011
 *          1.1, 23 Feb 2012
 *          1.2, 30 Sep 2013
 *          1.3, 16 Feb 2019
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
/* include standard headers */
#include <stdio.h>

/* include module headers */
#include "error.h"

/******************************************************************************/

/* use verbose debug output? 0 = no (default), 1 = yes */
#define KES_ERROR_DEBUG 0

/******************************************************************************/

/* structure for error message associated to a given error code */
typedef struct {
    kes_err_e         id;
    const char* const text;
} kes_error_list_t;

/* list of error codes and messages
 * NOTE order of items need not be the same as in kes_err_e
 */
static const kes_error_list_t list_of_errors[KES_ERR_TOTAL] = {
    {KES_ERR_NOERR,  "no error occurred ;-)"},
    {KES_ERR_BADECC, "bad value for eccentricity in kes_check_ecc()"},
    {KES_ERR_BADVAL, "bad value for parameter (INF or NaN) in kes_check_val()"},
    {KES_ERR_BADSTM, "bad starter method in kes_starter()"},
    {KES_ERR_BADSOL, "bad solver method in kesolver()"},
    {KES_ERR_BADTOL, "bad value for error tolerance"}
};

/******************************************************************************/

/*******************************************************************************
 * FUNCTION    : kes_show_error
 * DESCRIPTION : print error message related to error "type" to stderr
 * INPUT       : error type from enum kes_err_e
 * OUTPUT      : none
 ******************************************************************************/
inline void kes_show_error(const kes_err_e type)
{
    for (register kes_err_e e = KES_ERR_NOERR; e < KES_ERR_TOTAL; e++)
    {
#if KES_ERROR_DEBUG
    printf("kes_show_error: comparing %d to %d\n", e, type);
#endif
        if ( list_of_errors[e].id == type )
        {
            fprintf(
                stderr,
                "%s: Error %#x = %s\n\n",
                __func__, (int)type, list_of_errors[e].text
            );
            break;
        } // end if
    } // end for
} // end kes_show_error

/******************************************************************************/
