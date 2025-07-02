/***********************************************************************************************//**
 * \file xensiv_bgt60trxx_linux.c
 *
 * \brief
 * This file contains the Linux platform functions implementation
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

#ifdef __linux__

/* Feature test macros for POSIX functions */
#define _POSIX_C_SOURCE 200809L
#define _DEFAULT_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>
#include <linux/gpio.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <assert.h>
#include <endian.h>

#include "xensiv_bgt60trxx_platform.h"
#include "xensiv_bgt60trxx_linux.h"

/*******************************************************************************
* Macros
*******************************************************************************/
#define XENSIV_BGT60TRXX_SPI_MODE           (SPI_MODE_0)
#define XENSIV_BGT60TRXX_SPI_BITS_PER_WORD  (8)
#define XENSIV_BGT60TRXX_SPI_MAX_SPEED_HZ   (10000000)  /* 10 MHz */
#define XENSIV_BGT60TRXX_GPIO_CONSUMER      "xensiv_bgt60trxx"

/*******************************************************************************
* Local Functions
*******************************************************************************/

/**
 * @brief Configure GPIO pin for output
 */
static int configure_gpio_output(int gpio_chip_fd, unsigned int offset, bool initial_value)
{
    struct gpiohandle_request req;
    int ret;

    memset(&req, 0, sizeof(req));
    req.lineoffsets[0] = offset;
    req.flags = GPIOHANDLE_REQUEST_OUTPUT;
    req.default_values[0] = initial_value ? 1 : 0;
    req.lines = 1;
    strncpy(req.consumer_label, XENSIV_BGT60TRXX_GPIO_CONSUMER, sizeof(req.consumer_label) - 1);

    ret = ioctl(gpio_chip_fd, GPIO_GET_LINEHANDLE_IOCTL, &req);
    if (ret < 0) {
        return -1;
    }

    return req.fd;
}

/**
 * @brief Set GPIO pin value
 */
static int set_gpio_value(int gpio_fd, bool value)
{
    struct gpiohandle_data data;
    
    data.values[0] = value ? 1 : 0;
    return ioctl(gpio_fd, GPIOHANDLE_SET_LINE_VALUES_IOCTL, &data);
}

/*******************************************************************************
* Public Functions
*******************************************************************************/

int32_t xensiv_bgt60trxx_linux_init(xensiv_bgt60trxx_linux_t* obj,
                                    const char* spi_device,
                                    const char* gpio_chip,
                                    unsigned int rst_gpio_offset,
                                    unsigned int cs_gpio_offset)
{
    if (!obj || !spi_device || !gpio_chip) {
        return XENSIV_BGT60TRXX_STATUS_COM_ERROR;
    }

    memset(obj, 0, sizeof(xensiv_bgt60trxx_linux_t));

    // Initialize SPI
    obj->spi_fd = open(spi_device, O_RDWR);
    if (obj->spi_fd < 0) {
        fprintf(stderr, "Failed to open SPI device %s: %s\n", spi_device, strerror(errno));
        return XENSIV_BGT60TRXX_STATUS_COM_ERROR;
    }

    // Configure SPI mode
    uint8_t mode = XENSIV_BGT60TRXX_SPI_MODE;
    if (ioctl(obj->spi_fd, SPI_IOC_WR_MODE, &mode) < 0) {
        fprintf(stderr, "Failed to set SPI mode: %s\n", strerror(errno));
        close(obj->spi_fd);
        return XENSIV_BGT60TRXX_STATUS_COM_ERROR;
    }

    // Configure SPI bits per word
    uint8_t bits = XENSIV_BGT60TRXX_SPI_BITS_PER_WORD;
    if (ioctl(obj->spi_fd, SPI_IOC_WR_BITS_PER_WORD, &bits) < 0) {
        fprintf(stderr, "Failed to set SPI bits per word: %s\n", strerror(errno));
        close(obj->spi_fd);
        return XENSIV_BGT60TRXX_STATUS_COM_ERROR;
    }

    // Configure SPI max speed
    uint32_t speed = XENSIV_BGT60TRXX_SPI_MAX_SPEED_HZ;
    if (ioctl(obj->spi_fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed) < 0) {
        fprintf(stderr, "Failed to set SPI max speed: %s\n", strerror(errno));
        close(obj->spi_fd);
        return XENSIV_BGT60TRXX_STATUS_COM_ERROR;
    }

    // Initialize GPIO chip
    obj->gpio_chip_fd = open(gpio_chip, O_RDWR);
    if (obj->gpio_chip_fd < 0) {
        fprintf(stderr, "Failed to open GPIO chip %s: %s\n", gpio_chip, strerror(errno));
        close(obj->spi_fd);
        return XENSIV_BGT60TRXX_STATUS_COM_ERROR;
    }

    // Configure RST GPIO
    obj->rst_gpio_fd = configure_gpio_output(obj->gpio_chip_fd, rst_gpio_offset, true);
    if (obj->rst_gpio_fd < 0) {
        fprintf(stderr, "Failed to configure RST GPIO: %s\n", strerror(errno));
        close(obj->gpio_chip_fd);
        close(obj->spi_fd);
        return XENSIV_BGT60TRXX_STATUS_COM_ERROR;
    }

    // Configure CS GPIO
    obj->cs_gpio_fd = configure_gpio_output(obj->gpio_chip_fd, cs_gpio_offset, true);
    if (obj->cs_gpio_fd < 0) {
        fprintf(stderr, "Failed to configure CS GPIO: %s\n", strerror(errno));
        close(obj->rst_gpio_fd);
        close(obj->gpio_chip_fd);
        close(obj->spi_fd);
        return XENSIV_BGT60TRXX_STATUS_COM_ERROR;
    }

    return XENSIV_BGT60TRXX_STATUS_OK;
}

