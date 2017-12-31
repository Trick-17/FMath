FMath: Field Math library
=========================
**Efficient and expressive use of abstract Fields**<br />

&nbsp;

**Requirements:**
- C++17
- (later maybe OpenMP 4.5)
- (later maybe CUDA 8)


General Ideas
-------------

The library revolves around the `Field` template class, which is a convenience
wrapper around `std::vector` with added operators and functions. The Eigen library
is used for vector operations.

Expression templates ensure that mathematical operations are used efficiently,
avoiding temporaries.

The library provides convenience `typedef`s:
- `FMath::scalar`      = `double` or `float`, depending on your choice
- `FMath::ScalarField` = `FMath::Field<FMath::scalar>`
- `FMath::Vector3`     = `Eigen::Vector3<FMath::scalar>`
- `FMath::VectorX`     = `Eigen::VectorX<FMath::scalar>`
- `FMath::VectorField` = `FMath::Field<FMath::Vector3>`


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
```C++
FMath::ScalarField sf(N);
FMath::VectorField vf(N);

// Copy a scalar field to a new N-dimensional vector
FMath::VectorX vec1 = sf.asVectorX();
// Copy a vector field to a new 3N-dimensional vector
FMath::VectorX vec2 = vf.asVectorX();
// Interpret a scalar field as a N-dimensional vector without copying
Eigen::Ref<VectorX> vecRef = sf.asVectorXRef();
```