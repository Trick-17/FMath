#include <catch.hpp>

#include <FMath/Core>

using FMath::Field;
using FMath::scalar;

TEST_CASE( "SubSet", "[SubSet]" )
{
    // Initialize
    Field<scalar> sf1({1,2,3,4,5});
    Field<scalar> sf2({1,2,3,4,5});
    Field<scalar> sf3(5, 2);

    // Operate
    Field<scalar> sf_res1 = sf1[{0, 3, 4}];
    // Check
    REQUIRE( sf_res1.size() == 3 );
    REQUIRE( sf_res1[0] == Approx( 1 ) );
    REQUIRE( sf_res1[2] == Approx( 5 ) );

    // Operate
    sf2[{0,2,4}] = Field<scalar>{91, 92, 93};
    // Check
    REQUIRE( sf2[0] == Approx( 91 ) );
    REQUIRE( sf2[2] == Approx( 92 ) );
    REQUIRE( sf2[4] == Approx( 93 ) );

    // Operate
    sf2[{0,2,4}] = sf1[{0,1,2}]*sf3[{2,3,4}] + sf3[{2,3,4}];
    // Check
    REQUIRE( sf2[0] == Approx( 4 ) );
    REQUIRE( sf2[2] == Approx( 6 ) );
    REQUIRE( sf2[4] == Approx( 8 ) );
}