Using
=======================================================

**Requirements:**

- C++17
- OpenMP 4.5 (optional)
- (later maybe a CUDA version which supports C++17 features)

Adding this library to your project should be trivial.
You can do it manually:

- copy the `FMath` folder into your directory of choice
- copy the `thirdparty/Eigen` folder or provide your own
- make sure the `FMath` and `Eigen` folders are in your include-directories
- optionally define `FMATH_SCALAR_TYPE`
- optionally add OpenMP compiler flags to activate parallelisation

or using CMake:

- copy the entire repository folder into your directory of choice
- TODO...