#pragma once

#include <Eigen/Core>
#include <FMath/detail/Field.hpp>
#include <FMath/detail/Using.hpp>

namespace FMath
{
    // Field
    template<typename T>
    using Field       = FMath::detail::Field<T>;
    
    // Convenience Fields
    using IntField    = Field<int>;
    using ScalarField = Field<scalar>;
    using VectorField = Field<Vector3>;
}