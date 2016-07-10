# Vult for Pure Data
An example of generating puredata externals with Vult

## How to use

This code has been tested with Pure Data 0.47 in Linux and OSX. In order to compile it you need `cmake` and also fixing the include path to the file `m_pd.h` in your system. Set the appropriate path with the command `include_directories()` in the `CMakeLists.txt` file.

Follow these steps to compile:
```
$ mkdir build
$ cd build
$ cmake ../
$ make
```

This will produce the file `phd~.pd_darwin` or `phd~.pd_linux`. Put this file in a place where Pure Data can find it and open the `test.pd` file.

The source of the oscillator is the file `phasedist.vult`. In order to modify or regenerate the code you need to have the development version of Vult (https://github.com/modlfo/vult).

To regenerate the code run:
```
$ ./vultc.native -ccode -template pd -o phd phasedist.vult
```

This will generate the files `phd.cpp` and `phd.h`.
