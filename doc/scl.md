# scl

Definition of *cdv* scales

***


This module contains the various scales that can be used to create elements
in *cdv* where a scale is essentially just a mapping from some domain to a
codomain. The scale types, the terminology and the basic behavior follow 
those of [d3.js](https://github.com/d3/d3-scale) reasonably closely. One
major difference in terminology is the use of the term *range* in d3 to
denote what in *cdv* is called the *codomain*. The word range was intentionally
avoided as it has a very strong association with the concept of ranges as
abstractions built over iterators in C++.


|File|Description|
| :-- | :-- |
| [time_scale.hpp](#time_scalehpp) | __MISSING__ |
| [linear_scale.hpp](#linear_scalehpp) | __MISSING__ |
| [ordinal_scale.hpp](#ordinal_scalehpp) | __MISSING__ |
| [log_scale.hpp](#log_scalehpp) | __MISSING__ |
| [band_scale.hpp](#band_scalehpp) | Band scale and related properties |
| [sequential_scale.hpp](#sequential_scalehpp) | __MISSING__ |


## time_scale.hpp

Source: [cdv/scl/time_scale.hpp](/include/cdv/scl/time_scale.hpp)


### time_scale<Clock, Duration, Codomain>

```c++
template <typename Clock, typename Duration, typename Codomain>
class cdv::scl::time_scale;
```

|Nested Typedef|Type|Description|
| :-- | :-- | :-- |
| codomain_t | `Codomain` | __MISSING__ |
| domain_t | `std::chrono::time_point<Clock, Duration>` | __MISSING__ |





#### Constructor: time_scale<Clock, Duration, Codomain>

__MISSING__

**Overload 1:**

```c++
time_scale<Clock, Duration, Codomain>(const cdv::scl::time_scale::domain_t & x0, const cdv::scl::time_scale::domain_t & x1, const Codomain & y0, const Codomain & y1)
```

> __MISSING__

**Overload 2:**

```c++
time_scale<Clock, Duration, Codomain>(const date::year_month_day & x0, const date::year_month_day & x1, const Codomain & y0, const Codomain & y1)
```

> __MISSING__




<br />



#### ticks

__MISSING__

```c++
std::vector<domain_t> ticks(const size_t num_ticks_hint) const
```




<br />



#### operator()

__MISSING__

```c++
Codomain operator()(const cdv::scl::time_scale::domain_t & x) const
```




<br />



#### tick_formatter

__MISSING__

```c++
template <class num_ticks_hint:auto>
auto tick_formatter(const auto num_ticks_hint) const
```




<br />



#### snapped_to_grid

__MISSING__

```c++
time_scale<Clock, Duration, Codomain> snapped_to_grid(const size_t num_ticks_hint) const
```




<br />



#### domain

__MISSING__

```c++
auto domain() const
```




<br />



#### codomain

__MISSING__

```c++
auto codomain() const
```




<br />



## linear_scale.hpp

Source: [cdv/scl/linear_scale.hpp](/include/cdv/scl/linear_scale.hpp)


### linear_scale<DomainType, CodomainType>

```c++
template <class DomainType, typename CodomainType>
class cdv::scl::linear_scale;
```

|Nested Typedef|Type|Description|
| :-- | :-- | :-- |
| codomain_t | `CodomainType` | __MISSING__ |
| domain_t | `DomainType` | __MISSING__ |





#### ticks

__MISSING__

```c++
std::vector<DomainType> ticks(const size_t num_ticks_hint) const
```




<br />



#### operator()

__MISSING__

```c++
CodomainType operator()(const DomainType & x) const
```




<br />



#### tick_formatter

__MISSING__

```c++
template <class num_ticks_hint:auto>
auto tick_formatter(const auto num_ticks_hint) const
```




<br />



#### snapped_to_grid

__MISSING__

```c++
linear_scale<DomainType, CodomainType> snapped_to_grid(const size_t num_ticks_hint) const
```




<br />



#### Constructor: linear_scale<DomainType, CodomainType>

__MISSING__

**Overload 1:**

```c++
linear_scale<DomainType, CodomainType>(const DomainType x0, const DomainType x1, const CodomainType y0, const CodomainType y1, const linear_scale_properties<CodomainType> & properties)
```

> __MISSING__

**Overload 2:**

```c++
template <class domain:auto, class codomain:auto>
linear_scale<DomainType, CodomainType>(const stdx::range_of<DomainType> auto & domain, const stdx::range_of<CodomainType> auto & codomain, const linear_scale_properties<CodomainType> & properties)
```

> __MISSING__




<br />



#### domain

__MISSING__

```c++
auto domain() const
```




<br />



#### codomain

__MISSING__

```c++
auto codomain() const
```




<br />



### linear_scale_properties<CodomainType>

```c++
template <typename CodomainType>
struct cdv::scl::linear_scale_properties;
```

|Field|Type|Description|
| :-- | :-- | :-- |
| clamp | `bool` | __MISSING__ |
| interpolate | `linear_interpolator<CodomainType>` | __MISSING__ |





### linear_interpolator<cdv::pixels>

```c++

struct cdv::scl::linear_interpolator;
```




#### operator()

__MISSING__

```c++
constexpr cdv::pixels operator()(const cdv::pixels a, const cdv::pixels b, const double t) const
```




<br />



### linear_interpolator<FloatingPointType>

```c++

struct cdv::scl::linear_interpolator;
```




#### operator()

__MISSING__

```c++
constexpr FloatingPointType operator()(const FloatingPointType a, const FloatingPointType b, const FloatingPointType t) const
```




<br />



## ordinal_scale.hpp

Source: [cdv/scl/ordinal_scale.hpp](/include/cdv/scl/ordinal_scale.hpp)


### ordinal_scale_properties<CodomainType>

```c++
template <typename CodomainType>
struct cdv::scl::ordinal_scale_properties;
```

|Field|Type|Description|
| :-- | :-- | :-- |
| default_result | `std::optional<CodomainType>` | __MISSING__ |





### ordinal_scale<DomainType, CodomainType>

```c++
template <typename DomainType, typename CodomainType>
class cdv::scl::ordinal_scale;
```

|Nested Typedef|Type|Description|
| :-- | :-- | :-- |
| codomain_t | `CodomainType` | __MISSING__ |
| domain_t | `DomainType` | __MISSING__ |





#### operator()

__MISSING__

**Overload 1:**

```c++
cdv::scl::ordinal_scale::codomain_t operator()(const DomainType & x)
```

> __MISSING__

**Overload 2:**

```c++
cdv::scl::ordinal_scale::codomain_t operator()(const DomainType & x) const
```

> __MISSING__




<br />



#### domain

__MISSING__

```c++
auto domain() const
```




<br />



#### Constructor: ordinal_scale<DomainType, CodomainType>

__MISSING__

```c++
template <class domain:auto, class codomain:auto>
ordinal_scale<DomainType, CodomainType>(const stdx::range_of<DomainType> auto & domain, const stdx::range_of<CodomainType> auto & codomain, const ordinal_scale_properties<CodomainType> & properties)
```




<br />



#### codomain

__MISSING__

```c++
auto codomain() const
```




<br />



### make_ordinal_scale

__MISSING__

```c++
template <typename DomainType, typename CodomainRange>
auto make_ordinal_scale(const CodomainRange & codomain, const ordinal_scale_properties<ranges::range_value_type_t<CodomainRange>> & properties)
```




<br />



## log_scale.hpp

Source: [cdv/scl/log_scale.hpp](/include/cdv/scl/log_scale.hpp)


### log_scale_properties<CodomainType>

```c++
template <typename CodomainType>
struct cdv::scl::log_scale_properties;
```

|Field|Type|Description|
| :-- | :-- | :-- |
| base | `double` | __MISSING__ |
| clamp | `bool` | __MISSING__ |
| interpolate | `linear_interpolator<CodomainType>` | __MISSING__ |





### log_scale<DomainType, CodomainType>

```c++
template <class DomainType, typename CodomainType>
class cdv::scl::log_scale;
```

|Nested Typedef|Type|Description|
| :-- | :-- | :-- |
| codomain_t | `CodomainType` | __MISSING__ |
| domain_t | `DomainType` | __MISSING__ |





#### ticks

__MISSING__

```c++
std::vector<DomainType> ticks(const size_t num_ticks_hint) const
```




<br />



#### operator()

__MISSING__

```c++
CodomainType operator()(const DomainType & x) const
```




<br />



#### tick_formatter

__MISSING__

```c++
template <class num_ticks_hint:auto>
auto tick_formatter(const auto num_ticks_hint) const
```




<br />



#### base

__MISSING__

```c++
double base() const
```




<br />



#### snapped_to_grid

__MISSING__

```c++
log_scale<DomainType, CodomainType> snapped_to_grid(const size_t num_ticks_hint) const
```




<br />



#### Constructor: log_scale<DomainType, CodomainType>

__MISSING__

**Overload 1:**

```c++
log_scale<DomainType, CodomainType>(const DomainType x0, const DomainType x1, const CodomainType y0, const CodomainType y1, const log_scale_properties<CodomainType> & properties)
```

> __MISSING__

**Overload 2:**

```c++
template <class domain:auto, class codomain:auto>
log_scale<DomainType, CodomainType>(const stdx::range_of<DomainType> auto & domain, const stdx::range_of<CodomainType> auto & codomain, const log_scale_properties<CodomainType> & properties)
```

> __MISSING__




<br />



#### domain

__MISSING__

```c++
auto domain() const
```




<br />



#### codomain

__MISSING__

```c++
auto codomain() const
```




<br />



### symlog

__MISSING__

```c++
template <class T>
T symlog(const T base, const T x)
```




<br />



## band_scale.hpp

Source: [cdv/scl/band_scale.hpp](/include/cdv/scl/band_scale.hpp)



### Notes

Band scales are useful in mapping some number of discrete objects to a continuous space such
that each object is assigned it's own unique slice (a band) of that continuous space. The
most obvious use case for band scales is in creating the bars of a bar chart. For example, in
a bar chart with vertical bars, each bar should occupy some band of space along the x-axis.

The bands are a certain distance apart and you can control the amount of spacing between bands
by specify a padding value which represents a fraction of that total distance between bands.
There is an `inner_padding` property which controls the space between bands, and there is
also an `outer_padding` property which controls the amount of spacing before the first and
after the last band. Finally, there is an `alignment_factor` property with which the amount
of outer padding assigned to beginning or the end of the bands can be controlled. The following 
diagram shows how the padding controls the spacing:

![](https://raw.githubusercontent.com/cpp-niel/cdv/master/tests/approval_tests/cdv/fig/approved_files/fig.band_scale_terminology.approved.svg)

### band_scale_properties

```c++

struct cdv::scl::band_scale_properties;
```

|Field|Type|Description|
| :-- | :-- | :-- |
| alignment_factor | `double` | determines how much of the outer padding is assigned to before the bands and how much is assigned to after the bands. A value of 0.0 will assign no outer padding to before the bands and all outer padding to after the bands. A value of 1.0 will assign all outer padding to before the bands. The default value of 0.5 distributes the padding equally to before and after the bands. |
| inner_padding | `double` | The fraction of the distance between the centers of two bands that should be spacing between bands (default 0.0) |
| outer_padding | `double` | The fraction of the distance between the centers of two bands that should be spacing before the first and after the last bands (default 0.0) |




These are the properties that control spacing in band scales. Normally these properties
will only be used when creating a band scale. For example,

```c++
const auto s = cdv::scl::band_scale(my_keys, frame.x0(), frame.x1(),
                   {.inner_padding = 0.1, outer_padding = 0.2});
```
would create a band scale where the outer padding is wider than the inner padding, but
the outer padding is the same on each end because the alignment factor was not
explicitly set and therefore retains its default value of 0.5.

#### Constructor: band_scale_properties

Compiler generated constructors

**Overload 1:**

```c++
band_scale_properties()
```

> compiler generated default constructor

**Overload 2:**

```c++
band_scale_properties(cdv::scl::band_scale_properties &&)
```

> compiler generated move constructor

**Overload 3:**

```c++
band_scale_properties(const cdv::scl::band_scale_properties &)
```

> compiler generated copy constructor




<br />



#### Destructor: ~band_scale_properties

compiler generated destructor

```c++
~band_scale_properties()
```




<br />



### band_scale<DomainType, CodomainType>

```c++
template <typename DomainType, typename CodomainType>
class cdv::scl::band_scale;
```

|Nested Typedef|Type|Description|
| :-- | :-- | :-- |
| codomain_t | `CodomainType` | A floating point (or floating point like) type which can be divided into bands. |
| domain_t | `DomainType` | A range of the type of object that is to be mapped to the bands. |






#### min

__MISSING__

```c++
cdv::scl::band_scale::codomain_t min(const DomainType & x) const
```




<br />



#### Constructor: band_scale<DomainType, CodomainType>

Band scale constructor

```c++
template <class domain:auto>
band_scale<DomainType, CodomainType>(const stdx::range_of<cdv::scl::band_scale::domain_t> auto & domain, const cdv::scl::band_scale::codomain_t & codomain_start, const cdv::scl::band_scale::codomain_t & codomain_stop, const cdv::scl::band_scale_properties & properties)
```

> Constructs a band scale that maps a range of objects of the domain type to individual bands within a continuous codomain interval

|Argument|Description|
| :-- | :-- |
| domain | The domain objects that are to be mapped to bands. |
| codomain_start | The start of the codomain interval |
| codomain_stop | The end of the codomain interval |
| properties | The band scale properties that determine the spacing around bands |





<br />



#### ticks

__MISSING__

```c++
auto ticks(const size_t) const
```




<br />



#### operator()

__MISSING__

```c++
cdv::scl::band_scale::codomain_t operator()(const DomainType & x) const
```




<br />



#### tick_formatter

__MISSING__

```c++
auto tick_formatter(const size_t) const
```




<br />



#### band_width

__MISSING__

```c++
cdv::scl::band_scale::codomain_t band_width() const
```




<br />



#### max

__MISSING__

```c++
cdv::scl::band_scale::codomain_t max(const DomainType & x) const
```




<br />



#### domain

__MISSING__

```c++
auto domain() const
```




<br />



#### codomain

__MISSING__

```c++
auto codomain() const
```




<br />



## sequential_scale.hpp

Source: [cdv/scl/sequential_scale.hpp](/include/cdv/scl/sequential_scale.hpp)


### sequential_scale<DomainType, Interpolator>

```c++
template <class DomainType, class Interpolator>
class cdv::scl::sequential_scale;
```

|Nested Typedef|Type|Description|
| :-- | :-- | :-- |
| codomain_t | `std::invoke_result_t<Interpolator, DomainType>` | __MISSING__ |
| domain_t | `DomainType` | __MISSING__ |
| interpolator_t | `Interpolator` | __MISSING__ |





#### ticks

__MISSING__

```c++
std::vector<domain_t> ticks(const size_t num_ticks_hint) const
```




<br />



#### operator()

__MISSING__

```c++
constexpr auto operator()(const cdv::scl::sequential_scale::domain_t x) const
```




<br />



#### tick_formatter

__MISSING__

```c++
template <class num_ticks_hint:auto>
auto tick_formatter(const auto num_ticks_hint) const
```




<br />



#### Constructor: sequential_scale<DomainType, Interpolator>

__MISSING__

```c++
sequential_scale<DomainType, Interpolator>(const cdv::scl::sequential_scale::domain_t start, const cdv::scl::sequential_scale::domain_t stop, const cdv::scl::sequential_scale::interpolator_t & interpolator)
```




<br />



#### domain

__MISSING__

```c++
constexpr auto domain() const
```




<br />



