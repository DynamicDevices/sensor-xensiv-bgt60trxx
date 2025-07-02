/***********************************************************************************************//**
 * \file basic_example.c
 *
 * \brief
 * Basic example demonstrating XENSIV BGT60TRxx sensor initialization and device detection
 * on Linux platforms including Yocto Embedded Linux.
 *
 ***************************************************************************************************
 * \copyright
 * Copyright 2022 Infineon Technologies AG
 * SPDX-License-Identifier: Apache-2.0
 **************************************************************************************************/

// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <stdbool.h>
#include <stdint.h>
#include <signal.h>
#include <stdint.h>
#include <signal.h>

// Include the main library headers
#include "../xensiv_bgt60trxx.h"
#include "../xensiv_bgt60trxx_linux.h"

// ... existing code ...

/*******************************************************************************
* Macros
*******************************************************************************/
#define DEFAULT_SPI_DEVICE      "/dev/spidev0.0"
#define DEFAULT_GPIO_CHIP       "/dev/gpiochip0"
#define DEFAULT_RST_GPIO        18
#define DEFAULT_CS_GPIO         24

/*******************************************************************************
* Global Variables
*******************************************************************************/
static volatile bool g_running = true;
static xensiv_bgt60trxx_linux_obj_t g_sensor_obj;

/*******************************************************************************
* Function Prototypes
*******************************************************************************/
static void signal_handler(int sig);
static void print_usage(const char* program_name);
static void print_device_info(const xensiv_bgt60trxx_t* dev);

/*******************************************************************************
* Function Implementations
*******************************************************************************/

static void signal_handler(int sig)
{
    (void)sig;
    g_running = false;
    printf("\nShutdown requested...\n");
}

static void print_usage(const char* program_name)
{
    printf("Usage: %s [options]\n", program_name);
    printf("Options:\n");
    printf("  -s <device>    SPI device path (default: %s)\n", DEFAULT_SPI_DEVICE);
    printf("  -g <chip>      GPIO chip path (default: %s)\n", DEFAULT_GPIO_CHIP);
    printf("  -r <offset>    Reset GPIO offset (default: %d)\n", DEFAULT_RST_GPIO);
    printf("  -c <offset>    CS GPIO offset (default: %d)\n", DEFAULT_CS_GPIO);
    printf("  -h             Show this help message\n");
    printf("\nExample:\n");
    printf("  %s -s /dev/spidev1.0 -g /dev/gpiochip1 -r 20 -c 21\n", program_name);
}

static void print_device_info(const xensiv_bgt60trxx_t* dev)
{
    xensiv_bgt60trxx_device_t device_type = xensiv_bgt60trxx_get_device(dev);
    uint16_t fifo_size = xensiv_bgt60trxx_get_fifo_size(dev);
    
    printf("Device Information:\n");
    printf("  Device Type: ");
    
    switch (device_type) {
        case XENSIV_DEVICE_BGT60TR13C:
            printf("BGT60TR13C\n");
            break;
        case XENSIV_DEVICE_BGT60UTR13D:
            printf("BGT60UTR13D\n");
            break;
        case XENSIV_DEVICE_BGT60UTR11:
            printf("BGT60UTR11\n");
            break;
        default:
            printf("Unknown\n");
            break;
    }
    
    printf("  FIFO Size: %u samples\n", fifo_size);
}

int main(int argc, char* argv[])
{
    const char* spi_device = DEFAULT_SPI_DEVICE;
    const char* gpio_chip = DEFAULT_GPIO_CHIP;
    unsigned int rst_gpio = DEFAULT_RST_GPIO;
    unsigned int cs_gpio = DEFAULT_CS_GPIO;
    int opt;
    int32_t result;

    // Parse command line arguments
    while ((opt = getopt(argc, argv, "s:g:r:c:h")) != -1) {
        switch (opt) {
            case 's':
                spi_device = optarg;
                break;
            case 'g':
                gpio_chip = optarg;
                break;
            case 'r':
                rst_gpio = (unsigned int)atoi(optarg);
                break;
            case 'c':
                cs_gpio = (unsigned int)atoi(optarg);
                break;
            case 'h':
                print_usage(argv[0]);
                return 0;
            default:
                print_usage(argv[0]);
                return 1;
        }
    }

    // Set up signal handlers
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);

    printf("XENSIV BGT60TRxx Basic Example\n");
    printf("==============================\n");
    printf("SPI Device: %s\n", spi_device);
    printf("GPIO Chip: %s\n", gpio_chip);
    printf("Reset GPIO: %u\n", rst_gpio);
    printf("CS GPIO: %u\n", cs_gpio);
    printf("\n");

    // Initialize the sensor
    printf("Initializing sensor...\n");
    result = xensiv_bgt60trxx_linux_init_sensor(&g_sensor_obj,
                                               spi_device,
                                               gpio_chip,
                                               rst_gpio,
                                               cs_gpio,
                                               false); // Normal speed mode

    if (result != XENSIV_BGT60TRXX_STATUS_OK) {
        fprintf(stderr, "Failed to initialize sensor: %d\n", result);
        return 1;
    }

    printf("Sensor initialized successfully!\n\n");

    // Print device information
    print_device_info(&g_sensor_obj.dev);

    // Test register access
    printf("\nTesting register access...\n");
    uint32_t chip_id;
    result = xensiv_bgt60trxx_get_reg(&g_sensor_obj.dev, XENSIV_BGT60TRXX_REG_CHIP_ID, &chip_id);
    if (result == XENSIV_BGT60TRXX_STATUS_OK) {
        printf("Chip ID: 0x%08X\n", chip_id);
    } else {
        fprintf(stderr, "Failed to read chip ID: %d\n", result);
    }

    // Test FIFO status
    uint32_t fifo_status;
    result = xensiv_bgt60trxx_get_fifo_status(&g_sensor_obj.dev, &fifo_status);
    if (result == XENSIV_BGT60TRXX_STATUS_OK) {
        printf("FIFO Status: 0x%08X\n", fifo_status);
    } else {
        fprintf(stderr, "Failed to read FIFO status: %d\n", result);
    }

    printf("\nSensor is ready for operation.\n");
    printf("Press Ctrl+C to exit...\n");

    // Main loop - just keep the program running
    while (g_running) {
        sleep(1);
    }

    // Cleanup
    printf("Cleaning up...\n");
    xensiv_bgt60trxx_linux_deinit_sensor(&g_sensor_obj);
    
    printf("Example completed successfully.\n");
    return 0;
}