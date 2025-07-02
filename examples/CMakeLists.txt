cmake_minimum_required(VERSION 3.10)

# Example applications
if(ENABLE_LINUX_SUPPORT AND UNIX AND NOT APPLE)
    # Basic sensor initialization example
    add_executable(basic_example basic_example.c)
    target_link_libraries(basic_example xensiv_bgt60trxx)
    
    # FIFO data reading example
    add_executable(fifo_example fifo_example.c)
    target_link_libraries(fifo_example xensiv_bgt60trxx)
    
    # Configuration example
    add_executable(config_example config_example.c)
    target_link_libraries(config_example xensiv_bgt60trxx)
    
    # Install examples
    install(TARGETS basic_example fifo_example config_example
        RUNTIME DESTINATION bin/examples
    )
endif()