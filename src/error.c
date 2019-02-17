/*******************************************************************************
 * MODULE  : error.c
 * PURPOSE : error handling functions for Kepler Equation Solver Library
 * AUTHOR  : Bazso Akos
 * VERSION : 1.0, 28 Dec 2011
 *           1.1, 23 Feb 2012
 *           1.2, 30 Sep 2013
 *           1.3, 16 Feb 2019
 ******************************************************************************/
/* include standard headers */
#include <stdio.h>

/* include module header */
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
