Basics
=======================================================

### Reductions

```C++
#include <FMath/Core>

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

Extracting and operating on an indexed subset of a `Field`

```C++
#include <FMath/Core>

// A Field of size N1 and an index list of size N2<N1
FMath::ScalarField sf1(N1);
FMath::IntField    index_list1(N2);

// Set the indices of the Field entries you wish to extract...
// (this can also be used to re-order a Field)

// Extract the indexed set
FMath::ScalarField sf_subset1 = sf1[index_list1];

// Extract a small set via an initializer list
FMath::ScalarField sf_subset2 = sf1[{0,3,22}];

// Operate on subsets, combining different index lists
FMath::ScalarField sf2(N1);
FMath::ScalarField sf3(N3);
FMath::IntField    index_list2(N2);
sf1[index_list1] = sf2[index_list1] + sf3[index_list2];
```