void xensiv_bgt60trxx_linux_deinit(xensiv_bgt60trxx_linux_t* obj)
{
    if (!obj) {
        return;
    }

    if (obj->cs_gpio_fd >= 0) {
        close(obj->cs_gpio_fd);
    }
    if (obj->rst_gpio_fd >= 0) {
        close(obj->rst_gpio_fd);
    }
    if (obj->gpio_chip_fd >= 0) {
        close(obj->gpio_chip_fd);
    }
    if (obj->spi_fd >= 0) {
        close(obj->spi_fd);
    }

    memset(obj, 0, sizeof(xensiv_bgt60trxx_linux_t));
}

/*******************************************************************************
* Platform Interface Implementation
*******************************************************************************/

void xensiv_bgt60trxx_platform_rst_set(const void* iface, bool val)
{
    const xensiv_bgt60trxx_linux_t* obj = (const xensiv_bgt60trxx_linux_t*)iface;
    
    if (obj && obj->rst_gpio_fd >= 0) {
        set_gpio_value(obj->rst_gpio_fd, val);
    }
}

void xensiv_bgt60trxx_platform_spi_cs_set(const void* iface, bool val)
{
    const xensiv_bgt60trxx_linux_t* obj = (const xensiv_bgt60trxx_linux_t*)iface;
    
    if (obj && obj->cs_gpio_fd >= 0) {
        set_gpio_value(obj->cs_gpio_fd, val);
    }
}

int32_t xensiv_bgt60trxx_platform_spi_transfer(void* iface,
                                              uint8_t* tx_data,
                                              uint8_t* rx_data,
                                              uint32_t len)
{
    xensiv_bgt60trxx_linux_t* obj = (xensiv_bgt60trxx_linux_t*)iface;
    struct spi_ioc_transfer tr;
    int ret;

    if (!obj || obj->spi_fd < 0 || (!tx_data && !rx_data) || len == 0) {
        return XENSIV_BGT60TRXX_STATUS_COM_ERROR;
    }

    memset(&tr, 0, sizeof(tr));
    tr.tx_buf = (uintptr_t)tx_data;
    tr.rx_buf = (uintptr_t)rx_data;
    tr.len = len;
    tr.speed_hz = XENSIV_BGT60TRXX_SPI_MAX_SPEED_HZ;
    tr.bits_per_word = XENSIV_BGT60TRXX_SPI_BITS_PER_WORD;

    ret = ioctl(obj->spi_fd, SPI_IOC_MESSAGE(1), &tr);
    if (ret < 0) {
        fprintf(stderr, "SPI transfer failed: %s\n", strerror(errno));
        return XENSIV_BGT60TRXX_STATUS_COM_ERROR;
    }

    return XENSIV_BGT60TRXX_STATUS_OK;
}

int32_t xensiv_bgt60trxx_platform_spi_fifo_read(void* iface,
                                               uint16_t* rx_data,
                                               uint32_t len)
{
    xensiv_bgt60trxx_linux_t* obj = (xensiv_bgt60trxx_linux_t*)iface;
    struct spi_ioc_transfer tr;
    int ret;
    uint8_t* tx_buf = NULL;
    uint32_t byte_len = len * 2; // 16-bit data

    if (!obj || obj->spi_fd < 0 || !rx_data || len == 0) {
        return XENSIV_BGT60TRXX_STATUS_COM_ERROR;
    }

    // Allocate TX buffer filled with 0xFF for FIFO read
    tx_buf = malloc(byte_len);
    if (!tx_buf) {
        return XENSIV_BGT60TRXX_STATUS_COM_ERROR;
    }
    memset(tx_buf, 0xFF, byte_len);

    memset(&tr, 0, sizeof(tr));
    tr.tx_buf = (uintptr_t)tx_buf;
    tr.rx_buf = (uintptr_t)rx_data;
    tr.len = byte_len;
    tr.speed_hz = XENSIV_BGT60TRXX_SPI_MAX_SPEED_HZ;
    tr.bits_per_word = XENSIV_BGT60TRXX_SPI_BITS_PER_WORD;

    ret = ioctl(obj->spi_fd, SPI_IOC_MESSAGE(1), &tr);
    
    free(tx_buf);

    if (ret < 0) {
        fprintf(stderr, "SPI FIFO read failed: %s\n", strerror(errno));
        return XENSIV_BGT60TRXX_STATUS_COM_ERROR;
    }

    return XENSIV_BGT60TRXX_STATUS_OK;
}

void xensiv_bgt60trxx_platform_delay(uint32_t ms)
{
    struct timespec ts;
    ts.tv_sec = ms / 1000;
    ts.tv_nsec = (ms % 1000) * 1000000;
    nanosleep(&ts, NULL);
}

uint32_t xensiv_bgt60trxx_platform_word_reverse(uint32_t x)
{
    return htobe32(x);
}

void xensiv_bgt60trxx_platform_assert(bool expr)
{
    if (!expr) {
        fprintf(stderr, "XENSIV BGT60TRxx: Assertion failed!\n");
        abort();
    }
}

#endif /* __linux__ */