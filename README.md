<img align="right" width=15% src="./tests/approval_tests/cdv/fig/approved_files/fig.cdv_logo.approved.svg">

# cdv

### C++ Data Visualization

Visualize data using composable elements in C++20.

[![GitHub license](https://img.shields.io/badge/license-MIT-blue.svg)](https://github.com/andrew-neill/cdv/blob/master/LICENSE)
 
<img 
    width=20% src="./tests/approval_tests/cdv/fig/approved_files/area_charts.area_charts.stacked_fill_between.approved.svg"><img
    width=20% src="./tests/approval_tests/cdv/fig/approved_files/bar_charts.bar_charts.discrete_distribution.approved.svg"><img
    width=20% src="./tests/approval_tests/cdv/fig/approved_files/line_charts.line_charts.logarithmic_x_axis.approved.svg"><img
    width=20% src="./tests/approval_tests/cdv/fig/approved_files/scatter_charts.scatter_charts.stem_plot.approved.svg"><img
    width=20% src="./tests/approval_tests/cdv/fig/approved_files/scatter_charts.scatter_charts.with_alpha_and_sizes.approved.svg">

<img 
    width=20% src="./tests/approval_tests/cdv/fig/approved_files/line_charts.line_charts.multiple_plot_with_shared_x_axis.approved.svg"><img
    width=20% src="./tests/approval_tests/cdv/fig/approved_files/pie_charts.pie_charts.donut_chart_with_padding.approved.svg"><img
    width=20% src="./tests/approval_tests/cdv/fig/approved_files/heatmaps.heatmaps.inferno_heatmap.approved.svg"><img
    width=20% src="./tests/approval_tests/cdv/fig/approved_files/bar_charts.bar_charts.grouped_hbars.approved.svg"><img
    width=20% src="./tests/approval_tests/cdv/fig/approved_files/pie_charts.pie_charts.sunburst.approved.svg">
    
See [here](/doc/examples.md) for more examples including source code.
    

**PLEASE NOTE:** This is not released software. Not even close. It is work in progress. 
At the time of writing, there are still many features missing that I would like to
add to the library. Many C++20 features that *cdv* depends on are not available in all major
compilers. *cdv* has dependencies to third party libraries in order to be able to use
pending standard library features - notably [fmt](https://github.com/fmtlib/fmt), 
[range-v3](https://github.com/ericniebler/range-v3) and 
[date](https://github.com/HowardHinnant/date).

The interfaces will undergo major changes. The library will be cut into
modules as module support becomes available. *cdv* also depends on another C++20 library
[mfl](https://github.com/cpp-niel/mfl) which faces some similar issues. If you are
using a recent gcc version (> 10.1) and have decent experience with CMake and 
vcpkg, then you may be able to build and run *mfl* and *cdv*. Build instructions are 
still woefully missing. And above all, despite all the unit and approval testing,
there will be many bugs waiting to be discovered.  

Currently, neither *mfl* nor *cdv* build on MSVC due to multiple pending C++20 features. *cdv*
does not build on Clang due to the lack of 
[CTAD for aggregates](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2019/p1816r0.pdf).

## Contents

-   [Introduction](#introduction)
-   [Design Goals](#design-goals)
-   [Getting Started](#getting-started)
-   [Build Instructions](#build-instructions)
-   [Documentation](#documentation)
-   [License](#license)
-   [Thanks](#thanks)

## Introduction

*cdv* is a C++20 data visualization library that draws inspiration from Python's 
[matplotlib](https://matplotlib.org) and in particular from JavaScript's 
[D3.js](https://d3js.org). It aims to provide powerful facilities that can be flexibly 
composed to create beautiful static data visualizations. The approach is fundamentally
data driven and has been designed to play well with C++20 ranges.

## Design Goals

The development of this library started primarily as an exercise in learning C++20. As such, it 
has been designed with several major C++20 features in mind. In particular modules,
designated initializers, coroutines, ranges and concepts. *cdv* attempts to use these and other 
features of the C++ language to provide composable elements from which data visualizations can 
be created. Often this means taking some input data and transforming the data points into *cdv* 
elements that can then be rendered using one of various different `render` calls.

At the moment the drawing in the back end uses [cairo](https://www.cairographics.org), but
care has been taken to keep the actual drawing code minimal and isolated so that other
back ends may be an option in the future.

Many C++ libraries attempt to avoid using other third party libraries because package
management is notoriously difficult. With the advent of powerful package
management solutions for C++ over the past few years, *cdv* is designed around the
availability of third party packages. It uses [vcpkg](https://github.com/Microsoft/vcpkg/)
and embraces dependencies on other libraries that have already solved challenges that
*cdv* faces.

### Non-Goals

Currently, there are no plans for animated or interactive visualizations, but it may well
be feasible to create more dynamic visualizations on the basis of *cdv*. 

While *cdv* has adapted some of the *D3.js* principles and terminology, it is not the
goal of this library to replicate D3's feature set or behavior. *cdv* is a very C++
oriented solution to the challenge of static data visualization. It is in no way
affiliated with matplotlib, D3 or any other data visualization package.

## Getting Started

There are four common steps to creating a data visualization using *cdv*:
1. Create a [frame](/doc/frames.md)
2. Create [scales](/doc/scl.md) that map data to things like positions or colors
3. Use the scales to create [elements](/doc/elem.md) that can be rendered. These can be very 
   basic like lines and rectangles or more sophisticated like axes and legends.
4. Render the elements within the frame to some target format like svg or png.

Just to get a taste of what using *cdv* can look like, let's look at a minimal example 
which demonstrates each of these steps without going into any detail.

#### Step 1: create a default frame. 

This establishes the dimensions of the drawing space.

```c++
constexpr auto frame = cdv::fig::frame();
```
<sup><a href='/tests/approval_tests/cdv/fig/line_charts.cpp#L296-L296' title='Go to snippet source file'>source</a></sup>

#### Step 2: create scales for the x and y dimensions

Here, the scales represent a mapping from the x-domain in which
the data is defined [0, 2] to horizontal positions within the frame and from the y-domain of
the data [0, 4] to vertical positions within the frame.

```c++
const auto x = cdv::scl::linear_scale(0.0, 2.0, frame.x0(), frame.x1());
const auto y = cdv::scl::linear_scale(0.0, 4.0, frame.y0(), frame.y1());
```
<sup><a href='/tests/approval_tests/cdv/fig/line_charts.cpp#L300-L301' title='Go to snippet source file'>source</a></sup>

#### Step 3: use the scales to create elements that can be rendered. 

First we create an x and a y axis.

```c++
const auto x_axis = cdv::elem::bottom_axis(x, frame.y0());
const auto y_axis = cdv::elem::left_axis(y, frame.x0());
```
<sup><a href='/tests/approval_tests/cdv/fig/line_charts.cpp#L305-L306' title='Go to snippet source file'>source</a></sup>

Then we use the input data to create a line. The input data here is just some linearly
distributed x values between zero and two, and some y values which are generated by squaring
the x values.

```c++
namespace rv = ::ranges::views;
const auto xs = rv::linear_distribute(0.0, 2.0, 100);
const auto ys = xs | rv::transform([](auto v) { return v * v; });
const auto curve = cdv::elem::line{.xs = xs | rv::transform(x), .ys = ys | rv::transform(y)};
```
<sup><a href='/tests/approval_tests/cdv/fig/line_charts.cpp#L310-L313' title='Go to snippet source file'>source</a></sup>

#### Step 4: render

Finally, the elements that have been created are passed to a `render_to_svg` call.

```c++
const auto svg = cdv::fig::render_to_svg_string(frame.dimensions(), x_axis, y_axis, curve);
```
<sup><a href='/tests/approval_tests/cdv/fig/line_charts.cpp#L317-L317' title='Go to snippet source file'>source</a></sup>

The resulting svg looks like this:

![](./tests/approval_tests/cdv/fig/approved_files/line_charts.line_charts.getting_started.approved.svg)

See [here](/doc/tutorial.md) for a more in depth tutorial


## Build Instructions

Using vcpkg:

Install dependencies:

```commandline
vcpkg install mfl
```

Then from *cdv* root directory:

```commandline
mkdir build
cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=<path/to/vcpkg.cmake>
make
``` 

## Documentation

The best place to start learning how to use *cdv* is the [tutorial](/doc/tutorial.md).

If you are interested in seeing some of the things that the library is capable of, then
there are [examples](/doc/examples.md) which provide both source code and the resulting
visualizations. This is probably not a good place to get started though, as the examples
have no explanations.

The API documentation can be found [here](/doc/cdv.md)

## License

<img align="right" src="http://opensource.org/trademarks/opensource/OSI-Approved-License-100x137.png">

The library is licensed under the [MIT License](http://opensource.org/licenses/MIT):

Copyright &copy; 2020 Niel Waldren

Permission is hereby granted, free of charge, to any person obtaining a copy of this software 
and associated documentation files (the “Software”), to deal in the Software without 
restriction, including without limitation the rights to use, copy, modify, merge, publish, 
distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom 
the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or 
substantial portions of the Software.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND 
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, 
DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

* * *

This library contains Lewis Baker's coroutine generator from the 
[cppcoro](https://github.com/lewissbaker/cppcoro) library which is 
licensed under the [MIT License](http://opensource.org/licenses/MIT) (see above).
Copyright &copy; [Lewis Baker](https://lewissbaker.github.io)

* * *

Until the C++20 date facilities become widely available in the major standard library 
implementations, this library contains Howard Hinnant's [date library](https://github.com/HowardHinnant/date)
which is licensed under the [MIT License](http://opensource.org/licenses/MIT) (see above).
Copyright &copy; 2015-2019 [Howard Hinnant et al.](http://howardhinnant.github.io)

## Thanks

[D3.js](https://d3js.org) and [matplotlib](https://matplotlib.org) are huge sources of
inspiration and information. The whole data driven concept of using scales to transform
data into drawable things is already available in far better and far more extensive
fashion in JavaScript via D3. If using JavaScript is an option, then in my opinion 
D3.js is the way to go.

### Third party libraries, fonts, tools and services

Many thanks to the authors and contributors of the following third party libraries, fonts,
tools and services with which *cdv* is developed, built and tested:
-   [**range-v3**](https://github.com/ericniebler/range-v3) for extended range support
-   [**fmt**](https://github.com/fmtlib/fmt) for text formatting (pending `std::format`)
-   [**date**](https://github.com/HowardHinnant/date) for date handling (pending `std::chrono` extensions)
-   [**cairo**](https://www.cairographics.org) for drawing in the back end
-   [**HarfBuzz**](http://harfbuzz.org/) for access to extended font information and regular text shaping
-   [**FreeType**](https://www.freetype.org) for general font handling
-   [**fontconfig**](https://www.freedesktop.org/wiki/Software/fontconfig/) for font configuration and access
-   [**Clang**](https://clang.llvm.org) for compilation, standard library, static analysis, sanitizers and code formatting
-   [**GCC**](https://gcc.gnu.org) for compilation, standard library
-   [**CMake**](https://cmake.org) for build automation
-   [**vcpkg**](https://github.com/Microsoft/vcpkg/) for dependency management
-   [**github**](https://github.com) for project hosting and continuous integration
-   [**cpp_starter_project**](https://github.com/lefticus/cpp_starter_project) for project configuration guidance
-   [**stixfonts**](https://github.com/stipub/stixfonts) for the Open Type fonts used in mathematical formula rendering
-   [**DejaVuFonts**](https://dejavu-fonts.github.io) for the monospace font used in mathematical formula rendering
-   [**doctest**](https://github.com/onqtam/doctest) for unit and approval testing
-   [**Approval Tests for C++**](https://github.com/approvals/ApprovalTests.cpp) for approval testing
-   [**libFuzzer**](http://llvm.org/docs/LibFuzzer.html) for fuzz testing
-   [**Hyde**](https://github.com/adobe/hyde) for API documentation
