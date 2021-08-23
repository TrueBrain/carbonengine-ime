project(Ime VERSION 1.0.0)

add_library(Ime SHARED
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
target_include_directories(Ime PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/..)

if(APPLE)
    # We need this as a separate library due to namespace collisions between blue and Carbon
    add_library(KeyboardLayout STATIC KeyboardLayoutMac.h KeyboardLayoutMac.mm)
    target_link_libraries(Ime PRIVATE KeyboardLayout)
    set_target_properties(
        Ime
        PROPERTIES 
            PREFIX ""
            SUFFIX ".so"
    )

    enable_language(OBJCXX)
    target_link_libraries(Ime PRIVATE "-framework Foundation -framework IOKit -framework AppKit -framework Carbon")
endif()
set_target_properties(Ime PROPERTIES OUTPUT_NAME "_ime")
set_target_properties(Ime PROPERTIES CXX_VISIBILITY_PRESET hidden)

get_target_property(_SOURCES Ime SOURCES)