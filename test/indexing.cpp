#include <catch.hpp>

#include <FMath/Core>

using FMath::Field;
using FMath::scalar;

TEST_CASE( "Indexing fields, assigning and mixing slices", "[Indexing]" )
{
    // Initialize
    Field<scalar> sf1{1,2,3,4,5};
    Field<scalar> sf2{1,2,3,4,5};
    Field<scalar> sf3(5, 2);

    SECTION( "subsets" )
    {
        // Create field from subset of other field
        Field<scalar> sf_res1 = sf1[{0, 3, 4}];
        // Check
        REQUIRE( sf_res1.size() == 3 );
        REQUIRE( sf_res1[0] == Approx( 1 ) );
        REQUIRE( sf_res1[1] == Approx( 4 ) );
        REQUIRE( sf_res1[2] == Approx( 5 ) );

        // Assign subset of field from other field
        sf2[{0,2,4}] = Field<scalar>{91, 92, 93};
        // Check
        REQUIRE( sf2[0] == Approx( 91 ) );
        REQUIRE( sf2[2] == Approx( 92 ) );
        REQUIRE( sf2[4] == Approx( 93 ) );

        // Assign subset of field from expression of subsets
        sf2[{0,2,4}] = sf1[{0,1,2}]*sf3[{2,3,4}] + sf3[{2,3,4}];
        // Check
        REQUIRE( sf2[0] == Approx( 4 ) );
        REQUIRE( sf2[2] == Approx( 6 ) );
        REQUIRE( sf2[4] == Approx( 8 ) );
    }

    SECTION( "slices" )
    {
        // Create field from strided slice of other field
        Field<scalar> sf_res1 = sf1.slice(0, {}, 2);
        // Check
        REQUIRE( sf_res1.size() == 3 );
        REQUIRE( sf_res1[0] == Approx( 1 ) );
        REQUIRE( sf_res1[1] == Approx( 3 ) );
        REQUIRE( sf_res1[2] == Approx( 5 ) );

        // Assign slice from slice
        sf2.slice() = sf3.slice();
        // Check
        REQUIRE( sf2[0] == Approx( 2 ) );
        REQUIRE( sf2[1] == Approx( 2 ) );
        REQUIRE( sf2[2] == Approx( 2 ) );
        REQUIRE( sf2[3] == Approx( 2 ) );
        REQUIRE( sf2[4] == Approx( 2 ) );

        // Assign value to strided slice
        sf2.slice(0, {}, 2) = 3;
        // Check
        REQUIRE( sf2[0] == Approx( 3 ) );
        REQUIRE( sf2[1] == Approx( 2 ) );
        REQUIRE( sf2[2] == Approx( 3 ) );
        REQUIRE( sf2[3] == Approx( 2 ) );
        REQUIRE( sf2[4] == Approx( 3 ) );
    }

    SECTION( "mixture" )
    {
        // Assign subset of field from slice of other field
        sf2[{0,2,4}] = sf1.slice(2);
        // Check
        REQUIRE( sf2[0] == Approx( 3 ) );
        REQUIRE( sf2[2] == Approx( 4 ) );
        REQUIRE( sf2[4] == Approx( 5 ) );

        // Assign slice of field from subset of other field
        sf2.slice(0,1) = sf1[{1,2}];
        // Check
        REQUIRE( sf2[0] == Approx( 2 ) );
        REQUIRE( sf2[1] == Approx( 3 ) );
    }
}