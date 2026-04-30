cmake_minimum_required(VERSION 3.16)
project(Ime VERSION 1.0.0)

include(cmake/CcpBuildConfigurations.cmake)
include(cmake/CcpTargetConfigurations.cmake)
include(cmake/CcpPackageConfigHelpers.cmake)
include(CMakePackageConfigHelpers)

find_package(carbon-blue REQUIRED COMPONENTS Blue)

ccp_add_library(Ime SHARED
    Ime.h
    ImeWrapper.h
    KeyboardLayoutMac.h
    StdAfx.h
    Ime.cpp
    ImeWrapper.cpp
    Ime_Blue.cpp
    StdAfx.cpp
)
target_link_libraries(Ime PRIVATE Blue)

target_precompile_headers(Ime PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/StdAfx.h)
target_include_directories(Ime PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

if(APPLE)
    # We need this as a separate library due to namespace collisions between blue and Carbon
    add_library(KeyboardLayout STATIC KeyboardLayoutMac.h KeyboardLayoutMac.mm)
    target_link_libraries(Ime PRIVATE KeyboardLayout)
    enable_language(OBJCXX)
    target_link_libraries(Ime PRIVATE "-framework Foundation -framework IOKit -framework AppKit -framework Carbon")
endif()
set_target_properties(Ime PROPERTIES OUTPUT_NAME "_ime")
set_target_properties(Ime PROPERTIES CXX_VISIBILITY_PRESET hidden)

get_target_property(_SOURCES Ime SOURCES)

# Provide an install target iff this is the top level project only
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    option(INSTALL_TO_MONOLITH "Install this package to a perforce branch vendor folder - provided for backwards compatability" OFF)

    if(INSTALL_TO_MONOLITH)
        # Install rule to ensure that our runtime and linker files are in the expected, platform-specific folders
        install(
            TARGETS Ime
            CONFIGURATIONS ${CMAKE_CONFIGURATION_TYPES}
            ARCHIVE DESTINATION lib/${CCP_PLATFORM}/${CCP_ARCHITECTURE}/${CCP_TOOLSET}/
            LIBRARY DESTINATION lib/${CCP_PLATFORM}/${CCP_ARCHITECTURE}/${CCP_TOOLSET}/
            RUNTIME DESTINATION bin/${CCP_PLATFORM}/${CCP_ARCHITECTURE}/${CCP_TOOLSET}/
        )
    else()
        install(
            TARGETS Ime
            EXPORT ImeTargets
            CONFIGURATIONS ${CMAKE_CONFIGURATION_TYPES}
            LIBRARY DESTINATION lib/
            ARCHIVE DESTINATION lib/
            RUNTIME DESTINATION bin/
        )

        install(
            EXPORT ImeTargets
            DESTINATION share/carbon-ime
            FILE carbon-ime.cmake
        )

        install(
            FILES carbon-imeConfig.cmake DESTINATION share/carbon-ime
        )
    endif()

    # Install python packages
    install(DIRECTORY python/ DESTINATION python
            FILES_MATCHING
            PATTERN "*.py"
            PATTERN "package.yaml"
    )
endif()
