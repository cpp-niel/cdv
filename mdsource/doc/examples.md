# Examples

This page provides a sampling of visualizations that can be created using *cdv* along with
the code that was used to generate them. If you are new to *cdv* you may find it helpful
to go through the [tutorial](/doc/tutorial.md) first. There you will learn about many of 
the ideas that these examples build upon and it should be much easier to understand the
code here having completed the tutorial.

The examples often assume a prior 
- `using namespace ::cdv;`
- `using namespace ::cdv::units_literals;`
- `namespace rv = ::ranges::views;`

toc

## Simple Line Plot

This example is based on a similar matplotlib example where we generate curves for
x, x squared and x cubed functions for x in [0, 2]. We also create a legend using
the same color scale that was used to choose the colors for the curves. Notice, that
the legend text is in *TeX* format and rendered as a mathematical formula.

![](./../tests/approval_tests/cdv/fig/approved_files/line_charts.line_charts.full_chart_no_grid.approved.svg)

mdinject: example-full-chart-no-grid-data
mdinject: example-full-chart-no-grid


## Stacked Area Chart

Here we feed the data ranges into a *cdv* data structure called a `range_stack` which
effectively adds each new data set to the layer below it. This is very useful when
drawing stacked charts such as stacked bar charts, or in this case a stacked area chart:

![](./../tests/approval_tests/cdv/fig/approved_files/area_charts.area_charts.stacked_fill_between.approved.svg)

mdinject: example-stacked-area-chart


## Stem Plot

Again we replicate a matplotlib example here. This demonstrates how simple it is to
generate multiple elements for each data point. Here, both the marker and the stem
are simply returned as a pair. So we end up with a range of pairs of elements which
can just be passed to the render function as is.

![](./../tests/approval_tests/cdv/fig/approved_files/scatter_charts.scatter_charts.stem_plot.approved.svg)

mdinject: example-stem-plot


## Logarithmic Axis

In *cdv* you can use logarithmic axes for the x-axis, the y-axis or both. Here we see 
an example of using a base 10 logarithmic axis for the x-axis. Note also how the
overlaid formula is again specified in *TeX* syntax. *cdv* has built in support for
rendering mathematical formulas without having to install *LaTeX* or some similar
package. 

![](./../tests/approval_tests/cdv/fig/approved_files/line_charts.line_charts.logarithmic_x_axis.approved.svg)

mdinject: example-logarithmic-x-axis



## Time Line

In a similar manner to the stem plot we generate multiple elements per data point to
generate a simple but effective time line. This example also demonstrates the use of
dates, and in particular a date axis. Notice how the code does not use a frame, but
does use explicit pixel positions. This is usually not a good idea and would make
changing the display size in this example difficult.

![](./../tests/approval_tests/cdv/fig/approved_files/fig.time_line.approved.svg)

mdinject: example-time-line


## Scatter Plot

*cdv* supports many different *symbols* which are based on the available marker types
in matplotlib and use the same nomenclature. So in this scatter plot the symbol style
'P' represents a thick plus symbol: 

![](./../tests/approval_tests/cdv/fig/approved_files/scatter_charts.scatter_charts.with_varying_colors.approved.svg)

mdinject: example-scatter-plot



## Sharing Axes

In this plot, four different line plots are drawn, each with their own y-axis, but all
sharing the same x-axis:

![](./../tests/approval_tests/cdv/fig/approved_files/line_charts.line_charts.multiple_plot_with_shared_x_axis.approved.svg)

mdinject: example-shared-x-axis


## Bar Chart Distribution

This is another chart based on a matplotlib example. This example also uses a `range_stack` 
to stack the input data. Notice how each rectangle is defined by
- the question it belongs to (Q1, Q2, etc.) - aka the keys, and 
- the response given (strongly disagree, disagree etc.) - aka the categories. 

So the generation of the rectangles starts by generating the cartesian 
product of the keys and categories and then determines all the positions and labels and
colors by looking things up in the corresponding scales: 

![](./../tests/approval_tests/cdv/fig/approved_files/bar_charts.bar_charts.discrete_distribution.approved.svg)

mdinject: example-bar-chart-distribution


## Grouped Bar Chart

In a similar way to the bar chart distribution example, this example uses the cartesian
product of keys and categories to form groups. Note how both the positioning of each 
group and the position of each individual bar within a group is achieved using band scales. 

![](./../tests/approval_tests/cdv/fig/approved_files/bar_charts.bar_charts.grouped_hbars.approved.svg)

mdinject: example-group-bar-chart-data
mdinject: example-group-bar-chart


