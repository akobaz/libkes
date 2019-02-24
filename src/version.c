/*******************************************************************************
 * MODULE  : version.c
 * PURPOSE : provide version number for Kepler Equation Solver Library
 * AUTHOR  : Bazso Akos
 * VERSION : 1.0, 23 Feb 2019
 ******************************************************************************/
 /* include standard headers */
#include <stdio.h>

/* include module headers */
#include "version.h"

/******************************************************************************/

/* use verbose debug output? 0 = no (default), 1 = yes */
#define KES_VERSION_DEBUG 0

/******************************************************************************/

/***
 * macros for version number as strings
 * NOTE solution after https://stackoverflow.com/questions/240353/
 ***/
#define STR(x)            #x
#define TOSTR(x)          STR(x)
#define KES_VERSION_MAJOR 2019
#define KES_VERSION_MINOR 02
#define KES_VERSION       TOSTR(KES_VERSION_MAJOR) "." TOSTR(KES_VERSION_MINOR)

/******************************************************************************/

/*******************************************************************************
 *  FUNCTION    : kes_version
 *  DESCRIPTION : print the library's version number to stdout
 *  INPUT       : none
 *  OUTPUT      : none
 ******************************************************************************/
inline void kes_show_version(void)
{
    static const char name[] = "Kepler Equation Solver Library";
    fprintf(stdout, "%s v%s\n", name, KES_VERSION);
    return;
} // end kes_show_version

/******************************************************************************/

/*******************************************************************************
 *  FUNCTION    : kes_get_major_version
 *  DESCRIPTION : query the current major version
 *  INPUT       : none
 *  OUTPUT      : major version as integer
 ******************************************************************************/
inline int kes_get_major_version(void)
{
    return( (int)KES_VERSION_MAJOR );
} // end kes_get_major_version

/******************************************************************************/

/*******************************************************************************
 *  FUNCTION    : kes_get_minor_version
 *  DESCRIPTION : query the current minor version
 *  INPUT       : none
 *  OUTPUT      : minor version as integer
 ******************************************************************************/
inline int kes_get_minor_version(void)
{
    return( (int)KES_VERSION_MINOR );
} // end kes_get_minor_version

/******************************************************************************/
