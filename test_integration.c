/**
 * @file test_integration.c
 * @brief Integration test for XENSIV BGT60TRxx library
 * 
 * This test verifies that the library can be compiled and linked correctly,
 * and that basic API functions are accessible.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

// Include the main library header
#include "xensiv_bgt60trxx.h"

/**
 * @brief Test basic library constants and types
 * @return 0 on success, non-zero on failure
 */
static int test_basic_constants(void)
{
    printf("Testing basic library constants...\n");
    
    // Test that error codes are defined correctly
    assert(XENSIV_BGT60TRXX_STATUS_OK == 0);
    assert(XENSIV_BGT60TRXX_STATUS_COM_ERROR != 0);
    assert(XENSIV_BGT60TRXX_STATUS_DEV_ERROR != 0);
    assert(XENSIV_BGT60TRXX_STATUS_TIMEOUT_ERROR != 0);
    assert(XENSIV_BGT60TRXX_STATUS_GSR0_ERROR != 0);
    
    // Test that FIFO constants are reasonable
    assert(XENSIV_BGT60TRXX_NUM_SAMPLES_FIFO_WORD == 2);
    assert(XENSIV_BGT60TRXX_FIFO_WORD_SIZE_BYTES == 3);
    assert(XENSIV_BGT60TRXX_SPI_BURST_HEADER_SIZE_BYTES == 4);
    
    // Test that initial test word is defined
    assert(XENSIV_BGT60TRXX_INITIAL_TEST_WORD == 0x0001U);
    
    printf("✓ Basic constants test passed\n");
    return 0;
}

/**
 * @brief Test device enumeration
 * @return 0 on success, non-zero on failure
 */
static int test_device_types(void)
{
    printf("Testing device type enumeration...\n");
    
    // Test that device types are defined
    assert(XENSIV_DEVICE_BGT60TR13C == 0);
    assert(XENSIV_DEVICE_BGT60UTR13D == 1);
    assert(XENSIV_DEVICE_BGT60UTR11 == 2);
    assert(XENSIV_DEVICE_UNKNOWN == -1);
    
    printf("✓ Device types test passed\n");
    return 0;
}

/**
 * @brief Test reset command enumeration
 * @return 0 on success, non-zero on failure
 */
static int test_reset_commands(void)
{
    printf("Testing reset command enumeration...\n");
    
    // Test that reset commands are defined (values depend on register definitions)
    // We just test that they're different values
    assert(XENSIV_BGT60TRXX_RESET_SW != XENSIV_BGT60TRXX_RESET_FSM);
    assert(XENSIV_BGT60TRXX_RESET_FSM != XENSIV_BGT60TRXX_RESET_FIFO);
    assert(XENSIV_BGT60TRXX_RESET_SW != XENSIV_BGT60TRXX_RESET_FIFO);
    
    printf("✓ Reset commands test passed\n");
    return 0;
}

/**
 * @brief Test structure initialization
 * @return 0 on success, non-zero on failure
 */
static int test_structure_init(void)
{
    printf("Testing structure initialization...\n");
    
    // Test that we can create and initialize a sensor object structure
    xensiv_bgt60trxx_t sensor;
    memset(&sensor, 0, sizeof(sensor));
    
    // Test that the structure has the expected members
    sensor.iface = NULL;
    sensor.type = NULL;
    sensor.high_speed = false;
    
    // Test that the structure size is reasonable
    assert(sizeof(xensiv_bgt60trxx_t) > 0);
    assert(sizeof(xensiv_bgt60trxx_t) < 1000); // Sanity check
    
    printf("✓ Structure initialization test passed\n");
    return 0;
}

/**
 * @brief Test that function prototypes are available (linking test)
 * @return 0 on success, non-zero on failure
 */
static int test_function_availability(void)
{
    printf("Testing function availability (linking test)...\n");
    
    // Test that function symbols are available for linking
    // We don't call them since we don't have hardware, but we verify they exist
    
    // These should not be NULL if the library is properly linked
    void* init_func = (void*)xensiv_bgt60trxx_init;
    void* get_device_func = (void*)xensiv_bgt60trxx_get_device;
    void* config_func = (void*)xensiv_bgt60trxx_config;
    void* set_reg_func = (void*)xensiv_bgt60trxx_set_reg;
    void* get_reg_func = (void*)xensiv_bgt60trxx_get_reg;
    
    assert(init_func != NULL);
    assert(get_device_func != NULL);
    assert(config_func != NULL);
    assert(set_reg_func != NULL);
    assert(get_reg_func != NULL);
    
    printf("✓ Function availability test passed\n");
    return 0;
}

/**
 * @brief Main test function
 * @return 0 on success, non-zero on failure
 */
int main(void)
{
    printf("XENSIV BGT60TRxx Library Integration Test\n");
    printf("=========================================\n\n");
    
    int result = 0;
    
    // Run all tests
    result |= test_basic_constants();
    result |= test_device_types();
    result |= test_reset_commands();
    result |= test_structure_init();
    result |= test_function_availability();
    
    if (result == 0) {
        printf("\n✓ All integration tests passed!\n");
        printf("Library can be compiled and linked successfully.\n");
        printf("Basic API constants and types are correctly defined.\n");
        printf("Function symbols are available for linking.\n");
    } else {
        printf("\n✗ Some integration tests failed!\n");
        return 1;
    }
    
    return 0;
}