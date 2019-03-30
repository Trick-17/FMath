#define NONIUS_RUNNER

#include "nonius/nonius_single.h++"

#include <FMath/Core>

using FMath::Field;
using FMath::scalar;

static const int N = 1'000'000;

NONIUS_BENCHMARK("Basic implementation", []{
    Field<scalar> sf1(N), sf2(N);
    scalar dot = 0;
    for( int i=0; i<N; ++i)
    {
        dot += sf1[i] * sf2[i] + sf2[i];
    }
    return dot;
})

NONIUS_BENCHMARK("Multiple assignments", []{
    Field<scalar> sf1(N), sf2(N);
    sf1 = sf1*sf2 + sf2;
    scalar dot = sf1.sum();
    return dot;
})

NONIUS_BENCHMARK("Single assignment", []{
    Field<scalar> sf1(N), sf2(N);
    scalar dot = (sf1*sf2 + sf2).sum();
    return dot;
})