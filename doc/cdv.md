# cdv

Root directory of the cdv library

***
|Module|Description|
| :-- | :-- |
| [elem](./elem.md) | Definition of renderable elements and associated functionality |
| [core](./core.md) | Core components of the *cdv* library |
| [fig](./fig.md) |  |
| [fnt](./fnt.md) | Properties and enumerations related to the rendering of fonts |
| [scl](./scl.md) | Definition of *cdv* scales |




*cdv* is split into the four main modules listed here. Generally, all
four modules will be required to create a data visualization using
*cdv*. Things like units and colors which reside in the `core` module
can be found directly in the `cdv` namespace, whereas elements, scales
and figure generation code are in the `elem`, `scl` and `fig` namespaces
respectively.



