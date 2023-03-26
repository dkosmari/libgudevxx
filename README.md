libgudevxx - a C++ wrapper for libgudev
=======================================

`libgudevxx` is a C++20 wrapper for `libgudev`, which is a GObject wrapper for `libudev`, which is
a library for the `udev` Linux kernel subsystem.

This library makes it safer to integrate `libgudev` into GTK+ and gtkmm applications written
in C++, by using RAII and type safety. Note that this library does not depend on glibmm,
it provides its own thin wrappers.



Examples
--------

There's no short example to embed here yet. Check out the [tests](tests) directory for
sample use cases.


Installing
----------

Prerequisites:

  - a C++ compiler
  - libgudev
  - glibmm (optional, for building examples)

If you did not obtain the code from a tarball, you may need to run the `./bootstrap`
script once, to generate the `configure` script:

    ./bootstrap

After that, you can run:

    ./configure
    make
    sudo make install

For more installation options, see [INSTALL](INSTALL) or the output of
`./configure --help`.


