Subsets
=======================================================

### Indexed subset

Extracting and operating on an indexed subset of a `Field`
can be performed by passing a `std::vector<std::size_t>`
or anything compatible, such as a `Field<int>` into the
access operator.

```C++
#include <FMath/Core>

using FMath::Field;
using FMath::scalar;

// A Field of size N1 and an index list of size N2<N1
Field<scalar> sf1(N1);
Field<int>    index_list1(N2);

// Set the indices of the Field entries you wish to extract...
// (this can also be used to re-order a Field)

// Extract the indexed set
Field<scalar> sf_subset1 = sf1[index_list1];

// Extract a small set via an initializer list
Field<scalar> sf_subset2 = sf1[{0,3,22}];

// Operate on subsets, combining different index lists
Field<scalar> sf2(N1);
Field<scalar> sf3(N3);
Field<int>    index_list2(N2);
sf1[index_list1] = sf2[index_list1] + sf3[index_list2];
```

### Contiguous slices

`Field.slice()` takes:
- `std::size_t begin = 0`
- `std::optional<std::size_t> end = {}`
- `std::size_t stride = 1`


```C++
#include <FMath/Core>

using FMath::Field;
using FMath::scalar;

Field<scalar> sf1{1,2,3,4,5};

// Create field from strided slice of other field
Field<scalar> sf2 = sf1.slice(0, {}, 2);
// Expected contents
// sf2.size() == 3
// sf2[0] == 1
// sf2[1] == 3
// sf2[2] == 5

// Assign slice from slice
sf2.slice(0,1) = sf1.slice(2,3);
// Expected contents
// sf2[0] == 3
// sf2[1] == 4

// Resize to size `5` and set entire field to value `2`
sf2.resize(5);
sf2 = 2;
// Assign value `3` to strided slice
sf2.slice(0, {}, 2) = 3;
// Expected contents
// sf2[0] == 3
// sf2[1] == 2
// sf2[2] == 3
// sf2[3] == 2
// sf2[4] == 3
```