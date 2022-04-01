#include <catch.hpp>

#include <FMath/Core>

using FMath::Field;
using FMath::scalar;
using FMath::Vector3;

TEST_CASE( "Reductions", "[Reductions]" )
{
    int N = 200;

    Field<scalar>  sf(N, 3);
    Field<Vector3> vf(N, {0,0,1});

    SECTION("Sum and Mean")
    {
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
        REQUIRE( vf_sum.isApprox( Vector3{0, 0, double(N)} ) );
        INFO( "vf_mean: " + std::to_string(vf_mean[0]) + " " + std::to_string(vf_mean[1]) + " " + std::to_string(vf_mean[2]) );
        REQUIRE( vf_mean.isApprox( Vector3{0, 0, 1} ) );
    }

    SECTION("Min and Max")
    {
        REQUIRE( sf.min() == Approx(3) );
        REQUIRE( sf.max() == Approx(3) );

        sf[0] = 5;

        REQUIRE( sf.min() == Approx(3) );
        REQUIRE( sf.max() == Approx(5) );

        auto minmax = sf.minmax();
        REQUIRE( minmax.first  == Approx(3) );
        REQUIRE( minmax.second == Approx(5) );
    }

    SECTION("Min and Max components")
    {
        REQUIRE( vf.min_component() == Approx(0) );
        REQUIRE( vf.max_component() == Approx(1) );

        vf[0] = {1,2,3};

        REQUIRE( vf.min_component() == Approx(0) );
        REQUIRE( vf.max_component() == Approx(3) );

        auto minmax = vf.minmax_component();
        REQUIRE( minmax.first  == Approx(0) );
        REQUIRE( minmax.second == Approx(3) );
    }
}