cmake_minimum_required( VERSION "3.20.0" )

project( I2CLib VERSION 1.0 LANGUAGES CXX )

option( I2C_INCLUDE_UNIT_TESTS "Compile unit tests." ON )
option( I2C_CORE_GENERATE_DOC "Generate documentation." ON )

set( CMAKE_CXX_STANDARD 17 )
set( CMAKE_CXX_STANDARD_REQUIRED TRUE )

add_subdirectory( Source )

if ( I2C_INCLUDE_UNIT_TESTS )
    add_subdirectory( Test )
endif()

if ( I2C_CORE_GENERATE_DOC )
        find_package( Doxygen )
        if ( DOXYGEN_FOUND )
                set( DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/DocDoxygen/Doxyfile.in )
                set( DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile )

                configure_file( ${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY )

                add_custom_target( Docs
                        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
                        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                        COMMENT "Generating API documentation with Doxygen"
                        VERBATIM )
        else ()
                message( "Doxygen has not been found on the current system. Install it in order to generate the documentation." )
        endif ()
endif()
