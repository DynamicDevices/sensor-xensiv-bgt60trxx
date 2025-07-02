/***********************************************************************************************//**
 * \file xensiv_bgt60trxx_linux.h
 *
 * \brief
 * This file contains the Linux platform interface definitions
 * for interacting with the XENSIV(TM) BGT60TRxx 60GHz FMCW radar sensors.
 * Compatible with Yocto Embedded Linux and other Linux distributions.
 *
 ***************************************************************************************************
 * \copyright
 * Copyright 2022 Infineon Technologies AG
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 **************************************************************************************************/

#ifndef XENSIV_BGT60TRXX_LINUX_H_
#define XENSIV_BGT60TRXX_LINUX_H_

#ifdef __linux__

#include <stdint.h>
#include <stdbool.h>
#include "xensiv_bgt60trxx.h"

/**
 * \addtogroup group_board_libs_linux XENSIV BGT60TRxx Linux Platform
 * \{
 * XENSIV&trade; BGT60TRxx radar sensor Linux platform interface.
 *
 * This interface provides Linux-specific implementations for the XENSIV BGT60TRxx
 * radar sensor library, compatible with Yocto Embedded Linux and other Linux distributions.
 * It uses standard Linux kernel interfaces (spidev, GPIO character device) for hardware access.
 *
 */

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
* Data Structures
*******************************************************************************/

/**
 * @brief Linux platform interface structure for XENSIV BGT60TRxx sensor
 * 
 * This structure contains the Linux-specific interface objects needed to
 * communicate with the sensor hardware.
 */
typedef struct {
    int spi_fd;         /**< SPI device file descriptor */
    int gpio_chip_fd;   /**< GPIO chip file descriptor */
    int rst_gpio_fd;    /**< Reset GPIO line file descriptor */
    int cs_gpio_fd;     /**< Chip select GPIO line file descriptor */
} xensiv_bgt60trxx_linux_t;

/**
 * @brief Complete Linux sensor object combining device and interface
 */
typedef struct {
    xensiv_bgt60trxx_t dev;           /**< Core sensor device object */
    xensiv_bgt60trxx_linux_t iface;   /**< Linux platform interface */
} xensiv_bgt60trxx_linux_obj_t;

/*******************************************************************************
* Function Prototypes
*******************************************************************************/

/**
 * @brief Initialize the Linux platform interface for XENSIV BGT60TRxx sensor
 *
 * This function initializes the SPI and GPIO interfaces needed to communicate
 * with the XENSIV BGT60TRxx radar sensor on Linux systems.
 *
 * @param[out] obj Pointer to the Linux interface object to initialize
 * @param[in] spi_device Path to the SPI device (e.g., "/dev/spidev0.0")
 * @param[in] gpio_chip Path to the GPIO chip (e.g., "/dev/gpiochip0")
 * @param[in] rst_gpio_offset GPIO offset for the reset pin
 * @param[in] cs_gpio_offset GPIO offset for the chip select pin
 * @return XENSIV_BGT60TRXX_STATUS_OK if successful, error code otherwise
 *
 * @note The SPI device must be configured with appropriate permissions for the user
 * @note GPIO chip access requires appropriate permissions or running as root
 *
 * Example usage:
 * @code
 * xensiv_bgt60trxx_linux_t linux_iface;
 * int32_t result = xensiv_bgt60trxx_linux_init(&linux_iface,
 *                                              "/dev/spidev0.0",
 *                                              "/dev/gpiochip0",
 *                                              18,  // RST GPIO offset
 *                                              24); // CS GPIO offset
 * @endcode
 */
int32_t xensiv_bgt60trxx_linux_init(xensiv_bgt60trxx_linux_t* obj,
                                    const char* spi_device,
                                    const char* gpio_chip,
                                    unsigned int rst_gpio_offset,
                                    unsigned int cs_gpio_offset);

/**
 * @brief Deinitialize the Linux platform interface
 *
 * This function closes all file descriptors and cleans up resources
 * allocated during initialization.
 *
 * @param[in] obj Pointer to the Linux interface object to deinitialize
 */
void xensiv_bgt60trxx_linux_deinit(xensiv_bgt60trxx_linux_t* obj);

/**
 * @brief Initialize complete sensor object with Linux platform
 *
 * This is a convenience function that initializes both the platform interface
 * and the core sensor device in one call.
 *
 * @param[out] obj Pointer to the complete sensor object
 * @param[in] spi_device Path to the SPI device
 * @param[in] gpio_chip Path to the GPIO chip
 * @param[in] rst_gpio_offset GPIO offset for the reset pin
 * @param[in] cs_gpio_offset GPIO offset for the chip select pin
 * @param[in] high_speed Enable high-speed SPI mode
 * @return XENSIV_BGT60TRXX_STATUS_OK if successful, error code otherwise
 */
static inline int32_t xensiv_bgt60trxx_linux_init_sensor(xensiv_bgt60trxx_linux_obj_t* obj,
                                                         const char* spi_device,
                                                         const char* gpio_chip,
                                                         unsigned int rst_gpio_offset,
                                                         unsigned int cs_gpio_offset,
                                                         bool high_speed)
{
    int32_t result = xensiv_bgt60trxx_linux_init(&obj->iface, spi_device, gpio_chip,
                                                 rst_gpio_offset, cs_gpio_offset);
    if (result != XENSIV_BGT60TRXX_STATUS_OK) {
        return result;
    }

    result = xensiv_bgt60trxx_init(&obj->dev, &obj->iface, high_speed);
    if (result != XENSIV_BGT60TRXX_STATUS_OK) {
        xensiv_bgt60trxx_linux_deinit(&obj->iface);
    }

    return result;
}

/**
 * @brief Deinitialize complete sensor object
 *
 * @param[in] obj Pointer to the complete sensor object to deinitialize
 */
static inline void xensiv_bgt60trxx_linux_deinit_sensor(xensiv_bgt60trxx_linux_obj_t* obj)
{
    xensiv_bgt60trxx_linux_deinit(&obj->iface);
}

#ifdef __cplusplus
}
#endif

/** \} group_board_libs_linux */

#endif /* __linux__ */

#endif /* XENSIV_BGT60TRXX_LINUX_H_ */