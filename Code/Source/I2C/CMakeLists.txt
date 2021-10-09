set( EXECUTABLE_OUT_NAME main )

add_executable( ${EXECUTABLE_OUT_NAME} 
    main.cpp
    BMP180Controller.cpp
    BMP388Controller.cpp
    I2CBusController.cpp
)

if ( UNIX )
    target_link_libraries( ${EXECUTABLE_OUT_NAME}
        pthread
    )

    set_target_properties( ${EXECUTABLE_OUT_NAME} PROPERTIES LINK_FLAGS "-no-pie" )
endif()
