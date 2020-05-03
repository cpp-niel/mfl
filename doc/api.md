# API

In essence the API of *mfl* is a single function `mfl::layout` with the following signature:

```c++
layout_elements layout(const std::string_view input, const font_face_creator& create_font_face);
```

In order to call it, you must include the header `mfl/layout.hpp`.

`layout` takes two arguments:
- `input`: this is the formula to create the layout for. The syntax is very similar to that
used by TeX ([Supported Features](features.md)).
- `create_font_face`: a function that can create a font face of a given font family. The font
face provides information about the fonts that is required to create the layout. More on this [in
a moment](#fonts).

and it returns a single argument of type `mfl::layout_elements` which, if all goes well, will
contain a vector of `mfl::line`s and a vector of `mfl::shaped_glyph`s. `mfl::layout_elements`
also has an `error` member which is an optional string that will be set if there is an
error. For example, a syntax error in the input formula string.

An API containing a single function may seem nice and simple. Unfortunately, the layout of
mathematical formulas is heavily dependent on the fonts that are used and that second
argument is not simple to provide.

## Fonts

In order to be able to call `mfl::layout` the caller must be able to provide a factory 
function. When the factory function is called by *mfl* with a given font family, the
function must return a `unique_ptr`. This pointer should point to an object of a type 
that derives from `abstract_font_face`. You can see this in the definition of the
`font_face_creator` type (i.e. the type of the second argument to `mfl::layout`):

```c++
using font_face_creator = std::function<std::unique_ptr<abstract_font_face>(const font_family)>;
```

Here we can see that it is a function that takes a font family as its only argument and returns
a `std::unique_ptr<abstract_font_face>`. Internally, the faces are cached so you do not need
to worry about redundant faces for a given family being instantiated repeatedly.

In order to provide such a factory function, we need to be able to instantiate font face
objects.

### Font Faces

*mfl* supports a handful of so called font families which are defined in the enumeration
`mfl::font_family`. For each of these font families, users of the library must be able
to supply a font face which describes layout relevant properties of both the font in 
general and of specific glyphs from the font. This is all information that can, for instance, 
be found in open type fonts with support for mathematical type setting. All lengths/sizes are
expected to be in 26.6 fixed point representation - i.e. 64ths of a pixel. As the name suggests,
`abstract_font_face` is an abstract base class.  See the `font_face` type in the
`fonts_for_tests` project for an example of how to implement this. Subclasses must 
implement several member functions which we will discuss briefly here.

- `math_constants constants() const`: this is the function that provides layout information
  for the entire face. The `mfl::math_constants` type is an aggregation of many values
  which are needed when generating a layout for mathematical formulas. If you are interested
  in their application, there is a lot of information in the 
  [Microsoft Open Type documentation](https://docs.microsoft.com/en-us/typography/opentype/spec/math).
  The simplest way of filling this data structure is to use a third party library like
  [FreeType](https://www.freetype.org) and/or [HarfBuzz](http://harfbuzz.org/). This is
  the approach taken in the tests in this repository.
- `size_t glyph_index_from_code_point(const code_point char_code, const bool use_large_variant) const`:
  *mfl* differentiates between `glyph_index`
  and `code_point` when identifying glyphs. A `code_point` is a UCS4 unicode code point
  whereas a `glyph_index` is a user-defined identifier for a glyph inside a font. Typically,
  the index of the glyph in the font is a good choice.
- `math_glyph_info glyph_info(const size_t glyph_index) const`: this function supplies
  the information that varies from glyph to glyph. Mostly sizing information. Again, the 
  easiest way of accessing this information is to use a third party font handling library.
- `std::vector<size_variant> vertical_size_variants(const code_point char_code) const`:
  returns glyph indices and corresponding sizes for characters that have variable heights
  such as braces or parentheses.
- `std::vector<size_variant> horizontal_size_variants(const code_point char_code) const`:
  returns glyph indices and corresponding sizes for characters that have variable widths
  such as wide accents.
- `set_size`: sets the size of the font.


