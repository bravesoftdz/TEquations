# What

The `Source` folder contains the source code of the plugin which has to be installed manually. The `Plugin` folder contains the source code of the plugin for Qt Designer which can be installed and then drag-and-dropped in the form (like a button or a label).

# IMPORTANT, PLEASE READ

I have created and tested the plugin with MinGW **5.3.0** 32bit and MSVC **2017** 64bit; if you are using a more (or less) recent version you could encounter (or not) some problems. Of course, the plugin uses the library source files and you'll have to pay attention to some configurations according to the compiler you are using:

**MSVC**. Be sure that you have C++17 enabled. In Qt 5.11 it can be enabled with `QMAKE_CXXFLAGS += /std:c++17`.

**MinGW**. Be sure that you have C++17 enabled. In Qt 5.11 it can be enabled with `CONFIG += c++1z`. In `Fraction.h` you have to change this code...

```c++
#include <optional>
#include <exception>
```

to this code...

```c++
#include <experimental/optional>
#include <stdexcept>
```

and be sure that you replace the return signature of the `toMixedFraction()` to...

`std::experimental::optional<MixedFraction>`

As you (may) know, you can create C++ programs with Qt using 2 different compilers. My default compiler is MSVC 2017 and I use it with Visual Studio and with Qt; for this reason, the library work without any problem with Qt+MSVC. MinGW instead is a different compiler and so you need to adjust some things to make it work.
