cmake_minimum_required (VERSION 3.15)

project (approval_tests)

find_package(doctest CONFIG REQUIRED)

add_executable (approval_tests
        cdv/elem/axis.cpp
        cdv/elem/color_legend.cpp
        cdv/elem/swatch_legend.cpp
        cdv/fig/area_charts.cpp
        cdv/fig/bar_charts.cpp
        cdv/fig/fig.cpp
        cdv/fig/line_charts.cpp
        cdv/fig/scatter_charts.cpp
        cdv/fig/tutorial.cpp
        main.cpp
        cdv/fig/heatmaps.cpp cdv/fig/pie_charts.cpp cdv/elem/symbols.cpp cdv/elem/lines.cpp)

set_property(TARGET approval_tests PROPERTY CXX_STANDARD 20)
target_compile_definitions(approval_tests PRIVATE TEST_DATA_DIR="${PROJECT_SOURCE_DIR}/../data")

target_include_directories(approval_tests PUBLIC ./ ./../../include ./../../src)
target_link_libraries(approval_tests PUBLIC cdv project_warnings project_options doctest::doctest)

