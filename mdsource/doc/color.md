# color



***



|File|Description|
| :-- | :-- |
| [generated_schemes.hpp](#generated_schemeshpp) | __MISSING__ |


## generated_schemes.hpp

Source: [cdv/core/color/generated_schemes.hpp](/include/cdv/core/color/generated_schemes.hpp)


### from_interpolator

Creates a color scheme from a color interpolator

```c++
template <size_t N, typename Interpolator>
constexpr auto from_interpolator(const Interpolator & interp)
```

> Takes a number of entries as a template argument and a color interpolator as a function argument and returns a color scheme (an array of colors) with the corresponding number of colors taken from the interpolator.

|Argument|Description|
| :-- | :-- |
| interp | the interpolator to generate the scheme from |





<br />



