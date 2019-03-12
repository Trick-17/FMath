#define NONIUS_RUNNER

#include "nonius/nonius_single.h++"

#include <FMath/Core>

using FMath::Field;
using FMath::scalar;
using FMath::Vector3;

static const int N = 1'000'000;

NONIUS_BENCHMARK("Basic implementation", []{
    std::vector<int> relative_indices({ -1, +1 });
    Field<Vector3> orientations(N, Vector3{ 0, 0, 1 });
    Field<Vector3> intermediate(N, Vector3{ 0, 0, 0 });
    Field<scalar> res(N, 0.0);

    for( int idx=0; idx<N; ++idx )
    {
        if( idx > 0 && idx < orientations.size() - 1 )
        {
            for( auto & rel : relative_indices )
            {
                intermediate[idx] += orientations[idx + rel];
            }
        }
        else if( idx == 0 )
            intermediate[idx] += orientations[idx + 1];
        else if( idx == orientations.size() - 1 )
            intermediate[idx] += orientations[idx - 1];

        res[idx] = orientations[idx].dot(intermediate[idx]);
    }

    return res;
})

NONIUS_BENCHMARK("Lambda implementation", []{
    std::vector<int> relative_indices({ -1, +1 });
    Field<Vector3> orientations(N, Vector3{ 0, 0, 1 });
    Field<Vector3> intermediate(N, Vector3{ 0, 0, 0 });
    Field<scalar> res(N, 0.0);

    auto lambda = [&](std::size_t idx, Vector3 & val) {
        if( idx > 0 && idx < orientations.size() - 1 )
        {
            for( auto & rel : relative_indices )
            {
                val += orientations[idx + rel];
            }
        }
        else if( idx == 0 )
            val += orientations[idx + 1];
        else if( idx == orientations.size() - 1 )
            val += orientations[idx - 1];
    };

    res = orientations.dot(intermediate.applied_lambda(lambda));
    return res;
})


NONIUS_BENCHMARK("Mixed implementation", []{
    std::vector<int> relative_indices({ -1, +1 });
    Field<Vector3> orientations(N, Vector3{ 0, 0, 1 });
    Field<Vector3> intermediate(N, Vector3{ 0, 0, 0 });
    Field<scalar> res(N, 0.0);

    auto lambda = [&](std::size_t idx, Vector3 & val) {
        if( idx > 0 && idx < orientations.size() - 1 )
        {
            for( auto & rel : relative_indices )
            {
                val += orientations[idx + rel];
            }
        }
        else if( idx == 0 )
            val += orientations[idx + 1];
        else if( idx == orientations.size() - 1 )
            val += orientations[idx - 1];
    };

    for( int idx=0; idx<N; ++idx )
    {
        lambda(idx, intermediate[idx]);
        res[idx] = orientations[idx].dot(intermediate[idx]);
    }

    return res;
})