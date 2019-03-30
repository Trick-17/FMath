#include <catch.hpp>

#include <FMath/Core>

using FMath::Field;
using FMath::scalar;
using FMath::Vector3;
using FMath::VectorX;

TEST_CASE( "Transform", "[Transform]" )
{
    int N = 200;

    Field<scalar> sf(N);
    Field<Vector3> vf(N);

    // Copy a scalar field to a new N-dimensional vector
    VectorX vec1 = sf.asRef<VectorX>();
    // Copy a vector field to a new 3N-dimensional vector
    VectorX vec2 = vf.asRef<VectorX>();

    // Interpret a scalar field as a N-dimensional vector without copying
    Eigen::Ref<VectorX> vecRef1 = sf.asRef<VectorX>();
    // Interpret a vector field as a 3N-dimensional vector without copying
    Eigen::Ref<VectorX> vecRef2 = vf.asRef<VectorX>();

    // Check
    REQUIRE(vecRef1.size() == N);
    REQUIRE(vecRef2.size() == 3*N);
}