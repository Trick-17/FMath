#include <catch.hpp>

#include <FMath/Core>

using FMath::ScalarField;
using FMath::VectorField;
using FMath::scalar;
using FMath::Vector3;

TEST_CASE( "Reductions", "[Reductions]" )
{
    int N = 200;

    ScalarField sf(N, 3);
    VectorField vf(N, {0,0,1});

    // Calculate
    scalar sf_sum   = sf.sum();
    scalar sf_mean  = sf.mean();

    // Check
    REQUIRE(sf_sum  == 3*N);
    REQUIRE(sf_mean == 3);

    // Calculate
    Vector3 vf_sum  = vf.sum();
    Vector3 vf_mean = vf.mean();

    // Check
    INFO( "vf_sum: " + std::to_string(vf_sum[0]) + " " + std::to_string(vf_sum[1]) + " " + std::to_string(vf_sum[2]) );
    REQUIRE( vf_sum.isApprox( Vector3{0, 0, N} ) );
    INFO( "vf_mean: " + std::to_string(vf_mean[0]) + " " + std::to_string(vf_mean[1]) + " " + std::to_string(vf_mean[2]) );
    REQUIRE( vf_mean.isApprox( Vector3{0, 0, 1} ) );
}