/** @file 		drv_lager.h
 *  @brief
 *  	This file initializes the UART and provides functions for writing
 *  	to the OpenLager.
 *
 *  @author 	Jeremy Wolfe
 *  @date 		07 APR 2022
 */

#ifndef __DRV_LAGER_H__
#define __DRV_LAGER_H__

/* Defines */
#define LOG_BYTES		LOG_SIZE * 4U

void lagerInit(void);
void lagerWriteLog(char *pLog);

#endif /* __DRV_LAGER_H__ */
