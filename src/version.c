/*******************************************************************************
 * @file    version.c
 * @brief   provide version number for Kepler Equation Solver Library
 * @author  Bazso Akos
 * @version 1.0, 23 Feb 2019
 *          1.1, 10 Mar 2019
 *          1.2, 15 Sep 2019
 *          update of KES_VERSION_X on 18 Nov 2019
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
#define KES_VERSION_MINOR 11
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
    static const char info[] = "Copyright (C) 2011-2019 Bazso Akos";
    static const char gpl3[] =
    "This program is free software: you can redistribute it and/or modify\n"
    "it under the terms of the GNU Lesser General Public License as published by\n"
    "the Free Software Foundation, either version 3 of the License, or\n"
    "(at your option) any later version.\n"
    "\n"
    "This program is distributed in the hope that it will be useful,\n"
    "but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
    "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n"
    "GNU Lesser General Public License for more details.\n"
    "\n"
    "You should have received a copy of the GNU Lesser General Public License\n"
    "along with this program.  If not, see <https://www.gnu.org/licenses/>.\n";
    fprintf(
        stdout,
        "%s v%4d.%02d\n%s\n\n%s\n",
        //name, KES_VERSION, info, gpl3
        name, KES_VERSION_MAJOR, KES_VERSION_MINOR, info, gpl3
    );
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
