# fig



***



|File|Description|
| :-- | :-- |
| [render_svg.hpp](#render_svghpp) | __MISSING__ |
| [frame.hpp](#framehpp) | __MISSING__ |


## render_svg.hpp

Source: [cdv/fig/render_svg.hpp](/include/cdv/fig/render_svg.hpp)


### render_to_svg_stream

__MISSING__

**Overload 1:**

```c++
template <typename... Elements>
void render_to_svg_stream(std::ostream & os, const cdv::fig::frame_dimensions & dimensions, const Elements &... elements)
```




<br />



### render_svg

__MISSING__

**Overload 1:**

```c++
template <typename... Elements>
void render_svg(const std::string & file_name, const cdv::fig::frame_dimensions & dimensions, const Elements &... elements)
```




<br />



### render_to_svg_string

__MISSING__

**Overload 1:**

```c++
template <typename... Elements>
std::string render_to_svg_string(const cdv::fig::frame_dimensions & dimensions, const Elements &... elements)
```




<br />



## frame.hpp

Source: [cdv/fig/frame.hpp](/include/cdv/fig/frame.hpp)


### frame

```c++

class cdv::fig::frame;
```




#### dimensions

__MISSING__

**Overload 1:**

```c++
constexpr cdv::fig::frame_dimensions dimensions() const
```




<br />



#### y1

__MISSING__

**Overload 1:**

```c++
constexpr mfl::detail::quantity<mfl::pixels_tag> y1() const
```




<br />



#### x1

__MISSING__

**Overload 1:**

```c++
constexpr mfl::detail::quantity<mfl::pixels_tag> x1() const
```




<br />



#### y0

__MISSING__

**Overload 1:**

```c++
constexpr mfl::detail::quantity<mfl::pixels_tag> y0() const
```




<br />



#### x0

__MISSING__

**Overload 1:**

```c++
constexpr mfl::detail::quantity<mfl::pixels_tag> x0() const
```




<br />



#### Constructor: frame

__MISSING__

**Overload 1:**

```c++
frame()
```

> __MISSING__

**Overload 2:**

```c++
frame(const cdv::inches width, const cdv::inches height, const cdv::dots_per_inch dpi)
```

> __MISSING__

**Overload 3:**

```c++
frame(const cdv::pixels width, const cdv::pixels height)
```

> __MISSING__

**Overload 4:**

```c++
frame(const cdv::pixels width, const cdv::pixels height, const cdv::pixels margin_left, const cdv::pixels margin_right, const cdv::pixels margin_top, const cdv::pixels margin_bottom, const cdv::dots_per_inch dpi)
```

> __MISSING__




<br />



#### width

__MISSING__

**Overload 1:**

```c++
constexpr mfl::detail::quantity<mfl::pixels_tag> width() const
```




<br />



#### inner_width

__MISSING__

**Overload 1:**

```c++
constexpr mfl::detail::quantity<mfl::pixels_tag> inner_width() const
```




<br />



#### center

__MISSING__

**Overload 1:**

```c++
constexpr cdv::vec2<mfl::detail::quantity<mfl::pixels_tag>> center() const
```




<br />



#### y_center

__MISSING__

**Overload 1:**

```c++
constexpr mfl::detail::quantity<mfl::pixels_tag> y_center() const
```




<br />



#### height

__MISSING__

**Overload 1:**

```c++
constexpr mfl::detail::quantity<mfl::pixels_tag> height() const
```




<br />



#### inner_height

__MISSING__

**Overload 1:**

```c++
constexpr mfl::detail::quantity<mfl::pixels_tag> inner_height() const
```




<br />



#### x_center

__MISSING__

**Overload 1:**

```c++
constexpr mfl::detail::quantity<mfl::pixels_tag> x_center() const
```




<br />



