Lambdas
=======================================================

### Simple example

```C++
#include <FMath/Core>

using FMath::Field;
using FMath::scalar;

Field<scalar> sf(5, 0.0);

auto lambda = [](std::size_t i, scalar& val)
{
    // Every value of the field is set to a calculated value
    val = i+5;
};

sf.apply_lambda(lambda);

// Example entries of `sf`
// sf[0] == 5
// sf[3] == 8
```

### Complex example

This example shows a reasonably complex calculation:
1. each entry of `intermediate` should contain the sum of its "neighbours" in `orientations`
2. each entry of `res` should contain the scalar product of said sum with `orientations`

As should be, the first operation (in form of a lambda) and the second will be performed in
parallel over the elements of the result `Field`.

```C++
#include <FMath/Core>

using FMath::Field;
using FMath::scalar;
using FMath::Vector3;

int N = 5;
std::vector<int> relative_indices({-1,+1});
Field<Vector3> orientations(N, Vector3{0,0,1});
Field<Vector3> intermediate(N, Vector3{0,0,0});
Field<scalar> res(N, 0.0);

auto lambda = [&](std::size_t idx, Vector3& val)
{
    // Inside entries get doth directions
    if( idx > 0 && idx < orientations.size()-1 )
    {
        for( auto& rel : relative_indices )
        {
            val += orientations[idx+rel];
        }
    }
    // Left end: right-hand side
    else if( idx == 0 )
        val += orientations[idx+1];
    // Right end: left-hand side
    else if( idx == orientations.size()-1 )
        val += orientations[idx-1];
};

// Result: scalar field of products of vectors in `orientations` and `intermediate`
res = orientations.dot(intermediate.applied_lambda(lambda));

// Contents of result:
// res[0] == 1
// res[1] == 2
// res[2] == 2
// res[3] == 2
// res[4] == 1
```