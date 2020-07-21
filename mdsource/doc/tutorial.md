# Tutorial

The [Getting Started](/README.md#getting-started) section on the main readme page gives a very
brief overview of a simple chart plotted using *cdv*. This tutorial, on the other hand, is
designed to go from the very basics to more complex visualizations and provide some
explanations of what is actually going on along the way. If you are interested in using
*cdv*, this is the place to start.

toc

## Hello, world

In this very first lesson we will already get to see the
cornerstones of a typical *cdv* program. The goal is to use *cdv* to generate a 
`std::string` instance that contains the svg code to render the text "Hello, world"
in large letters in the center of the page. That string can then - for instance - 
be written to the terminal.  

(Quick side note: if you follow the code links in this tutorial, you will see that the
svg strings in the original source are not being written to the terminal. They are being 
passed to the *approval test framework* which is checking that there have been no code
changes that have unintentionally caused the string to change. In other words, we are
using the svg strings to try to detect regressions. If you are interested 
in approval testing and/or the framework that *cdv* is using then you can find more information
on the [ApprovalTests.cpp github page](https://github.com/approvals/ApprovalTests.cpp)).

The basic structure of our program will be as follows:

```c++
#include <iostream>

int main() 
{
    const auto svg = /* Some cdv code to generate the string */
    std:: cout << svg << std::endl;
}
```

Before we can start using the *cdv* API, we need
to include a few headers (this will hopefully be replaced by module imports in the
not-too-distant future).

mdinject: tutorial-hello-world-include

With these headers included we can write the *cdv* code to generate our string:

mdinject: tutorial-hello-world

The first thing we need to do is create a [frame](/doc/frames.md). For now, we will use a
default constructed frame which defines a 640x480 drawing area. *cdv* makes extensive use
of defaults in order to allow users to configure a large variety of different aspects of
a visualization without having to always specify them all.

The next line creates a *cdv element* of type `elem::text`. Text elements require the 
string to render and the position to render the string to. They also accept a 
`text_properties` argument which could be omitted entirely, in which case it would defer
to default properties. However, in our case the default font size is too small, so we set
the font size using C++20's designated initialization. This allows us both to specify only the
font size (and none of the other text properties which can retain their default values)
and also to be able to see clearly at the call site which properties we are actually 
setting. Designated initializers are one of the key ways that *cdv* makes use of defaults.

There is one more thing to mention about the font size. As is typically the case, font 
sizes in *cdv* are specified in *points*. `points` in *cdv* is a so-called unit type and
cannot be inadvertently used where some other unit is expected. That is why we have to
explicitly instantiate a `points` object when specifying the font size. There are also other units
in *cdv* such as `pixels` and `dots_per_inch` and there will be more to say about units and
how we specify them in the next section.

In the final line we pass the elements that we have created (in this case just our message)
to the `render_to_svg_string` function along with the dimensions of the frame. This function
returns our svg code in a `std::string` instance. 

One last thing to note is that everything in the *cdv* API resides in the `cdv` namespace.

This is our result:

![](./../tests/approval_tests/cdv/fig/approved_files/tutorial.hello_world.approved.svg)

## Units

Say we wanted to write our text to some other arbitrary position - let's just choose
the coordinates (200, 300) - then surely all we have to do is replace `frame.center()` in the
creation of our text element with those coordinates? And, yes, that is all we have to do. But 
similarly to the font size in the first lesson, coordinates in *cdv* are strongly typed. 
Again, this is a good thing that protects against having some value in points for instance, 
and passing that value to a function that expects coordinates.

The unit of coordinates in *cdv* is *pixels*. So in addition to the `cdv::points` type
that we saw in the first lesson, there is also a `cdv::pixels` type. This means that the 
coordinates that we need to provide are `(cdv::pixels{200}, cdv::pixels{300})`. Now that's 
already beginning to look a bit cumbersome. Fortunately, *cdv* makes use of user defined 
literals which allow us to write `(200_px, 300_px)`. The user defined literal for points uses
the suffix `_pt`. So instead of writing `cdv::points{36}` as we did in the first section
of the tutorial, we just write `36_pt`. In order to use the unit literals we need to
grant access to the namespace `cdv::units_literals`.

Putting it all together we end up with the following code:

mdinject: tutorial-hello-world-units

And here's the result:

![](./../tests/approval_tests/cdv/fig/approved_files/tutorial.hello_world_units.approved.svg)

*cdv* makes use of strongly typed units wherever possible. This provides a lot of protection
from unit conversion issues and also allows us to see what units are used at the call site.

## Scales and Axes

Having looked at how to position the text element in the previous section we have actually
violated a fairly important guideline in the use of *cdv*. It is very rarely a good idea
to specify positions explicitly for the simple reason that if you decide to modify your
frame you'll likely end up having to modify all the explicit positions. The guideline to
follow in order to avoid this issue is short and simple:

**Use scales to position elements**

But what is a [scale](/doc/scales.md)? In abstract terms, a scale is way of mapping some
*domain* of values to a related *codomain*. In the context of this lesson, we'll look at
a specific example of this idea in the form of a specific type of scale: the *linear scale*.

In *cdv*, scales reside in the `cdv::scl` namespace. In that namespace there is a type called
`linear_scale` that maps some floating point interval to a destination interval. This can
be useful in many ways, but one of the primary uses of linear scales is to map some
interval to positions within the frame that we are creating elements for. This way
we can create a space within which we specify our "positions" that is independent
of the actual pixel values. The scale takes care of all the transformations for us.

Conveniently, there are positions defined in a frame object that tell us where our main
drawing area starts and ends in both x and y. These are known as `x0`, `x1`, `y0` and 
`y1`. Take a look at the [frame](/doc/frames.md) documentation for more information.
We can use these values to instantiate our scales:

mdinject: tutorial-create-scales

Essentially, we
are saying that we would like to create a scale that maps the floating point interval 
[0.0, 10.0] to [x0, x1] and [y0, y1] respectively. Now - as we will see in a moment -
rather than specifying positions explicitly, we can specify them in terms of the range
[0.0, 10.0] and let the scales take care of mapping those values to the correct positions
in the frame - regardless of how the frame is actually defined.

To help us visualize this, we are going to introduce axes. Axes are elements that can
be passed to the `render` functions. They can be created with various different kinds of
scale. In particular linear scales can be used to create axes. The axis then takes care
of drawing the main *spine* of the axis and of generating and labelling *ticks*. There
are different kinds of axis depending on where the ticks should go in relation to the
spine. For instance there is a *left axis* which is a vertical axis with ticks to the
left of the spine. Here, we will also use a *bottom axis* which is a horizontal axis
with ticks below the spine. Predicatably, there are also *right* and *top* axes, but
we will not be using them here. When we create an axis we need to provide the scale that
the axis represents, but also where the axis should be drawn. Typically, a left axis
will be placed at `x0` - it is a vertical axis at the left of the page. Correspondingly,
a bottom axis will usually be placed at `y0`. That is what we are doing here:

mdinject: tutorial-create-axes

Now that we have our axes, all that remains to do is to create our text element and
then pass it - along with the axes - to the render function. 

mdinject: tutorial-scales-and-axes

Note how we no longer specify the position of the text in pixels, but using the 
values 4.0 and 7.0 from the interval [0.0, 10.0]. We apply the scales to the values
using the scales' built-in function call operator. So when we write `x(4.0)`, that
is actually a call to a function defined in the `linear_scale` class that takes
the value 4.0 as an argument and returns the position that that maps to
in our codomain. The codomain was defined in pixels as the interval [x0, x1]. You can see in the
result how the text is centered on the position (4, 7) without us having to know
where that is in pixels. This is how positions should practically always be
specified in *cdv*. 

The final result looks like this:

![](./../tests/approval_tests/cdv/fig/approved_files/tutorial.scales_and_axes.approved.svg)


## Visualizing Data

*cdv* is all about visualizing data. It's even in the name. But so far we have just been
writing text. Let's define some data:

mdinject: tutorial-line-plot-define-data

Notice how the data has conveniently been chosen to have 11 values where all values lie
in the range [0, 10]. This allows us to define our scales and axes exactly as we did in
the previous lesson:

mdinject: tutorial-line-plot-scales-and-axes

Now we get to the interesting part of this lesson. The goal here is to draw this chart:

![](./../tests/approval_tests/cdv/fig/approved_files/tutorial.line_plot.approved.svg)

where each of the 11 values in our data corresponds to the values 0, 1, ... , 10 on the
x-axis. The values themselves are represented by the height on the y-axis - basically, 
it's a typical line plot.

*cdv* has a type of element called a *line* which is created with a range of x coordinates
and a range of y coordinates. When a *cdv* line is drawn, the points defined by the
coordinates are simply connected by lines. Nothing about this is specific to charts
or functions, it is a graphical notion akin to what is often known as a *polyline*.

So what we need to do is to generate x values representing 0, 1, 2 etc. and then apply
the x scale to each of those values to get our x coordinates. Similarly, we need to
take our data values and apply the y scale to them in order to get our y coordinates.
Then we can use these x and y coordinates to generate a *cdv* line. Finally, we pass the
axes and the line to the render function.

This may sound like a lot of work, but ranges make it simple. *cdv* is designed around
ranges and without a reasonable understanding of 
[C++20 ranges](https://en.cppreference.com/w/cpp/ranges) and/or the 
[range-v3](https://github.com/ericniebler/range-v3) library which *cdv* uses you
may find *cdv* awkward to use. Let's look at how to do all the work described above
using ranges:

mdinject: tutorial-line-plot

As you can see, the heavy lifting is a single line of code. We are creating a 
`cdv::elem::line` from two ranges. The first range is the first
eleven whole numbers starting from zero. These numbers are generated by a call to
`iota`. The resulting range is then piped into `transform` using our linear scale
`x` as the argument. So we are applying the scale `x` to every element in
0, 1, ... , 10. For the y coordinates we already have our data, so we can just
pipe that straight into the transform using the scale `y`.
 
The last line, the `render` call, is the same as in the previous lessons, only
now we are passing in our line element rather than a text element.

# Colors

Data visualization often comes to life with color, so let's make a small modification
to the plot from the previous lesson and draw the line in some shade of blue. Colors 
in *cdv* are often specified using scales, which is something we'll look into one of the 
next sections. For now, we just want to set our existing line's color to blue.

You can specify colors in *cdv* as rgba values, either floating point values in [0, 1]
or as 32 bit unsigned integer values. However, usually it is both easier and more
readable to use one of the many predefined colors that are provided by *cdv*. For
example, the entire set of valid [CSS 4 colors](https://www.w3schools.com/cssref/css_colors.asp)
is provided in the `cdv::css4` namespace.

Color is a *property* of the line, and like we saw when creating a text
element earlier, lines also accept a final argument that we have so far left off. This
argument is of type `cdv::elem::line_properties` and reverts to the defaults of the 
solid black line that we saw in the previous section if not provided by the caller.

Again, as with the font size in our text properties, we only need to provide those
properties that we actually want to set. All other properties will retain their
defaults. Here, we will set the color, the line width and the line style. This is
the entire body of the program, where only the creation of the line differs
from the previous lesson:

mdinject: tutorial-colors

Our plot now looks like this:

![](./../tests/approval_tests/cdv/fig/approved_files/tutorial.line_plot_colors.approved.svg)


# Bar Charts

*cdv* has no notion of line plots, bar charts or any other kind of chart. The idea behind
the library is to provide fundamental elements from which visualizations can be composed
without restricting the users to a defined set of supported visualization types. So, what
if we want to create some common kind of chart? In the previous sections we looked at a
line plot, but that's a relatively simple case because there is an element
(`cdv::elem::line`) that represents exactly what we want to display. What about a bar
chart? There is no `cdv::elem::sequence_of_bars` that we can pass our data to. To draw
a bar chart we have to start using *cdv* in a slightly more sophisticated way. We have to
start transforming our data into elements. Let's take a look at what that means.

To keep things simple and familiar, we'll use the same data that we used for
our line plots. But for reasons that will become apparent in a moment we will no
longer generate our x values in-line. We will create them as a range named *keys*
up front, together with our other data:

mdinject: tutorial-bar-chart-define-data

The bars of a bar chart are really nothing more than a set of rectangles, and that's
exactly how we are going to define them. However, each bar should be centered on its
key. So the first bar - for instance - should be centered on zero on the x-axis. If
we tried to use the x-scale that we have been using so far, that first bar would be
centered on the y-axis! We need the x-values - the keys - to be distributed along 
the x-axis differently. In *cdv* there is a kind of scale called a *band scale* which
is very useful for this kind of task (the names and behaviors of these scales are 
kept reasonably close to those used by [D3.js](https://github.com/d3/d3-scale)).

A band scale takes some range of keys and an output interval and then splits that
output interval into equal sized bands. By default the bands fill up the available
space, but there are also parameters with which spacing can be controlled. Here, we'll
just use the defaults:

mdinject: tutorial-bar-chart-band-scale

Here we are saying: take the keys and distribute them in equal sized bands across
the interval [x0, x1].

Our y-scale can remain a linear scale and the creation of our axes just does the
right thing automatically, regardless of the kind of scale we pass in. So the
next few lines should look very familiar by now:

mdinject: tutorial-bar-chart-scale-and-axes

Now that we have our scales and axes, we can generate the actual rectangles. To
do this, we are going to take each key and its corresponding value and then use
the scales to calculate the corners of each bar. There is a *cdv* element called
`rectangle` that is defined by its corners and by default is rendered as a filled 
blue rectangle.

To compute the top and bottom of a rectangle is simple. The bottom is always
`y(0.0)` and the top is the y scale applied to the current value, `y(value)`. But 
for the left and right edges of each rectangle we can't just apply `x` to our
key. In fact, applying `x` to the key gives us the center of that key's band, which
is neither the left edge nor the right edge (incidentally, this behavior does
not replicate D3.js where applying a band scale to a key returns the start of
the band). A band scale `x` in *cdv* provides `x.min(key)` and `x.max(key)` which 
return the edges of the band for the given key.

So if we zip our keys and values together and transform the resulting range 
into rectangles, we have our bars:

mdinject: tutorial-bar-chart-create-bars

But what can we do with `bars` now? `bars` is not an element, it's a range of
elements. Can we pass a range of elements to the `render` functions? The answer
is yes, and this is a very common way of using *cdv*. In fact the render functions
will accept ranges of elements or any tuple-like thing that contains elements and
any combination of the two. So a range of ranges of elements or a range of tuples
of elements or whatever. So long as they contain elements it's all fine. So, in 
the end, our `render` call should look very familiar too:

mdinject: tutorial-bar-chart-render-bars

The chart that we end up with looks like this:

![](./../tests/approval_tests/cdv/fig/approved_files/tutorial.bar_chart.approved.svg)

Notice how creating the x-axis with a band scale has resulted in a totally different
axis, with the keys distributed evenly across the available space. And it's also worth
mentioning here that there is no reason that those keys need to be numbers. Any type
that can be represented in text using std::format (actually fmtlib at the time of
writing) can serve as a key for band scales.

## More Scales

So far we have been using scales to position elements, but scales are more general than 
that. In this lesson, we'll draw the same bar chart that we did in the previous
lesson, but we'll use a scale to give each bar its own color.

In *cdv* there is a kind of scale called an *ordinal scale* that maps elements from
one range to elements of another range. And that's exactly what we need here because
we want to map keys to colors so that we can give each bar a color according to its
key.

So our first range is our keys, and the second range is a range of colors. We could
specify the colors manually if we wanted (e.g. `std::array{css4::red, css4::blue, ...}`),
but *cdv* provides ready made color schemes out of the box which are ideal for
this kind of thing. The schemes are the same as those provided by D3 which include, for
instance the old and new tableau schemes which are very popular in matplotlib. Technically, 
they are just arrays of colors that reside in the `cdv::scheme` namespace and look like this:

![](./../tests/approval_tests/cdv/elem/approved_files/color_legend.categorical_schemes.approved.svg)

Let's say that for our bar chart we'd like to choose `pastel1`. It would appear that we
have a problem because we have eleven bars, but only nine colors. However, this is
no problem. An ordinal scale will automatically wrap around, so that in this case the
last two bars will have the same color as the first two. We create our color scale like
this:

mdinject: tutorial-bar-chart-create-color-scheme

Once we have our color scale, we can apply it to the key to get the color during the
construction of the rectangles. So by making just the following small changes:

mdinject: tutorial-bar-chart-create-colored-bars 

we end up with a more colorful result:

![](./../tests/approval_tests/cdv/fig/approved_files/tutorial.bar_chart_colors.approved.svg)

## More colors

Looking at the colors of the bars in the previous lesson, it may occur to us that the colors
appear rather random, and that it may be more effective to visualize the data such that the
colors correspond to the heights of the bars in some way.

Again, this is a job for a scale. However, ordinal scales can't help us here because they map
some countable range of things to another countable range of things. We need a scale that
can map a continuous interval to some sort of range of colors. Sounds like a perfect fit 
for a linear scale, and indeed, a linear scale could handle this just fine. We could choose 
our domain to cover the input data values - say [4.0, 10.0] and then two colors that we would like
to interpolate between and we could get it to work. But there is a far more powerful way
that allows us to map a continuous input range to complex functions that - in the case
of colors - can allow us to transition through several different colors as we move
through the input domain. This kind of scale is called a *sequential scale* and the
functions that sequential scales can map to are known as *interpolators*.

Again, like with our color schemes in the previous example, *cdv* provides a large set
of color interpolators out of the box - almost all of those provided by D3 which includes
the most popular ones used in matplotlib. Here's just a small sampling of some of
the color interpolators that may be familiar to users of either D3 or matplotlib or
other data visualization tools:
 
![](./../tests/approval_tests/cdv/elem/approved_files/color_legend.other_scales.approved.svg)

For our bar chart, we'll choose the *viridis* interpolator. With just some slight changes
to the creation of our bars we can make the colors dependent on the height of the bars.
Notice how we are now no longer passing the key to our color scale, but the value:

mdinject: tutorial-bar-chart-create-more-colored-bars

The resulting chart looks like this:

![](./../tests/approval_tests/cdv/fig/approved_files/tutorial.bar_chart_more_colors.approved.svg)


## What Now?

The goal of this tutorial is to provide a starting point for getting to know *cdv*, but it
only scratches the surface of the features that are available in the library. Hopefully the
tutorial has provided enough information for you to start experimenting with your own 
visualizations or to look through the [examples](/doc/examples.md) with a better
understanding of the code used to generate them. Thanks for taking a look.