#include <catch.hpp>

#include <FMath/Core>

using FMath::Field;
using FMath::scalar;
using FMath::Vector3;

TEST_CASE( "VectorField", "[VectorField]" )
{
    // Initialize
    Field<scalar> sf1(5, 2);

    Field<Vector3> vf1(5, {0, 0,   1});
    Field<Vector3> vf2(5, {0, 0.5, 0.5});

    // Operate
    Field<scalar>  sf_res2 = vf1.dot(vf2) + sf1;
    Field<Vector3> vf_res1 = vf1 + vf2;
    Field<Vector3> vf_res2 = vf1 + vf1.cross(vf2);
    
    // Check
    REQUIRE( sf_res2[0] == Approx( 2.5 ) );

    INFO( "vf result 1: " + std::to_string(vf_res1[0][0]) + " " + std::to_string(vf_res1[0][1]) + " " + std::to_string(vf_res1[0][2]) );
    REQUIRE( vf_res1[0].isApprox(Vector3{ 0, 0.5, 1.5 }));

    INFO( "vf result 2: " + std::to_string(vf_res2[0][0]) + " " + std::to_string(vf_res2[0][1]) + " " + std::to_string(vf_res2[0][2]) );
    REQUIRE( vf_res2[0].isApprox( Vector3{-0.5, 0, 1} ) );
}