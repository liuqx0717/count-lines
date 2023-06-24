# Overview

Count the occurrence of the lines using a hash table. Faster than
`sort | uniq -c`.

```
USAGE:

   ./count-lines  [-s] [-r] [-c] [--] [--version] [-h]


Where:

   -s,  --sum-results
     Sum the results of previous runs.

   -r,  --reverse
     Reverse the sorting order.

   -c,  --sort-count
     Sort by count (by default sort by line content).

   --,  --ignore_rest
     Ignores the rest of the labeled arguments following this flag.

   --version
     Displays version information and exits.

   -h,  --help
     Displays usage information and exits.


   Count the occurrence of the lines using a hash table. Read from stdin.
```



# Compile

Use [CMake](https://cmake.org/) to compile.

Dependencies:

- [TCLAP](https://tclap.sourceforge.net/) (required). This is a header-only
  library for parsing command-line args.

- [asan, ubsan](https://github.com/google/sanitizers) (optional). For unit
  tests only. Note that on MSVC, only `asan` is available.

## CMake options

- `CMAKE_BUILD_TYPE` (string): select build type for single-configuration
  generators, e.g. Makefile, Ninja. Typical values include `Debug`, `Release`,
  `RelWithDebInfo` and `MinSizeRel`. For multi-configuration generators (e.g.
  Visual Studio), this option is ignored (use `--config` in this case, see the
  example below).

- `TCLAP_INCLUDE_DIR` (path): manually specify the `TCLAP` location, e.g.
  `/.../tclap-1.2.5/include`. Takes precedence over the system include path.

- `ENABLE_ASAN` (boolean, `0|1`): disable/enable `asan` and `ubsan`. By default
  disabled on older versions of MSVC, enabled in other cases. For unit tests
  only.

- `STATIC_LINK` (boolean, `0|1`): statically link all the dependencies. By
  default enalbed on MSVC, disabled in other cases.
  - On Linux, you need to install the static version of the libraries (e.g.
    `glibc-static`, `libstdc++-static`).
  - Cannot specify `-static` with `-fsanitize=address` on gcc. So `-fsanitize=
    address` won't be used when this option is enabled.

## Examples

### Linux

```
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make -j8
make test
```

### Windows

```
md build
cd build
cmake -DTCLAP_INCLUDE_DIR=D:\codes\tclap-1.2.5\include ..
cmake --build . -j8 --config Release
ctest -C Release
```

