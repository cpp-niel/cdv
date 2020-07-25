# core

Core components of the *cdv* library

***
|Module|Description|
| :-- | :-- |
| [color](./color.md) |  |




This module includes the `rgba_color` class listed here. But it also includes
the units like `pixels` and `points` which are actually just imported from
the [mfl](https://github.com/cpp-niel/mfl) library.

Furthermore, at the time of writing, C++20 calendar and date support are not yet
implemented in the major C++ compiles. So the `core` module also contains
Howard Hinnant's [date](https://github.com/HowardHinnant/date) library in the
`cdv::date` namespace. Hopefully, this can be replaced with the standardized
features in the near future.

Finally, the core module contains the color schemes and interpolators that
can be conveniently combined with *cdv* scales to select colors when
creating elements. The [cdv github repository](https://github.com/cpp-niel/cdv/doc/colors.md)
contains more information on schemes and interpolators.



|File|Description|
| :-- | :-- |
| [rgba_color.hpp](#rgba_colorhpp) | Contains the `rgba_color` class and related operators |


## rgba_color.hpp

Source: [cdv/core/rgba_color.hpp](/include/cdv/core/rgba_color.hpp)



In addition to the `rgba_color` class, this header also provides explicitly
named color constants that are often more convenient to use than manually
instantiating `rgba_color` objects with hex colors or rgba double values.
At the moment there are two namespaces containing named color constants. The
first is `cdv::css4` which contains all the 
[140 named colors](https://www.w3schools.com/colors/colors_names.asp)
defined in the css4 specification. So for instance, in this code:

```c++
constexpr auto col = cdv::css4::dodgerblue;
```

`col` is now an instance of `cdv::rgba_color` that represents the `#1E90FF`
blue color defined in the css4 specification as `DodgerBlue`. Note, that
the names are all lower case in *cdv*.

The second namespace containing explicit color names is `cdv::tab` which contains
the ten colors of the [New Tableau 10]() palette. Just like with the `css4` 
colors, you can use them anywhere where an `rgba_color` instance is required.
For example, this would give you an instance of `rgba_color` representing
the Tableau 10 green color:

```c++
constexpr auto col = cdv::tab::green;
```

Note that there are also `tableau10` and `original_tableau10` color schemes.

### rgba_color

```c++

class cdv::rgba_color;
```



### Notes

The rgba_color class is essentially a wrapper around a 32 bit
unsigned integer that represents the four bytes of an rgba
color.

This class is likely to be replaced with something more substantial
in the future, so it is highly recommended to try to stick to
explicitly named colors, color schemes and color interpolators
rather than using this class directly. At least wherever possible.


#### with_alpha

Get a copy of the color with modified alpha

**Overload 1:**

```c++
constexpr cdv::rgba_color with_alpha(double a) const
```

|Argument|Description|
| :-- | :-- |
| a | The alpha value that the returned copy should have. This value should be in the range [0, 1] where 0 represents 0% opacity (fully transparent), and 1 represents 100% opacity (fully opaque).        
 |




## Example

```c++
const auto color = cdv::css4::black.with_alpha(0.5);
assert(color.alpha == 0.5);
```


<br />



#### Constructor: rgba_color

Constructs an `rgba_color` instance.

**Overload 1:**

```c++
explicit rgba_color(const std::uint32_t value)
```

> Constructs an rgba_color instance from a 32 bit unsigned integer.

|Argument|Description|
| :-- | :-- |
| value | An unsigned 32 bit integer where each byte represents one of the red, green,  blue and alpha components of the color.
 |


**Overload 2:**

```c++
explicit rgba_color(std::string_view hex_string)
```

> Constructs an rgba_color instance from a string representation of a hex color.

|Argument|Description|
| :-- | :-- |
| hex_string | A string representation of a color. See notes below for what constitutes a valid string. If the input string is not a valid representation of a color, an exception will be thrown.
 |


**Overload 3:**

```c++
rgba_color()
```

> Constructs an rgba_color instance representing fully opaque black

**Overload 4:**

```c++
rgba_color(cdv::rgba_color &&)
```

> Compiler generated move constructor

**Overload 5:**

```c++
rgba_color(const cdv::rgba_color &)
```

> Compiler generated copy constructor

**Overload 6:**

```c++
rgba_color(const double r, const double g, const double b)
```

> Constructs a color from individual red, green and blue values. An exception will be thrown if the passed in values are not in [0, 1]. The alpha component is set to fully opaque.


|Argument|Description|
| :-- | :-- |
| r | The red component as a double value in [0, 1] |
| g | The green component as a double value in [0, 1] |
| b | The blue component as a double value in [0, 1] |


**Overload 7:**

```c++
rgba_color(const double r, const double g, const double b, const double a)
```

> Constructs a color from individual red, green, blue and alpha values. An exception will be thrown if the passed in values are not in [0, 1].


|Argument|Description|
| :-- | :-- |
| r | The red component as a double value in [0, 1] |
| g | The green component as a double value in [0, 1] |
| b | The blue component as a double value in [0, 1] |
| a | The alpha component as a double value in [0, 1] |




### Notes

In order for a string to be accepted as a valid representation of a color, it must be one of
the following:

- A string containing a hex representation of the color. The string must begin
   with the `#` symbol and must be followed by exactly six or eight hexadecimal
   characters. In the six character case, the string is treated just like an
   html color specification with each pair of characters representing the red,
   green and blue bytes respectively. In this case alpha is set to fully opaque.
   In the eight character case, the final pair of characters represents the alpha
   value. So a semi-transparent green color might be defined as `"#00ff0080"`.
- One of the matplotlib style single character colors, `"k"`, `"w"`, `"r"`, 
    `"g"`, `"b"`, `"c"`, `"y"`, `"m"`, representing black, white, red, green,
    blue, cyan, yellow and magenta respectively.
- A CSS4 color name. E.g. `"DodgerBlue"`. The comparison is case insensitive.
- The name of a tableau color with prepended `tab:`. Eg. `"tab:blue"`. The
    comparison is case insensitive.


<br />



#### as_uint32

Returns a 32 bit unsigned integer representation of the color

**Overload 1:**

```c++
constexpr std::uint32_t as_uint32() const
```

> Returns the color as a 32 bit integer. This is a very cheap operation because internally, the color is stored as a 32 bit unsigned integer.





<br />



#### Destructor: ~rgba_color

Compiler generated destructor

**Overload 1:**

```c++
~rgba_color()
```




<br />



#### alpha

Returns the alpha value

**Overload 1:**

```c++
constexpr double alpha() const
```

> Returns the alpha value which is a value in [0, 1] which represents the opacity of the color. A value of 0 represents 0% percent opacity (fully transparent) and a value of 1 represents 100% opacity (fully opaqe)





<br />



#### as_doubles

Converts the components of the color to four double values

**Overload 1:**

```c++
constexpr std::array<double, 4> as_doubles() const
```

> Returns a four element array containing each of the red, green, blue and alpha components of the color as double values in [0, 1].





<br />



