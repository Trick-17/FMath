#pragma once

#include <memory>
#include <vector>

#include <Eigen/Core>

namespace FMath
{
    // Scalar precision type
    using scalar = double;
        
    // Dynamic Eigen typedefs
    using VectorX    = Eigen::Matrix<scalar, -1,  1>;
    using RowVectorX = Eigen::Matrix<scalar,  1, -1>;
    using MatrixX    = Eigen::Matrix<scalar, -1, -1>;

    // 3D Eigen typedefs
    using Vector3    = Eigen::Matrix<scalar, 3, 1>;
    using RowVector3 = Eigen::Matrix<scalar, 1, 3>;
    using Matrix3    = Eigen::Matrix<scalar, 3, 3>;

    // Field
    template<typename T, typename Alloc = std::allocator<T> >
    using Field       = std::vector<T, Alloc>;

    // Fields (convenience)
    using IntField    = Field<int>;
    using ScalarField = Field<scalar>;
    using VectorField = Field<Vector3>;
}