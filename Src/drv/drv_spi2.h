/** @file 		drv_spi.h
 *  @brief
 *  	This file initializes the SPI peripheral as well as writes
 *  	and reads to another device over SPI
 *
 *  Still needs to implement SPI2 for OSD chip
 *  Find a way to reuse write and read functions for SPI1 and SPI2
 *
 *  @author 	Jeremy Wolfe
 *  @date 		23 FEB 2022
 */

#ifndef __DRV_SPI2_H__
#define __DRV_SPI2_H__

/* Defines */
#define SPI1_ENABLE					GPIOA->BSRR |= GPIO_BSRR_BR4
#define SPI1_DISABLE				GPIOA->BSRR |= GPIO_BSRR_BS4

#define SPI_BR_PRESCALER_16			(0x3 << 3U)
#define SPI_BR_PRESCALER_128		(0x6 << 3U)

/* Prototypes */
void spi2Init(void);
void spi2ReadBytes(uint8_t reg, uint8_t *pData, uint8_t size);
void spi2WriteBytes(uint8_t *pData, uint8_t size);
void spi2ReadOneByte(uint8_t reg, uint8_t *pData);
void spi2WriteOneByte(uint8_t reg, uint8_t data);

#endif /* __DRV_SPI2_H__ */
