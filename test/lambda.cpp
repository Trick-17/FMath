#include <catch.hpp>

#include <FMath/Core>

using FMath::Field;
using FMath::scalar;
using FMath::Vector3;

TEST_CASE( "Applying a lambda to the components of a Field", "[Lambda]" )
{
    // Initialize
    Field<scalar> sf1{1,2,3,4,5};
    Field<scalar> sf2{1,2,3,4,5};
    Field<scalar> sf3(5, 2);
    Field<Vector3> vf1(1, Vector3{0,0,2});

    SECTION( "apply" )
    {
        auto lambda = [](const std::size_t i, const scalar &) -> scalar
        {
            return i + 5;
        };

        sf1.apply_lambda(lambda);
        REQUIRE( sf1[0] == Approx( 5 ) );
        REQUIRE( sf1[3] == Approx( 8 ) );
    }

    SECTION( "applied" )
    {
        auto lambda = [](const std::size_t, const scalar & val) -> scalar
        {
            return val + 1;
        };

        Field<scalar> res = sf1.applied_lambda(lambda) + sf2;

        REQUIRE( res[0] == Approx( 3 ) );
        REQUIRE( res[3] == Approx( 9 ) );
    }

    SECTION( "thorough test" )
    {
        int N = 5;
        std::vector<int> relative_indices({-1,+1});
        Field<Vector3> orientations(N, Vector3{0,0,1});
        Field<Vector3> intermediate(N, Vector3{0,0,0});
        Field<scalar> res(N, 0.0);

        auto lambda = [&](const std::size_t idx, const Vector3 & val) -> Vector3
        {
            Vector3 tmp = val;
            if( idx > 0 && idx < orientations.size()-1 )
            {
                for( auto& rel : relative_indices )
                {
                    tmp += orientations[idx+rel];
                }
            }
            else if( idx == 0 )
                tmp += orientations[idx+1];
            else if( idx == orientations.size()-1 )
                tmp += orientations[idx-1];
            return tmp;
        };

        res = orientations.dot(intermediate.applied_lambda(lambda));

        REQUIRE( res[0] == Approx( 1 ) );
        REQUIRE( res[1] == Approx( 2 ) );
        REQUIRE( res[2] == Approx( 2 ) );
        REQUIRE( res[3] == Approx( 2 ) );
        REQUIRE( res[4] == Approx( 1 ) );
    }
}