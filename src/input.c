/*******************************************************************************
 * @file    input.c
 * @brief   data structure and utility functions for input data
 * @author  Bazso Akos
 * @version 1.0, 22 Sep 2019
 *
 * @copyright
 * Copyright (C) 2019 Bazso Akos
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
#include <stddef.h>

/* include module headers */
#include "input.h"
#include "utils.h"

/******************************************************************************/

/* use verbose debug output? 0 = no (default), 1 = yes */
#define KES_INPUT_DEBUG 0

/******************************************************************************/

/*** internal constants ***/

/* default error tolerance for solver functions */
static const double std_tol     = KES_STD_TOL;

/* default upper limit for number of iterations */
static const int    std_maxiter = KES_STD_MAXITER;

/******************************************************************************/

/*******************************************************************************
 * FUNCTION    : kes_check_input
 * DESCRIPTION : check input parameters and set default values if needed
 * INPUT       : structure of type kes_input_t as input
 * OUTPUT      : status code:
 *               - 0  = success
 *               - >0 = warning in case of any replacements
 ******************************************************************************/
inline int kes_check_input(kes_input_t* data)
{
    /* return status code */
    int ret = 0;

    /* override useless settings */
    if ( data->tolf < std_tol )
    {
        data->tolf    = std_tol;
        ++ret;
    } // end if

    if ( data->tolx < std_tol )
    {
        data->tolx    = std_tol;
        ++ret;
    } // end if

    if ( data->maxiter == 0 )
    {
        data->maxiter = std_maxiter;
        ++ret;
    } // end if

    return ret;
} // end kes_check_input

/******************************************************************************/

/*******************************************************************************
 * FUNCTION    : kes_get_tolf
 * DESCRIPTION : return current value of parameter "tolf" in data structure
 * INPUT       : structure of type kes_input_t as input
 * OUTPUT      : value tolf
 ******************************************************************************/
inline double kes_get_tolf(const kes_input_t* const in)
{
    if ( in != NULL )
    {
        return( in->tolf );
    } // end if
    else
    {
        return 0.0;
    } // end else
} // end kes_get_tolf

/******************************************************************************/

/*******************************************************************************
 * FUNCTION    : kes_get_tolx
 * DESCRIPTION : return current value of parameter "tolx" in data structure
 * INPUT       : structure of type kes_input_t as input
 * OUTPUT      : value tolx
 ******************************************************************************/
inline double kes_get_tolx(const kes_input_t* const in)
{
    if ( in != NULL )
    {
        return( in->tolx );
    } // end if
    else
    {
        return 0.0;
    } // end else
} // end kes_get_tolx

/******************************************************************************/

/*******************************************************************************
 * FUNCTION    : kes_get_maxiter
 * DESCRIPTION : return current value of parameter "maxiter" in data structure
 * INPUT       : structure of type kes_input_t as input
 * OUTPUT      : value maxiter
 ******************************************************************************/
inline int kes_get_maxiter(const kes_input_t* const in)
{
    if ( in != NULL )
    {
        return( in->maxiter );
    } // end if
    else
    {
        return 0;
    } // end else
} // end kes_get_maxiter

/******************************************************************************/

/*******************************************************************************
 * FUNCTION    : kes_set_tolf
 * DESCRIPTION : set new value for parameter "tolf" in data structure
 * INPUT       : - structure of type kes_input_t
 *               - value "tolf" (ignored if negative)
 * OUTPUT      : error code from kes_err_e
 ******************************************************************************/
inline kes_err_e kes_set_tolf(
    kes_input_t* in,
    const double tolf
    )
{
    /* default value: error */
    kes_err_e err = KES_ERR_BADTOL;

    /* check input error tolerance to be a valid number */
    if (
        (kes_check_val(tolf) == KES_ERR_NOERR) &&
        (tolf > std_tol) &&
        (tolf < 1.0)
    ) {
        in->tolf = tolf;
        err      = KES_ERR_NOERR;
    } // end if

    return( err );
} // end kes_set_tolf

/******************************************************************************/

/*******************************************************************************
 * FUNCTION    : kes_set_tolx
 * DESCRIPTION : set new value for parameter "tolx" in data structure
 * INPUT       : - structure of type kes_input_t
 *               - value "tolx" (ignored if negative)
 * OUTPUT      : error code from kes_err_e
 ******************************************************************************/
inline kes_err_e kes_set_tolx(
    kes_input_t* in,
    const double tolx
    )
{
    /* default value: error */
    kes_err_e err = KES_ERR_BADTOL;

    /* check input error tolerance to be a valid number */
    if (
        (kes_check_val(tolx) == KES_ERR_NOERR) &&
        (tolx > std_tol) &&
        (tolx < 1.0)
    ) {
        in->tolx = tolx;
        err      = KES_ERR_NOERR;
    } // end if

    return( err );
} // end kes_set_tolx

/******************************************************************************/

/*******************************************************************************
 * FUNCTION    : kes_set_maxiter
 * DESCRIPTION : set new value for parameter "maxiter" in data structure
 * INPUT       : - structure of type kes_input_t
 *               - value "maxiter"
 * OUTPUT      : error code from kes_err_e
 ******************************************************************************/
inline kes_err_e kes_set_maxiter(
    kes_input_t* in,
    const int    maxiter
    )
{
    /* default value: error */
    kes_err_e err = KES_ERR_BADVAL;

    /* check input value to be a suitable number
     * TODO FIXME limit to how many times std_maxiter ???
     */
    if (
        (maxiter > 0) &&
        (maxiter < 10 * std_maxiter)
    ) {
        in->maxiter = maxiter;
        err         = KES_ERR_NOERR;
    } // end if

    return( err );
} // end kes_set_maxiter

/******************************************************************************/

/*******************************************************************************
 * FUNCTION    : kes_new_input_dummy
 * DESCRIPTION : dummy function to return input structure "in"
 * INPUT       : structure of type kes_input_t
 * OUTPUT      : structure of type kes_input_t
 ******************************************************************************/
kes_input_t kes_new_input_dummy(kes_input_t in)
{
    return in;
} // end kes_new_input_dummy

/******************************************************************************/
