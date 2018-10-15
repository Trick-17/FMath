#include <catch.hpp>

#include <FMath/Core>

using FMath::Field;
using FMath::scalar;

TEST_CASE( "ScalarField", "[ScalarField]" )
{
    // Initialize
    Field<scalar> sf1({1,2,3,4,5});
    Field<scalar> sf2({1,2,3,4,5});
    Field<scalar> sf3(5, 1);

    // Assign
    sf3 = 2;

    // Operate
    Field<scalar> sf_res1 = sf1 + sf2 * sf3 + 2;

    // Check
    REQUIRE( sf_res1[0] == Approx( 5 ) );
    REQUIRE( sf_res1[4] == Approx( 17 ) );
}