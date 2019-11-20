/***************************************************************************//**
 * @file    version.h
 * @brief   provide version number for Kepler Equation Solver Library
 * @author  Bazso Akos
 * @version 2019.11
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
#pragma once
#ifndef KES_VERSION__H
#define KES_VERSION__H

/******************************************************************************/

/*** function declarations ***/

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * @brief print the Library's version number to stdout
 * @return none
 */
void kes_show_version(void);


/*!
 * @brief query the current major version
 * @return major version as integer
 */
int kes_get_major_version(void);


/*!
 * @brief query the current minor version
 * @return minor version as integer
 */
int kes_get_minor_version(void);

#ifdef __cplusplus
}
#endif

/******************************************************************************/

#endif  /* KES_VERSION__H */
