# fnt

Properties and enumerations related to the rendering of fonts

***



|File|Description|
| :-- | :-- |
| [font_weights.hpp](#font_weightshpp) | Contains the data structures for font weights, slants and stretches |
| [font_properties.hpp](#font_propertieshpp) | Properties related to the appearance of fonts |


## font_weights.hpp

Source: [cdv/fnt/font_weights.hpp](/include/cdv/fnt/font_weights.hpp)


#### Enumeration: font_weights

```c++
enum class cdv::fnt::font_weights
```

Defines the different font weights supported by *cdv*. The font weight determines how thick or thin the strokes of the font are.

|Name|Description|
| :-- | :-- |
| ultralight | The thinnest font characters |
| light | Thinner font characters |
| normal | Normal weighting - neither thin nor thick |
| medium | Very slightly thicker than normal |
| semibold | Thicker font characters |
| bold | Thick font characters |
| heavy | Very thick font characters |
| black | The thickest font characters |





<br />

#### Enumeration: font_stretches

```c++
enum class cdv::fnt::font_stretches
```

Defines the different font stretch options supported by *cdv*. The font stretch allows fonts to be rendered either narrower or wider. Some fonts have specifically designed faces with different stretches.

|Name|Description|
| :-- | :-- |
| condensed | the narrowest stretch supported by *cdv* |
| semicondensed | a narrower stretch |
| normal | no stretch |
| semiexpanded | a wider stretch |
| expanded | the widest stretcj supported by *cdv* |





<br />

#### Enumeration: font_slants

```c++
enum class cdv::fnt::font_slants
```

Defines the different font slant options supported by *cdv*

|Name|Description|
| :-- | :-- |
| normal | No slant |
| italic | Italic slant - this usually, but not necessarily - refers to fonts that have been specifically designed with italic glyphs. It is up to the font designers whether they deem a font to be italic. |
| oblique | Oblique slant - this usually, but not necessarily - refers to slanted fonts that have been generated from non-slanted fonts by skewing the glyphs. It is up to the font designers whether they deem a font to be oblique. |





<br />

## font_properties.hpp

Source: [cdv/fnt/font_properties.hpp](/include/cdv/fnt/font_properties.hpp)


### font_properties

```c++

struct cdv::fnt::font_properties;
```

|Field|Type|Description|
| :-- | :-- | :-- |
| family | `std::string` | A string representing the font family. *cdv* uses *fontconfig* internally to find the font that best matches the provided family name. Default value is "sans-serif". |
| slant | `cdv::fnt::font_slants` | How the font is slanted. Default is `fnt::font_slants::normal` - i.e. no slant. |
| stretch | `cdv::fnt::font_stretches` | How the font is stretched. Default is `fnt::font_stretches::normal`. |
| weight | `cdv::fnt::font_weights` | The weight of the font (light, bold, etc.). Default is `fnt::font_weights::normal`. |




The `font_properties` data structure contains the information that is used when selecting a font. The actual font selection uses *fontconfig* internally, so it is important that the system that *cdv* is running on supports *fontconfig*. On many platforms this should be a given. On some you may have to set an environment variable to let *fontconfig* know where to find the configuration information (e.g. on MacOS something along the lines of `FONTCONFIG_PATH=/opt/X11/lib/X11/fontconfig`).


### formatter<cdv::fnt::font_properties>

```c++

struct fmt::v6::formatter;
```




