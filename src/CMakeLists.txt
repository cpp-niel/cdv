cmake_minimum_required (VERSION 3.15)

find_package(mfl REQUIRED)
find_package(fmt REQUIRED)
find_package(range-v3 REQUIRED)
find_package(unofficial-cairo REQUIRED)
find_package(unofficial-fontconfig REQUIRED)
find_package(Freetype REQUIRED)
find_package(harfbuzz REQUIRED)
find_package(Threads)

add_library(cdv
        back_end/cairo.cpp
        core/rgba_color.cpp
        fnt/font_weights.cpp
        fnt/freetype.cpp
        fnt/freetype_error.cpp
        fnt/mfl_font_face.cpp
        fnt/text_shaper.cpp
        )

set_property(TARGET cdv PROPERTY CXX_STANDARD 20)

target_compile_definitions(cdv PRIVATE CDV_DATA_DIR="${PROJECT_SOURCE_DIR}/data")

target_include_directories(cdv PUBLIC ./../include)
target_include_directories(cdv PRIVATE ./ ${CAIRO_INCLUDE_DIR})

target_link_libraries(cdv PUBLIC
        range-v3
        fmt::fmt
        mfl::mfl
        ${CMAKE_THREAD_LIBS_INIT}
        )

target_link_libraries(cdv PRIVATE
        project_warnings
        project_options
        unofficial::cairo::cairo
        unofficial::fontconfig::fontconfig
        Freetype::Freetype
        harfbuzz::harfbuzz
        )
