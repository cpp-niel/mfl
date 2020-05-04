# mfl

## mathematical formula layout

[![GitHub build](https://github.com/cpp-niel/mfl/workflows/continuous-integration/badge.svg)](https://github.com/cpp-niel/mfl/workflows/continuous-integration/badge.svg)
[![Travis build](https://travis-ci.org/cpp-niel/mfl.svg?branch=master)](https://travis-ci.org/cpp-niel/mfl)
[![Coverage Status](https://coveralls.io/repos/github/cpp-niel/mfl/badge.svg?branch=master)](https://coveralls.io/github/cpp-niel/mfl?branch=master)
[![Codacy Badge](https://api.codacy.com/project/badge/Grade/7c69b659287948279110d52066f1d9d5)](https://www.codacy.com/manual/cpp-niel/mfl?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=cpp-niel/mfl&amp;utm_campaign=Badge_Grade)
[![GitHub license](https://img.shields.io/badge/license-MIT-blue.svg)](https://github.com/cpp-niel/mfl/blob/master/LICENSE)

#### A C++ library which computes the layout information for mathematical formulas provided in TeX-like syntax.

Given appropriate font information *mfl* can take a formula given in TeX syntax, like this:

`f(a) = \frac{1}{2\pi i} \int_\gamma \frac{f(z)}{z-a} \, dz`

and provide all the sizing and positioning details for the lines and glyphs that are
needed to render the formula like this:

![](./tests/approval_tests/approved_files/docs.cauchy_integral.approved.svg)

## Contents

-   [Design goals](#design-goals)
-   [License](#license)
-   [Contact](#contact)
-   [Thanks](#thanks)

## Design Goals

Turning a formula from a text representation (like the TeX input language) into beautifully 
laid out lines and glyphs on some output device can be viewed as a four stage process:
1. parse the input text and generate an abstract representation of the formula
2. process the abstract formula representation, for example adding implicit spacing around
operators
3. transform the abstract representation into a set of renderable elements with specific
sizes and positions
4. render each element to the output device

*mfl* deals only with stages 1 to 3. It essentially offers a single function. The arguments
of this function are the textual representation of the formula and information about the
fonts that are to be used. The result of the function is a set of renderable elements. In
particular, *mfl* will provide exact information on what needs to be rendered where, but
does not have any support for actually rendering to output devices.

*mfl* attempts to support a useful subset of what is available to TeX users and to perform
layout in a manner that is visually pleasing and familiar. The documentation provides more
details on what is supported and how to use *mfl*.

### Non goals

-   *mfl* does not aim to support everything available in some specific existing formula 
    layout engine
-   *mfl* does not aim to generate identical layouts to those generated by any specific 
    existing formula layout engine

## License

<img align="right" src="http://opensource.org/trademarks/opensource/OSI-Approved-License-100x137.png">

The library is licensed under the [MIT License](http://opensource.org/licenses/MIT):

Copyright &copy; 2020 Niel Waldren

Permission is hereby granted, free of charge, to any person obtaining a copy of this software 
and associated documentation files (the “Software”), to deal in the Software without 
restriction, including without limitation the rights to use, copy, modify, merge, publish, 
distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom 
the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or 
substantial portions of the Software.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND 
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, 
DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

* * *

The library contains the UTF-8 Decoder from Bjoern Hoehrmann (with some minor C to C++ modifications)
which is licensed under the [MIT License](http://opensource.org/licenses/MIT) (see above).
Copyright &copy; 2008-2009 [Björn Hoehrmann](http://bjoern.hoehrmann.de/)

## Contact

Please direct all issues, questions and feedback to GitHub Issues for this repository.

## Thanks

The layout part of this library attempts to emulate the original TeX 
layout algorithms reasonably closely. This is made possible by the work of Reinhold Heckmann and 
Reinhard Wilhelm published in the paper [**A Functional Description of TeX's Formula Layout**](https://www.rw.cdl.uni-saarland.de/people/heckmann/private/abstracts/neuform-review.html)
where they dissect the original algorithms and provide a very digestible overview of
the principles behind the layout of most formula elements.

### Third party libraries, fonts, tools and services

Many thanks to the authors and contributors of the following third party libraries, fonts
tools and services with which *mfl* is developed, built and tested:
-   [**range-v3**](https://github.com/ericniebler/range-v3) for extended range support
-   [**fmt**](https://github.com/fmtlib/fmt) for text formatting
-   [**Clang**](https://clang.llvm.org) for compilation, standard library, static analysis, sanitizers and code formatting
-   [**GCC**](https://gcc.gnu.org) for compilation, standard library
-   [**CMake**](https://cmake.org) for build automation
-   [**vcpkg**](https://github.com/Microsoft/vcpkg/) for dependency management
-   [**github**](https://github.com) for project hosting and continuous integration
-   [**cpp_starter_project**](https://github.com/lefticus/cpp_starter_project) for project configuration guidance

The following are used during testing:
-   [**doctest**](https://github.com/onqtam/doctest) for unit testing
-   [**Approval Tests for C++**](https://github.com/approvals/ApprovalTests.cpp) for approval testing
-   [**libFuzzer**](http://llvm.org/docs/LibFuzzer.html) for fuzz testing
-   [**stixfonts**](https://github.com/stipub/stixfonts) for the Open Type fonts used to test the library
-   [**DejaVuFonts**](https://dejavu-fonts.github.io) for the monospace font used in approval testing
-   [**HarfBuzz**](http://harfbuzz.org/) for access to extended font information when testing the library
-   [**FreeType**](https://www.freetype.org) for font reading when testing the library
-   [**cairo**](https://www.cairographics.org) for the rendering of svgs when testing the library
