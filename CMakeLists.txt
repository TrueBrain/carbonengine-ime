cmake_minimum_required(VERSION 3.16)
project(Ime VERSION 1.0.0)

if(NOT DEFINED ENV{CCP_EVE_PERFORCE_BRANCH_PATH})
    message(FATAL_ERROR "Missing required environment variable CCP_EVE_PERFORCE_BRANCH_PATH")
elseif(NOT IS_ABSOLUTE $ENV{CCP_EVE_PERFORCE_BRANCH_PATH})
    message(FATAL_ERROR "CCP_EVE_PERFORCE_BRANCH_PATH needs to be an absolute path")
elseif(NOT EXISTS $ENV{CCP_EVE_PERFORCE_BRANCH_PATH})
    message(FATAL_ERROR "CCP_EVE_PERFORCE_BRANCH_PATH is pointing at a non-existing location $ENV{CCP_EVE_PERFORCE_BRANCH_PATH}")
elseif(NOT IS_DIRECTORY $ENV{CCP_EVE_PERFORCE_BRANCH_PATH})
    message(FATAL_ERROR "CCP_EVE_PERFORCE_BRANCH_PATH needs to be a directory")
else()
    message(STATUS "Using CCP_EVE_PERFORCE_BRANCH_PATH located at $ENV{CCP_EVE_PERFORCE_BRANCH_PATH}")
endif()

file(TO_CMAKE_PATH "$ENV{CCP_EVE_PERFORCE_BRANCH_PATH}" BRANCH_ROOT_DIR)
list(INSERT CMAKE_MODULE_PATH 0 ${BRANCH_ROOT_DIR}/cmake)

include(CcpGlobalSettings)

find_package(Blue REQUIRED)

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
    # Install rule to ensure that our runtime and linker files are in the expected, platform-specific folders
    install(
        TARGETS Ime
        CONFIGURATIONS ${CMAKE_CONFIGURATION_TYPES}
        ARCHIVE DESTINATION lib/${CCP_PLATFORM}/${CCP_ARCHITECTURE}/${CCP_TOOLSET}/
        LIBRARY DESTINATION lib/${CCP_PLATFORM}/${CCP_ARCHITECTURE}/${CCP_TOOLSET}/
        RUNTIME DESTINATION bin/${CCP_PLATFORM}/${CCP_ARCHITECTURE}/${CCP_TOOLSET}/
    )
    # Install python packages
    install(DIRECTORY python/ DESTINATION python
            FILES_MATCHING
            PATTERN "*.py"
            PATTERN "package.yaml"
    )
endif()
