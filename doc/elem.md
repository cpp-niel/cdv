# elem

Definition of renderable elements and associated functionality

***



|File|Description|
| :-- | :-- |
| [area.hpp](#areahpp) | filled area delineated by lines and `fill_between` functions for creating areas |
| [symbol_properties.hpp](#symbol_propertieshpp) | properties of symbols as used for instance in scatter plots |
| [line_properties.hpp](#line_propertieshpp) | line drawing properties |
| [pie_slices.hpp](#pie_sliceshpp) | create a pie slice generator from an input range of values |
| [fill_properties.hpp](#fill_propertieshpp) | properties of elements that can be filled e.g. `area` or `rectangle` |
| [color_legend.hpp](#color_legendhpp) | legend element created from ordinal or sequential scales that map to colors |
| [rectangle.hpp](#rectanglehpp) | rectangular shaped element |
| [swatch_legend.hpp](#swatch_legendhpp) | legend element created from ordinal scales organized in columns of color swatches |
| [text_properties.hpp](#text_propertieshpp) | properties determining the rendering of text |
| [line.hpp](#linehpp) | line element representing one or more linear segments |
| [arc.hpp](#archpp) | filled circles and rings including circle and ring segments |
| [scatter.hpp](#scatterhpp) | multiple similar symbols as a single element |
| [symbol.hpp](#symbolhpp) | a single symbol as a single element |
| [axis.hpp](#axishpp) | generate lines, ticks and labels from a scale to represent an axis |
| [range_stack.hpp](#range_stackhpp) | stack multiple ranges on top of each other |
| [text.hpp](#texthpp) | element representing normal text and/or mathematical formulas |


## area.hpp

Source: [cdv/elem/area.hpp](/include/cdv/elem/area.hpp)


### area<XRange, YRange>

```c++
template <class XRange, class YRange>
struct cdv::elem::area;
```

|Field|Type|Description|
| :-- | :-- | :-- |
| fill | `cdv::elem::fill_properties` | properties which determine how the area should be filled |
| xs | `XRange` | the x coordinates of the delimiting polyline |
| ys | `YRange` | the y coordinates of the delimiting polyline |




An area is a highly flexible element which can represent any shape that can be enclosed in
a polyline. One common use of an area is in area charts for which there are convenience
functions which help to create areas, see [fill_between](#fill_between).


### fill_between

creates an area for x values by filling between y values

**Overload 1:**

```c++
template <class XRange, class TopYRange, class BaseYRange>
auto fill_between(const XRange & xs, const TopYRange & top_ys, const BaseYRange & base_ys, const cdv::elem::fill_properties & fill)
```

> Creates an `area` for the given x coordinates that fills the area between the two ranges of y coordinates that are passed in. The two ranges of y coordinates must both correspond to the given x coordinates. The area is formed by taking the `top_ys` in reverse order and then appending the `base_ys`. The resulting polyline defines the area. This is useful when filling the area defined by two lines in a chart. Both lines share the same x coordinates. The upper y values would be the `top_ys` the lower y coordinates would be the `base_ys`.

|Argument|Description|
| :-- | :-- |
| xs | the x coordinates |
| top_ys | the y coordinates that define the upper line |
| base_ys | the y coordinates that define the lower line |
| fill | properties which determine how the area should be filled |


**Overload 2:**

```c++
template <class XRange, class YRange, typename YType>
auto fill_between(const XRange & xs, const YRange & ys, const YType & y, const cdv::elem::fill_properties & fill)
```

> Creates an `area` for the given x coordinates that fills the area between the range of y coordinates and the constant value that are passed in. The range of y coordinates corresponds to the x coordinates and must have the same size in order to define a line. The area is defined by the points of that line in reverse order followed by the first x coordinate and the constant y value and then the last x coordinate and the constant y value. This essentially creates an area between the line defined by the `xs` and `ys` and the horizontal line defined by the `y` value.

|Argument|Description|
| :-- | :-- |
| xs | the x coordinates |
| ys | the y coordinates that define the line |
| y | the y value that defines the constant value to fill to |
| fill | properties which determine how the area should be filled |





The following example uses `fill_between` to create an area defined by the line given by 
`xs` and `ys` (which are transformed into pixel space by appropriate scales) and the
constant value 0.4:

```c++
const auto area =
    elem::fill_between(xs | rv::transform(x), ys | rv::transform(y), y(0.4), {.color = tab::pink});
```
<sup><a href='/tests/approval_tests/cdv/fig/area_charts.cpp#L55-L56' title='Go to snippet source file'>source</a></sup>

when plotted with axes the area looks like this:

![](./../tests/approval_tests/cdv/fig/approved_files/area_charts.area_charts.fill_between_arbitrary_constant.approved.svg)



<br />



## symbol_properties.hpp

Source: [cdv/elem/symbol_properties.hpp](/include/cdv/elem/symbol_properties.hpp)


### symbol_properties

```c++

struct cdv::elem::symbol_properties;
```

|Field|Type|Description|
| :-- | :-- | :-- |
| color | `cdv::rgba_color` | the color of the symbol |
| style | `char` | the style of the symbol (see below for more information) |




The symbol styles provided by *cdv* are based on the marker styles that will
be familiar to matplotlib users. They are specified in the form of `char` values. For 
instance, the style value `'*'` represents a five pointed star symbol. Here is a complete
list of the supported symbols and their corresponding `char` values:

![](./../tests/approval_tests/cdv/elem/approved_files/symbols.all_symbols.approved.svg)



## line_properties.hpp

Source: [cdv/elem/line_properties.hpp](/include/cdv/elem/line_properties.hpp)


### line_properties

```c++

struct cdv::elem::line_properties;
```

|Field|Type|Description|
| :-- | :-- | :-- |
| cap | `cdv::elem::cap_style` | the style in which the ends of lines are drawn. Default is `cap_style::butt` (see [cap_style](#enumeration-cap_style)) |
| color | `cdv::rgba_color` | the color of the line |
| join | `cdv::elem::join_style` | the style in which connected line segments are joined together. Default is `join_style::miter` (see [join_style](#enumeration-join_style)) |
| style | `cdv::elem::line_type` | the line style: solid, dashed etc. Default is solid (see [line_type](#line_type)) |
| width | `cdv::points` | the width of the line |




Lines appear in various different contexts in *cdv*. For example as parts of axes
or legends, or as outlines of filled shapes or simply as `line` objects. Their
appearance in all of these contexts can be controlled using the `line_properties`
data structure.


### line_type

```c++

struct cdv::elem::line_type;
```

|Field|Type|Description|
| :-- | :-- | :-- |
| dash_sequence | `std::vector<double>` | all line types are represented as a sequence of dashes. See below for a more thorough explanation. |




Whether a line is solid, dashed, dotted or some more complex combination is represented as a sequence of
`double` values. If the sequence is empty, then the line is solid. This is the default. Otherwise the
sequence represents the lengths of the dashes and gaps where the pattern of dashes and gaps is
repeated along the line to be drawn. The given values are a factor of the line's width. So for instance
a sequence of `{2.0, 1.0}` would create a dashed line where the dash is twice as long as the line is
wide and the gap is the same length as the line is wide.

Commonly, there is no need to specify the dash sequence explicitly because there are predefined
sequences for the most common line types which can be specified as strings in a similar way to
matplotlib:

| Line Type | Name | Short Name
| :-- | :-- | :-- |
| solid | "solid" | "-" |
| dashes | "dashed" | "--" |
| dots | "dotted" | ":" |
| dash / dots | "dashdot" | "-." |



#### Constructor: line_type

constructs a line type

**Overload 1:**

```c++
line_type()
```

> default constructor - represents a solid line

**Overload 2:**

```c++
line_type(cdv::elem::line_type &&)
```

> compiler generated move constructor

**Overload 3:**

```c++
line_type(const cdv::elem::line_type &)
```

> compiler generated copy constructor

**Overload 4:**

```c++
line_type(const char * s)
```

> construct a line type from a string. This is an intentionally implicit constructor to allow one to create line properties by, for instance, writing `{.style = ":"}` to represent a dotted line.

**Overload 5:**

```c++
line_type(std::string_view s)
```

> construct a line type from a `std::string_view`. See the description of the [line_type](#line_type) data structure for details on valid strings. If an invalid string is passed in, it will be ignored and the line type will be solid.




<br />



#### Enumeration: join_style

```c++
enum class cdv::elem::join_style
```

represents the different ways that the line segments can be joined

|Name|Description|
| :-- | :-- |
| miter | edges of line segments are extended until they meet at a sharp point |
| round | a round transition is made between edges of line segments |
| bevel | a flat transition is made between edges of line segments |




The following diagram shows the various different join styles for lines
segments meeting at a certain angle.

![](./../tests/approval_tests/cdv/elem/approved_files/lines.line_joins.approved.svg)



<br />

#### Enumeration: cap_style

```c++
enum class cdv::elem::cap_style
```

represents the different ways that the ends of lines can be capped

|Name|Description|
| :-- | :-- |
| butt | the line ends abruptly at the line end point |
| square | the line end point is enclosed in a square |
| round | the line end point is enclosed in a circle |




The following diagram shows the various different cap styles for lines
of identical length. Notice how the `square` capped line looks slightly longer
because the actual end points are at the center of the squares that cap the line.

![](./../tests/approval_tests/cdv/elem/approved_files/lines.line_caps.approved.svg)


<br />

## pie_slices.hpp

Source: [cdv/elem/pie_slices.hpp](/include/cdv/elem/pie_slices.hpp)


### pie_slice<Data>

```c++
template <typename Data>
struct cdv::elem::pie_slice;
```

|Field|Type|Description|
| :-- | :-- | :-- |
| data | `const Data &` | a reference to the data item that this pie slice represents |
| end_angle | `cdv::radians` | the end angle of the slice |
| pad_angle | `cdv::radians` | the padding of the slice (see [arc](#arc) for more information on padding) |
| start_angle | `cdv::radians` | the start angle of the slice |




The [pie_slices](#pie_slices) function returns a coroutine generator that generates instances
of this data structure. As such it is a kind of intermediate data structure used to convey
information from the pie slices generator to whatever needs the information about the pie
slices. Typically this will be arcs that are to be rendered in some form of radial chart.
See the documentation of `pie_slices` for more information.


### pie_geometry

```c++

struct cdv::elem::pie_geometry;
```

|Field|Type|Description|
| :-- | :-- | :-- |
| end_angle | `cdv::radians` | the end angle of the pie (default is 2π radians) |
| pad_angle | `cdv::radians` | the amount of padding to apply to each slice of the pie. When the pie to be sliced is not a full circle, padding may be applied to the ends of the pie slices. The padding works the same as for an [arc](#arc) element. (default is 0 - i.e. no padding) |
| start_angle | `cdv::radians` | the start angle of the pie (default is 0 radians) |




This type is designed to be an argument to the [pie_slices](#pie_slices) function. If all values are default, then
the `pie_slices` function will slice up an entire circle into pie slices. However, by setting start and end angles
for the pie geometry it is possible to use `pie_slices` to divide a pie *segment* into slices.


### label_angle

determines a good angle for a label within a pie slice assuming that the label is to be oriented along the line from the pie center through the center of the slice.

```c++
template <typename Data>
auto label_angle(const pie_slice<Data> & slice)
```

> returns the label angle in radians such that a text element rendered at this angle will run along the line from the pie center through the center of the given slice and that the text is always the right way up.

|Argument|Description|
| :-- | :-- |
| slice | The slice to determine the label angle for |





<br />



### pie_slices

cuts the pie with the given geometry into slices according to the given data.

```c++
template <typename Data, class inputs:auto, class get_value:auto>
cppcoro::generator<pie_slice<Data>> pie_slices(const stdx::range_of<Data> auto & inputs, const ranges::invocable<Data> auto & get_value, const cdv::elem::pie_geometry & geometry)
```

> This function returns a coroutine generator that can be used to generate pie slices for the given data. The data itself can be completely arbitrary, but it must be possible to extract a numerical value for each item of data. This numerical value is then used to determine the sizes of the respective pie slices.

|Argument|Description|
| :-- | :-- |
| inputs | a range of arbitrary input data. Each generated pie slice will contain a reference to the data item that it represents. |
| get_value | a function that is applied to each data item to get a number value that is used to compute the slices. |
| geometry | the geometry of the pie to slice up. If this is not provided, then it defaults to a full circle. |




The `pie_slices` function is particularly useful in conjunction with the [arc](#arc) element. The generated 
slices contain the information required to create arcs that can represent the pie slices in a pie
or donut chart. The following code demonstrates how to use pie slices to create arcs:

```c++
const auto data = std::array{std::pair{400, "flour"}, std::pair{15, "yeast"}, std::pair{200, "soy milk"},
                             std::pair{60, "margarine"}, std::pair{90, "sugar"}};
const auto color = scl::ordinal_scale(data | rv::values, scheme::dark2);
auto slices = elem::pie_slices(data, [](const auto p) { return p.first; });
const auto arcs = rv::all(slices) | rv::transform([&](const auto& slice) {
                      return elem::arc{.center = frame.center(),
                                       .outer_radius = frame.inner_height() / 2.0,
                                       .start_angle = slice.start_angle,
                                       .end_angle = slice.end_angle,
                                       .fill = {.color = color(slice.data.second)}};
                  });
```
<sup><a href='/tests/approval_tests/cdv/fig/pie_charts.cpp#L209-L219' title='Go to snippet source file'>source</a></sup>

Notice how the call to `pie_slices` also provides a lambda which lets the `pie_slices` function know
how to access the numerical values which determine the actual pie slices. Notice also in the
last line, how the data item is embedded in the slice and is used to get the
name which is passed to the color scale to determine the color of the slice.

Rendering the arcs would generate the following pie chart:

![](./../tests/approval_tests/cdv/fig/approved_files/pie_charts.pie_charts.simple_pie_chart.approved.svg)



<br />



### centroid

for a given pie slice, returns the position at the angle through the center of the pie slice and the given radius.

```c++
template <typename Data>
auto centroid(const pie_slice<Data> & slice, const cdv::pixel_pos center, const cdv::pixels radius)
```

|Argument|Description|
| :-- | :-- |
| slice | the slice to determine the centroid for |
| center | the center position of the pie |
| radius | the radius at which the centroid is to be positioned |




This function is very useful in determining a good place for labels within pie (or donut) slices. By choosing 
a radius that lies in the center of the pie slice, this function will deliver a position that
is in the center of the filled area for that slice. Alternatively, a radius just outside the slice will
provide a position that is angularly centered on the slice, but just beyond the edge of the slice.
This can also be a good position for labels in some visualizations.


<br />



## fill_properties.hpp

Source: [cdv/elem/fill_properties.hpp](/include/cdv/elem/fill_properties.hpp)


### fill_properties

```c++

struct cdv::elem::fill_properties;
```

|Field|Type|Description|
| :-- | :-- | :-- |
| color | `cdv::rgba_color` | the color to fill the element with (currently only single color fills are possible) |
| outline | `cdv::elem::line_properties` | the properties which determine how to render the shape's outline |





## color_legend.hpp

Source: [cdv/elem/color_legend.hpp](/include/cdv/elem/color_legend.hpp)


### color_legend<Scale>

```c++
template <typename Scale>
struct cdv::elem::color_legend;
```

|Field|Type|Description|
| :-- | :-- | :-- |
| block_height | `cdv::pixels` | the height of the block(s) that display the colors |
| height | `cdv::pixels` | the height of the legend (without a title) |
| num_ticks_hint | `size_t` | a hint for the number of ticks to use to divide up the legend |
| padding | `double` | the amount of padding between each block when using an ordinal scale. This is essentially the inner padding of a band scale that is used to position the blocks. See [scales](./scl.md#band_scale) for details on how this padding value is applied) |
| pos | `cdv::pixel_pos` | the bottom left corner of the legend |
| scale | `Scale` | the scale that the legend is to be created for. This can be an ordinal scale or a sequential scale, but must map to colors. |
| tick_label_properties | `cdv::elem::text_properties` | the properties that determine how to render the labels |
| tick_length | `cdv::pixels` | the length of the ticks (set this value to 0_px if ticks are not required |
| tick_line_properties | `cdv::elem::line_properties` | the properties that determine how the tick lines are rendered |
| title | `std::string` | the legend title |
| title_offset | `cdv::pixel_pos` | an offset from the title's default position which is left aligned and just above the legend |
| title_properties | `cdv::elem::text_properties` | the properties which determine how the title should be rendered |
| width | `cdv::pixels` | the width of the legend |




Color legends are a convenient way of providing a legend for a visualization. There is nothing special about them in the sense
that they are simply an arrangement of rectangles, lines and texts. It is possible to create legends that look completely
different. But, as a convenience, the `color_legend` element is available out of the box and may suffice for many typical
cases.

Often the colors in a visualization will be determined by an ordinal scale which maps some countable number of things to
specific colors. An example of creating a color legend for this kind of scale might look like this:

```c++
auto ordinal = scl::ordinal_scale(std::array{1, 2, 3, 4, 5, 6, 7, 8}, scheme::original_tableau10);
const auto ordinal_legend = elem::color_legend{
    .scale = ordinal, .pos = {50_px, 100_px}, .width = 500_px, .height = 30_px, .block_height = 15_px};
```
<sup><a href='/tests/approval_tests/cdv/elem/color_legend.cpp#L83-L85' title='Go to snippet source file'>source</a></sup>

Another common way of providing color in a visualization is to use a sequential scale that maps some continuous
range of values to a color interpolator. `color_legend` is also useful in this scenario. The following code would
create such a color legend:

```c++
const auto sequential = scl::sequential_scale(0.0, 1.0, interpolator::magma);
const auto sequential_legend = elem::color_legend{
    .scale = sequential, .pos = {50_px, 20_px}, .width = 500_px, .height = 30_px, .block_height = 15_px};
```
<sup><a href='/tests/approval_tests/cdv/elem/color_legend.cpp#L89-L91' title='Go to snippet source file'>source</a></sup>

These two types of scale will be rendered quite differently:

![](./../tests/approval_tests/cdv/elem/approved_files/color_legend.scale_types.approved.svg)



## rectangle.hpp

Source: [cdv/elem/rectangle.hpp](/include/cdv/elem/rectangle.hpp)


### rectangle

```c++

struct cdv::elem::rectangle;
```

|Field|Type|Description|
| :-- | :-- | :-- |
| corner_radius | `cdv::pixels` | the radius of the corners (default is 0_px i.e. no rounding) |
| fill | `cdv::elem::fill_properties` | the properties which determine how the rectangle is rendered |
| max | `cdv::pixel_pos` | the upper right corner of the rectangle |
| min | `cdv::pixel_pos` | the lower left corner of the rectangle |




The rectangle element is a commonly used element. It can, for example, be used as bars in bar charts or cells in
heat maps.


## swatch_legend.hpp

Source: [cdv/elem/swatch_legend.hpp](/include/cdv/elem/swatch_legend.hpp)


### swatch_legend<Domain>

```c++
template <typename Domain>
struct cdv::elem::swatch_legend;
```

|Field|Type|Description|
| :-- | :-- | :-- |
| block_height | `cdv::pixels` | the height of the swatch blocks (default is 16 pixels) |
| block_width | `cdv::pixels` | the width of the swatch blocks (default is 16 pixels) |
| columns | `std::vector<pixels>` | the widths of the columns. This determines how many columns there are. Leave this empty to place all the swatches in one row. (default is empty) |
| label_format_specifier | `std::string` | the specifier to pass to `fmt::format` in order to generate the label from the items in the scale's domain. (default is empty) |
| label_properties | `cdv::elem::text_properties` | the properties that determine how to render the swatch labels |
| pos | `cdv::pixel_pos` | the position of the first swatch in the first row of the legend |
| scale | `scl::ordinal_scale<Domain, rgba_color>` | the scale to generate the swatch legend for |
| title | `std::string` | the title of the legend (default is empty) |
| title_offset | `cdv::pixel_pos` | an offset from the title's default position which is left aligned and just above the legend |
| title_properties | `cdv::elem::text_properties` | the properties which determine how the title should be rendered |




`swatch_legend`s provide an alternative form of legend for ordinal scales to [color_legends](#color_legend). Each item is
rendered as a rectangular swatch with a label next to it and the swatches can be arranged in user defined columns.
The following code demonstrates how to create a simple swatch legend with three columns from an ordinal scale:

```c++
auto scale = scl::ordinal_scale(std::array{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11}, scheme::accent);
const auto legend =
    elem::swatch_legend{.scale = scale, .pos = {30_px, 210_px}, .columns = {100_px, 100_px, 100_px}};
```
<sup><a href='/tests/approval_tests/cdv/elem/swatch_legend.cpp#L28-L30' title='Go to snippet source file'>source</a></sup>

Rendering the swatch legend defined above produces the following output:

![](./../tests/approval_tests/cdv/elem/approved_files/swatch_legend.swatches_in_columns.approved.svg)



## text_properties.hpp

Source: [cdv/elem/text_properties.hpp](/include/cdv/elem/text_properties.hpp)


### text_properties

```c++

struct cdv::elem::text_properties;
```

|Field|Type|Description|
| :-- | :-- | :-- |
| color | `cdv::rgba_color` | the color of the text |
| font | `fnt::font_properties` | the font in which to render the text |
| font_size | `cdv::points` | the size of the font |





#### Enumeration: horizontal_anchor

```c++
enum class cdv::elem::horizontal_anchor
```

where text is anchored horizontally relative to its given position

|Name|Description|
| :-- | :-- |
| left | the left end of the text is anchored at the text position |
| center | the center of the text is anchored at the text position |
| right | the right end of the text is anchored at the text position |





<br />

#### Enumeration: vertical_anchor

```c++
enum class cdv::elem::vertical_anchor
```

where text is anchored vertically relative to its given position

|Name|Description|
| :-- | :-- |
| bottom | the bottom of the text is anchored at the text position |
| middle | the middle of the text is anchored at the text position |
| top | the top of the text is anchored at the text position |





<br />

## line.hpp

Source: [cdv/elem/line.hpp](/include/cdv/elem/line.hpp)


### line<XRange, YRange>

```c++
template <class XRange, class YRange>
struct cdv::elem::line;
```

|Field|Type|Description|
| :-- | :-- | :-- |
| properties | `cdv::elem::line_properties` | the properties with which the line should be rendered |
| xs | `XRange` | the x coordinates of the points that make up the line |
| ys | `YRange` | the y coordinates of the points that make up the line |




An `elem::line` in *cdv* is a line in the sense of a line plot and is
generally made up of several segments defined by ranges of x and y
coordinates. For instance - assuming the existence of x and y scales,
axes and some data - the line defined here:

```c++
namespace rv = ::ranges::views;
const auto line = cdv::elem::line(rv::iota(0, 11) | rv::transform(x), data | rv::transform(y));
const auto svg = cdv::fig::render_to_svg_string(frame.dimensions(), x_axis, y_axis, line);
```
<sup><a href='/tests/approval_tests/cdv/fig/tutorial.cpp#L90-L92' title='Go to snippet source file'>source</a></sup>

will take that data and plot it against x values from 0 to 10. The result would
look like this:

![](./../tests/approval_tests/cdv/fig/approved_files/tutorial.line_plot.approved.svg)



### vline

A convenience function for creating a vertical line

```c++
cdv::elem::line<std::__1::array<mfl::detail::quantity<mfl::pixels_tag>, 2>, std::__1::array<mfl::detail::quantity<mfl::pixels_tag>, 2>> vline(const cdv::pixels x, const cdv::pixels y0, const cdv::pixels y1, cdv::elem::line_properties properties)
```

> This function is essentially just a convenient way of constructing an `elem::line` for the commonly arising case that you want a single vertical line.

|Argument|Description|
| :-- | :-- |
| x | The x value of the line |
| y0 | The starting y value of the line |
| y1 | The ending y value of the line |
| properties | The properties of the line |





<br />



### hline

A convenience function for creating a horizontal line

```c++
cdv::elem::line<std::__1::array<mfl::detail::quantity<mfl::pixels_tag>, 2>, std::__1::array<mfl::detail::quantity<mfl::pixels_tag>, 2>> hline(const cdv::pixels x0, const cdv::pixels x1, const cdv::pixels y, cdv::elem::line_properties properties)
```

> This function is essentially just a convenient way of constructing an `elem::line` for the commonly arising case that you want a single horizontal line.

|Argument|Description|
| :-- | :-- |
| x0 | The starting x value of the line |
| x1 | The ending x value of the line |
| y | The y value of the line |
| properties | The properties of the line |





<br />



## arc.hpp

Source: [cdv/elem/arc.hpp](/include/cdv/elem/arc.hpp)


### arc

```c++

struct cdv::elem::arc;
```

|Field|Type|Description|
| :-- | :-- | :-- |
| center | `cdv::pixel_pos` | the center of the circle that defines the arc |
| end_angle | `cdv::radians` | the angle at which the arc ends (default is 2π radians) |
| fill | `cdv::elem::fill_properties` | properties which determine how the arc should be filled |
| inner_radius | `cdv::pixels` | the inner radius of the arc. Set this to a value greater than zero in order to represent a ring (segment) rather than a circle (segment) (default is 0 - i.e. no inner radius) |
| outer_radius | `cdv::pixels` | the outer radius of the arc. This is the radius of the circle (segment) if the inner radius is zero and the outer radius of the ring (segment) if the inner radius is positive. |
| pad_angle | `cdv::radians` | the amount of padding to apply to each end of the arc. When drawing a segment rather than a complete circle or ring, this value can be used to apply padding. The padding is applied at the outer radius and then projected inwards to the inner radius so that the edges of the gap between two segments are parallel. The results may look strange when applying padding to circle segments rather than ring segments (default is 0 - i.e. no padding) |
| start_angle | `cdv::radians` | the angle at which the arc starts (default is 0 radians) |




Arcs are particularly useful when drawing pie or donut charts. They can be generated conveniently
using [pie slices](#pie_slices).

As an example of an arc, this definition

```c++
const auto arc00 = elem::arc{.center = frame.center(),
                             .outer_radius = 150_px,
                             .inner_radius = 110_px,
                             .start_angle = 0_rad,
                             .end_angle = 2_rad,
                             .fill = {.color = tab::blue}};
```
<sup><a href='/tests/approval_tests/cdv/fig/pie_charts.cpp#L143-L148' title='Go to snippet source file'>source</a></sup>

generates the blue ring segment in the middle ring in the following image:

![](./../tests/approval_tests/cdv/fig/approved_files/pie_charts.pie_charts.manual_arcs.approved.svg)

Note how the above image also demonstrates an inner radius of zero (the pie in the center) and the use of the
pad angle to separate each segment slightly in the outer ring.


## scatter.hpp

Source: [cdv/elem/scatter.hpp](/include/cdv/elem/scatter.hpp)


### scatter<XRange, YRange, SizeRange>

```c++
template <class XRange, class YRange, class SizeRange>
struct cdv::elem::scatter;
```

|Field|Type|Description|
| :-- | :-- | :-- |
| properties | `cdv::elem::symbol_properties` | The properties of the symbols used in the scatter |
| sizes | `SizeRange` | The sizes of the symbols. |
| xs | `XRange` | The x coordinates of the symbols |
| ys | `YRange` | The y coordinates of the symbols |




*cdv* supports the notion of a [symbol](#symbol). There is a common kind of 
visualization - a scatter plot - where a (sometimes large) number of symbols all 
share the same properties. Like in the following visualization:

![](./../tests/approval_tests/cdv/fig/approved_files/scatter_charts.scatter_charts.simple_random_scatter.approved.svg)

This could be done in *cdv* using the `elem::symbol` 
type, but it is much more efficient to group all the symbols together in a
single *scatter* element.

See [symbol properties](#symbol_properties) for an overview of the supported symbol styles.


## symbol.hpp

Source: [cdv/elem/symbol.hpp](/include/cdv/elem/symbol.hpp)


### symbol

```c++

struct cdv::elem::symbol;
```

|Field|Type|Description|
| :-- | :-- | :-- |
| position | `cdv::pixel_pos` | __MISSING__ |
| properties | `cdv::elem::symbol_properties` | __MISSING__ |
| size | `cdv::points` | __MISSING__ |




Symbols are shapes that can be used to represent different categories and are commonly
used in scatter plots. They correspond to what *matplotlib* refers to as a *marker*.
`elem::symbol` in *cdv* represents an individual symbol. If you would like to represent
many symbols which all share the same properties, then consider using the
[scatter](#scatterhpp) type instead.

Symbols can be used in many different ways. For instance the round dots where the
branches meet the timeline in the following plot are symbols:

![](./../tests/approval_tests/cdv/fig/approved_files/fig.time_line.approved.svg)

See [symbol properties](#symbol_properties) for an overview of the supported symbol styles.



## axis.hpp

Source: [cdv/elem/axis.hpp](/include/cdv/elem/axis.hpp)


### axis<Scale>

```c++
template <typename Scale>
struct cdv::elem::axis;
```

|Field|Type|Description|
| :-- | :-- | :-- |
| orientation | `cdv::elem::axis_orientation` | The [axis orientation](#axis_orientation) governs both whether the axis is vertical or horizontal and where the ticks are placed relative to the spine of the axis. |
| position | `cdv::elem::axis::codomain_t` | For a horizontal axis, this is the vertical position of the axis. For a vertical axis, this is the horizontal position of the axis. |
| properties | `axis_properties<cdv::elem::axis::codomain_t>` | Properties governing the appearance of the scale. |
| scale | `Scale` | The scale which the axis represents. |


|Nested Typedef|Type|Description|
| :-- | :-- | :-- |
| codomain_t | `typename Scale::codomain_t` | The type that the axis scale maps to. |




`axis` is the basic template type that describes either a vertical or horizontal 
line with optional grid, tick marks and labels. These are referred to in *cdv* as
*spine*, *grid*, *ticks* and *labels* as shown here:

![](./../tests/approval_tests/cdv/elem/approved_files/axis.components.approved.svg)

Normally, it is convenient to
create an instance of `axis` using one of the functions [top axis](#top_axis),
[right axis](#right_axis), [bottom axis](#bottom_axis) or [left axis](#left_axis).

An `axis` is templated on the type of scale that it relates to and will produce
different results based on the type of scale that it uses. For instance, a
[linear scale](#./scl.md#linear_sale) will attempt to spread ticks evenly over the space available along
the axis and a [band scale](./scl.md#band_scale) will place a tick in the center of each band.


### axis_properties<Codomain>

```c++
template <typename Codomain>
struct cdv::elem::axis_properties;
```

|Field|Type|Description|
| :-- | :-- | :-- |
| grid | `cdv::elem::line_properties` | The line properties of the lines which make up the grid of an axis. The grid of an axis is made up of lines extending in the opposite direction to the ticks. |
| grid_length | `Codomain` | How far the grid lines extend. |
| num_ticks_hint | `size_t` | A hint as to the number of ticks that the axis should be marked with. Depending on the type of axis, this number will be interpreted differently. It is only to be seen as a hint and the actual number of ticks may differ. |
| spine | `cdv::elem::line_properties` | The line properties of the line which makes up the spine of the axis. |
| tick_label_offset | `cdv::pixel_pos` | The offset between the tick labels and the ticks. |
| tick_labels | `cdv::elem::text_properties` | The properties of the text used to draw the tick labels themselves. |
| tick_length | `Codomain` | The length of the tick marks. |
| ticks | `cdv::elem::line_properties` | The line properties of the tick marks. |




The `axis_properties` determine the appearance of the various components which make up an 
axis.


### top_axis

Creates a horizontal axis with ticks and labels above the spine

```c++
template <typename Scale, typename Codomain>
auto top_axis(const Scale & scale, const Codomain & position, const axis_properties<Codomain> & properties)
```

> This is a convenience function for the common case of a horizontal axis rendered at a specific vertical position where the ticks and labels of the axis are positioned above the spine of the axis.

|Argument|Description|
| :-- | :-- |
| scale | The scale that the axis relates to. |
| position | The vertical position of the axis. |
| properties | The properties of the axis. |





<br />



### right_axis

Creates a vertical axis with ticks and labels left of the spine

```c++
template <typename Scale, typename Codomain>
auto right_axis(const Scale & scale, const Codomain & position, const axis_properties<Codomain> & properties)
```

> This is a convenience function for the common case of a vertical axis rendered at a specific horizontal position where the ticks and labels of the axis are positioned right of the spine of the axis.

|Argument|Description|
| :-- | :-- |
| scale | The scale that the axis relates to. |
| position | The horizontal position of the axis. |
| properties | The properties of the axis. |





<br />



#### Enumeration: axis_orientation

```c++
enum class cdv::elem::axis_orientation
```

The four different possible orientations of a standard axis.

|Name|Description|
| :-- | :-- |
| left | Vertical axis with ticks and labels left of the spine. |
| right | Vertical axis with ticks and labels right of the spine. |
| top | Horizontal axis with ticks and labels above the spine. |
| bottom | Horizontal axis with ticks and labels below the spine. |





<br />

### bottom_axis

Creates a horizontal axis with ticks and labels below the spine

```c++
template <typename Scale, typename Codomain>
auto bottom_axis(const Scale & scale, const Codomain & position, const axis_properties<Codomain> & properties)
```

> This is a convenience function for the common case of a horizontal axis rendered at a specific vertical position where the ticks and labels of the axis are positioned below the spine of the axis.

|Argument|Description|
| :-- | :-- |
| scale | The scale that the axis relates to. |
| position | The vertical position of the axis. |
| properties | The properties of the axis. |





<br />



### left_axis

Creates a vertical axis with ticks and labels left of the spine

```c++
template <typename Scale, typename Codomain>
auto left_axis(const Scale & scale, const Codomain & position, const axis_properties<Codomain> & properties)
```

> This is a convenience function for the common case of a vertical axis rendered at a specific horizontal position where the ticks and labels of the axis are positioned left of the spine of the axis.

|Argument|Description|
| :-- | :-- |
| scale | The scale that the axis relates to. |
| position | The horizontal position of the axis. |
| properties | The properties of the axis. |





<br />



## range_stack.hpp

Source: [cdv/elem/range_stack.hpp](/include/cdv/elem/range_stack.hpp)


### range_stack<Key, Value>

```c++
template <typename Key, typename Value>
class cdv::elem::range_stack;
```

|Nested Typedef|Type|Description|
| :-- | :-- | :-- |
| layer_t | `std::vector<std::pair<Value, Value>>` | __MISSING__ |





#### Constructor: range_stack<Key, Value>

__MISSING__

**Overload 1:**

```c++
template <class RngOfRngs, class keys:auto>
range_stack<Key, Value>(const RngOfRngs & range_of_ranges, const stdx::range_of<Key> auto & keys)
```

> __MISSING__

**Overload 2:**

```c++
template <class RngOfRngs, typename InnerType, class keys:auto>
range_stack<Key, Value>(const RngOfRngs & range_of_ranges, const stdx::range_of<Key> auto & keys, const InnerType base_value)
```

> __MISSING__




<br />



#### layer

__MISSING__

```c++
const cdv::elem::range_stack::layer_t & layer(const Key & key) const
```




<br />



## text.hpp

Source: [cdv/elem/text.hpp](/include/cdv/elem/text.hpp)


### text

```c++

struct cdv::elem::text;
```

|Field|Type|Description|
| :-- | :-- | :-- |
| pos | `cdv::pixel_pos` | __MISSING__ |
| properties | `cdv::elem::text_properties` | __MISSING__ |
| rotation | `cdv::radians` | __MISSING__ |
| string | `std::string` | __MISSING__ |
| x_anchor | `cdv::elem::horizontal_anchor` | __MISSING__ |
| y_anchor | `cdv::elem::vertical_anchor` | __MISSING__ |





#### operator=

__MISSING__

```c++
cdv::elem::text & operator=(cdv::elem::text &&)
```




<br />



#### Constructor: text

__MISSING__

**Overload 1:**

```c++
text(cdv::elem::text &&)
```

> __MISSING__

**Overload 2:**

```c++
text(const cdv::elem::text &)
```

> __MISSING__




<br />



### draw

__MISSING__

```c++
template <typename Surface>
void draw(const cdv::elem::text & t, Surface & surface, const cdv::pixel_pos &)
```




<br />



