# libgudevxx - a C++ wrapper for libgudev

`libgudevxx` is a C++20 wrapper for `libgudev`, which is a GObject wrapper for `libudev`, which is
a library for the `udev` Linux kernel subsystem.

This library makes it safer to integrate `libgudev` into GTK+/GLib and gtkmm/glibmm
applications written in C++, by using RAII and type safety. Note that this library does
not depend on glibmm, it comes with its own thin wrapper to convert GLib data types to C++
types.


## Usage

The API mimics libgudev, so you should consult the libgudev documentation for reference.

The library provides 3 classes in the namespace `gudev`:

  - `gudev::Client`: allows you to obtain a device (or a list of all devices in a
    subsystem). It can also listen for the `"uevent"` signal, if there's a GLib event loop
    running. Either assign a lambda to `uevent_callback` member variable, or inherit from
    this class (overriding the `on_uevent()` method) to catch these events. This is how
    you can track device changes.

  - `gudev::Device`: holds metadata about a device. The most useful ones are name, device
    file path, properties and tags.

  - `gudev::Enumerator`: provides querying rules to obtain lists of devices.

These classes are defined in their respective headers:

```cpp
#include <gudevxx/Client.hpp>
#include <gudevxx/Device.hpp>
#include <gudevxx/Enumerator.hpp>
```

You can also just include `<gudevxx/gudevxx.hpp>` to include all headers.

A `pkg-config` script is available, `libgudevxx.pc`, that helps with compiler and linker
flags:

    g++ -std=c++20 test_prog.cpp `pkg-config --cflags --libs libgudevxx`


## Examples

There's no short example to embed here yet. Check out the [examples](examples) directory.


## Building

### Dependencies

- A C++20 compiler with an adequate C++ development environment. This is usually done by
  installing some meta package like `task-c++` or `build-essential`. In addition, you may
  need to manually install:
  - `autoconf`
  - `automake`
  - `libtool`
  - `pkg-config`

- [libgudev](http://gitlab.gnome.org/GNOME/libgudev): you will need the "devel" version of
  the package.
- [glibmm](http://gitlab.gnome.org/GNOME/glibmm): Optional, for building examples, you'll
  need the "devel" version of the package.

### Instructions

If you downloaded a release tarball, you can skip step 0.

0. `bootstrap`
1. `./configure`
2. `make`
3. `sudo make install`

For more installation options, see [INSTALL](INSTALL) or the output of `./configure
--help`.
