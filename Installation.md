# Compilation and installation #

**Dependencies:**
  * CImg (included in the package)
  * ImageMagick

**Optional dependencies:**
| Library  | CMake flag | Enables |
|:---------|:-----------|:--------|
| Boost.Program\_options | -DWITH\_BOOST\_PROGRAM\_OPTIONS=ON/OFF | command-line interface |
| CGAL     | -DWITH\_CGAL=ON/OFF | sparse vector fields and related algorithms |
| OpenCV   | -DWITH\_OPENCV=ON/OFF | OpenCV algorithms |

**Installation:**

OptFlow uses CMake as its build system. To compile (and install) the package, type the following commands:

```
  cmake . <optional flags>   <- put your CMake flags here
  make
  sudo make install
```

Also make sure that you have recent versions of CMake and C++ compilers installed. OptFlow has been tested to compile with CMake >=2.6 and g++ >= 4.2. The last step is optional. You can also use the compiled files in the build directory.