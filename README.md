FMath: Field Math library
=========================
**Efficient and expressive use of abstract Fields**<br />

&nbsp;

**Requirements:**
- C++17
- OpenMP 4.5 (optional)
- (later maybe a CUDA version which supports C++17 features)


General Ideas
-------------

The library revolves around the `Field` template class, which is a convenience
wrapper around `std::vector` with added operators and functions. The Eigen library
is used for vector operations.

Expression templates ensure that mathematical operations are used efficiently,
avoiding temporaries.

The library provides convenience `typedef`s:
- `FMath::scalar`      = default is `double`, can be defined differently by `FMATH_SCALAR_TYPE`
- `FMath::ScalarField` = `FMath::Field<FMath::scalar>`
- `FMath::Vector3`     = `Eigen::Vector3<FMath::scalar>`
- `FMath::VectorX`     = `Eigen::VectorX<FMath::scalar>`
- `FMath::VectorField` = `FMath::Field<FMath::Vector3>`


Parallelisation
---------------

Since almost all operations on `Field`s, defined in this library, are either trivially parallelizable
or typical reductions, both CPU and GPU could and should be used to speed up operations.

OpenMP 4.5 can easily be used to parallelize everything on CPU and also supports usage of devices.
However, due to the lack of unified memory abstractions, GPU parallelisation does not yet come as
naturally.


Incorporation into your project
-------------------------------

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


Usage Examples
--------------

### Reductions
```C++
// Single field reduction
FMath::VectorField vf(N);
FMath::scalar mean = vf.mean();

// N-dimensional dot-product
FMath::ScalarField sf1(N), sf2(N);
sf1 *= sf2;
FMath::scalar dot = sf1.sum();
```

### Operators
```C++
FMath::VectorField vf1(N), vf2(N);
FMath::ScalarField sf1(N);

// This will produce an expression object, due to auto
auto vf = vf1 + vf2*vf2;
// This will actually evaluate the expression
FMath::ScalarField sf_result = vf.dot(vf1);
```

### Convenience math functions
```C++
FMath::VectorField vf1(N), vf2(N);

// Element-wise dot product
FMath::ScalarField sf_dot = vf1.dot(vf2);
// Element-wise cross product
FMath::VectorField vf_cross = vf1.cross(vf2);
```

### Other convenience functions
Copying or re-interpreting a `Field` as an `Eigen::VectorX`
```C++
FMath::ScalarField sf(N);
FMath::VectorField vf(N);

// Copy a scalar field to a new N-dimensional vector
FMath::VectorX vec1 = sf.asRef<VectorX>();
// Copy a vector field to a new 3N-dimensional vector
FMath::VectorX vec2 = vf.asRef<VectorX>();

// Interpret a scalar field as a N-dimensional vector without copying
Eigen::Ref<VectorX> vecRef1 = sf.asRef<VectorX>();
// Interpret a vector field as a 3N-dimensional vector without copying
Eigen::Ref<VectorX> vecRef2 = vf.asRef<VectorX>();
```

Extracting an indexed set from a `Field`
```C++
// A Field of size N1 and an index list of size N2<N1
FMath::ScalarField sf(N1);
FMath::IntField    index_list(N2);

// Set the indices of the Field entries you wish to extract...
// (this can also be used to re-order a Field)

// Extract the indexed set
FMath::ScalarField sf_subset = sf.subset(index_list);
```