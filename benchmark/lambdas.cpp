#define NONIUS_RUNNER

#include "nonius/nonius_single.h++"

#include <FMath/Core>

using FMath::Field;
using FMath::scalar;
using FMath::Vector3;

static const int N = 100'000;

std::vector<int> relative_indices({ -1, +1 });
Field<Vector3> orientations(N, Vector3{ 0, 0, 1 });
Field<Vector3> intermediate(N, Vector3{ 0, 0, 0 });
Field<scalar> sf(N, 0.0);
Field<scalar> res(N, 0.0);

//////////////////////////////////////////////////////////////////////

auto lambda_large = [&](const std::size_t idx, const Vector3 & val) -> Vector3
{
    Vector3 tmp = val;
    if( idx > 0 && idx < orientations.size() - 1 )
    {
        for( auto & rel : relative_indices )
        {
            tmp += orientations[idx + rel];
        }
    }
    else if( idx == 0 )
        tmp += orientations[idx + 1];
    else if( idx == orientations.size() - 1 )
        tmp += orientations[idx - 1];
    return tmp;
};

NONIUS_BENCHMARK("Large basic", []{
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

NONIUS_BENCHMARK("Large lambda", []{
    res = orientations.dot(intermediate.applied_lambda(lambda_large));
    return res;
})

NONIUS_BENCHMARK("Large mixed", []{
    for( int idx=0; idx<N; ++idx )
    {
        lambda_large(idx, intermediate[idx]);
        res[idx] = orientations[idx].dot(intermediate[idx]);
    }
    return res;
})

//////////////////////////////////////////////////////////////////////

auto lambda_medium = [&](const std::size_t idx, const Vector3 & val) -> Vector3
{
    Vector3 tmp = val;
    if( idx > 0 && idx < intermediate.size() )
    {
        tmp += intermediate[idx-1];
        tmp += intermediate[idx+1];
    }
    return tmp;
};

NONIUS_BENCHMARK("Medium basic", []{
    for( int idx=0; idx<N; ++idx )
    {
        if( idx > 0 && idx < intermediate.size() )
        {
            orientations[idx] += intermediate[idx-1];
            orientations[idx] += intermediate[idx+1];
        }
    }
})

NONIUS_BENCHMARK("Medium lambda", []{
    orientations.apply_lambda(lambda_medium);
})

NONIUS_BENCHMARK("Medium mixed", []{
    for( int idx=0; idx<N; ++idx )
    {
        orientations[idx] = lambda_medium(idx, intermediate[idx]);
    }
})

//////////////////////////////////////////////////////////////////////

NONIUS_BENCHMARK("Minimal basic", []{
    for( int idx=0; idx<N; ++idx )
    {
        res[idx] = sf[idx];
    }
})

auto lambda_minimal = [&](const std::size_t idx, const scalar & val) -> scalar
{
    return sf[idx];
};

NONIUS_BENCHMARK("Minimal lambda", []{
    res.apply_lambda(lambda_minimal);
})

NONIUS_BENCHMARK("Minimal mixed", []{
    for( int idx=0; idx<N; ++idx )
    {
        res[idx] = lambda_minimal(idx, sf[idx]);
    }
})