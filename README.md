Kepler Equation Solver Library -- libkes
========================================

A shared/static library to solve Kepler's equation.


Description
-----------

This library provides functions for solving different forms of *Kepler's
equation* that are common when dealing with Orbital Dynamics or Celestial
Mechanics.
*Kepler's equation* relates the (physical) time to the position angle of a body
in its orbit.
In general, this equation cannot be solved in closed form, i.e. there is no
'simple' algebraic formula that immediately gives a solution.
Instead, there are hundreds of different iterative methods that solve the
problem in a numerical fashion.
The aim of this library is to provide access to a well-chosen sample of solver
methods in a **simple and uniform way**.


Features
--------

This is a list of the cases of Kepler's equation handled in this library:

* circular = for circular motion
* elliptic = for motion on an ellipse (e.g. planets in the solar system)
* parabolic = for motion on a parabola (e.g. some comets)
* hyperbolic = for motion on a hyperbola (e.g. rockets launched from Earth)


The library implements the following solver methods:

* Bisection method (a.k.a. binary search, interval halving)
* Danby-Burkardt methods of order 4 & 5 (improved N-R with quartic and quintic
  convergence rate)
* Fixed-point iteration method
* Halley's method (improved N-R with cubic convergence rate)
* Markley's non-iterative fifth-order refinement to cubic approximation
* Mikkola's direct method
* Laguerre-Conway method
* Newton-Raphson method
* Nijenhuis method (high-order N-R with different starter regions)
* Secant method
* Wegstein secant iteration method


Installation
------------

Simply run the included Makefiles.
There are two of them that let you choose to compile either a *static* or a
*shared* version of the library.
Your compiler must support C99 mode compilation (tested with gcc version 4.8.3).

## Compilation examples

1. This compiles the shared library (**libkes.so**) using the `Release' target.

        make -f Makefile.shared Release

2. This compiles the static library (**libkes.a**) using the `Debug' target.

        make -f Makefile.static Debug


## How to use the library

The library consists basically of two files: a header file (**libkes.h**) and a
library file (**libkes.a** or **libkes.so**, depending on which you choose).
To use it in your own code, simply include the header in your program:

``` C
    #include <libkes.h>

    int main(void) {
        kes_show_version();
        return 0;
    }
```

Do not forget to link to the library file when compiling you program.


Documentation
-------------

A Doxygen documentation system (www.doxygen.org) generated documentation is
available in the `doc/doxygen` subdirectory of the package.

There are a number of small example programs in the `example/` subdirectory to
get you started.
Each program covers a different aspect of the library.


License
-------

This library is released under the GNU Lesser General Public License (LGPL)
Version 3.
See the files COPYING and COPYING.LESSER for details.

Created by Akos Bazso, 2011-2019. Version 2019.11
