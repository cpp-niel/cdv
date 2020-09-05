cmake_minimum_required (VERSION 3.15)

project (unit_tests)

find_package(doctest CONFIG REQUIRED)

add_executable (unit_tests
        core/color/interpolator.cpp
        core/rgba_color.cpp
        core/vec2.cpp
        elem/area.cpp elem/axis.cpp
        elem/color_legend.cpp
        elem/draw_line_markers.cpp
        elem/draw_polygonal_markers.cpp
        elem/draw_round_markers.cpp
        elem/line.cpp
        elem/range_stack.cpp
        elem/rectangle.cpp
        elem/scatter.cpp
        elem/swatch_legend.cpp
        elem/symbol.cpp
        elem/text.cpp
        fnt/font_properties.cpp
        fnt/font_weights.cpp
        fnt/freetype.cpp
        fnt/text_shaper.cpp
        main.cpp
        scl/band_scale.cpp
        scl/linear_scale.cpp
        scl/log_scale.cpp
        scl/ordinal_scale.cpp
        scl/point_scale.cpp
        scl/sequential_scale.cpp
        scl/ticks.cpp
        scl/time_scale.cpp
        elem/arc.cpp elem/pie_slices.cpp)

set_property(TARGET unit_tests PROPERTY CXX_STANDARD 20)

target_include_directories(unit_tests PUBLIC ./../../include ./../../src ./)

target_link_libraries(unit_tests PRIVATE cdv project_warnings project_options doctest::doctest)

add_test(NAME unit_test COMMAND unit_tests)
