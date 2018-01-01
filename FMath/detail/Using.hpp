#pragma once

#include <Eigen/Core>

namespace FMath
{
    // Scalar precision type
#   ifdef FMATH_SCALAR_TYPE
        using scalar = FMATH_SCALAR_TYPE;
#   else
        using scalar = double;
#   endif

    // Dynamic Eigen typedefs
    using VectorX    = Eigen::Matrix<scalar, -1,  1>;
    using RowVectorX = Eigen::Matrix<scalar,  1, -1>;
    using MatrixX    = Eigen::Matrix<scalar, -1, -1>;

    // 3D Eigen typedefs
    using Vector3    = Eigen::Matrix<scalar, 3, 1>;
    using RowVector3 = Eigen::Matrix<scalar, 1, 3>;
    using Matrix3    = Eigen::Matrix<scalar, 3, 3>;
}