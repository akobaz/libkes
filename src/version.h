/***************************************************************************//**
 * @file    version.h
 * @brief   provide version number for Kepler Equation Solver Library
 * @author  Bazso Akos
 * @version 2019.02
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
