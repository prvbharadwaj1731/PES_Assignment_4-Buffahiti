/*
 * log.h
 *
 *  Created on: 01-Mar-2022
 *      Author: prvbh
 */

#ifndef LOG_H_
#define LOG_H_

#include <stdio.h>
#ifdef DEBUG
#define LOG printf
#else
#define LOG (...)
#endif

#endif /* LOG_H_ */
