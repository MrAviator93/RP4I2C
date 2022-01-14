set( EXECUTABLE_OUT_NAME main )

add_executable( ${EXECUTABLE_OUT_NAME} 
    main.cpp
    LM75Controller.cpp
    BMP180Controller.cpp
    I2CBusController.cpp
    MCP23017Controller.cpp
)

if ( UNIX )
    target_link_libraries( ${EXECUTABLE_OUT_NAME}
        pthread
    )

    set_target_properties( ${EXECUTABLE_OUT_NAME} PROPERTIES LINK_FLAGS "-no-pie" )
endif()
