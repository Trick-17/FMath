Basics
=======================================================

### Reductions

```C++
#include <FMath/Core>

using FMath::Field;
using FMath::scalar;
using FMath::Vector3;

// Single field reduction
int N = 10;
Field<Vector3> vf(N, Vector3{0,0,1});
Vector3 mean = vf.mean();

// N-dimensional dot-product
FMath::ScalarField sf1(N), sf2(N);
sf1 *= sf2;
FMath::scalar dot = sf1.sum();

// More efficient version of the dot product:
dot = (sf1*sf2).sum();
```

### Operators

```C++
#include <FMath/Core>

FMath::VectorField vf1(N), vf2(N);
FMath::ScalarField sf1(N);

// This will produce an expression object, due to auto
auto vf = vf1 + vf2*vf2;
// This will actually evaluate the expression
FMath::ScalarField sf_result = vf.dot(vf1);
```

### Convenience math functions

```C++
#include <FMath/Core>

FMath::VectorField vf1(N), vf2(N);

// Element-wise dot product
FMath::ScalarField sf_dot = vf1.dot(vf2);
// Element-wise cross product
FMath::VectorField vf_cross = vf1.cross(vf2);
```

### Other convenience functions

Copying or re-interpreting a `Field` as an `Eigen::VectorX`

```C++
#include <FMath/Core>

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