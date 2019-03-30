#pragma once

#include <Eigen/Dense>

namespace FMath::detail
{
    // Definition for OpenMP reduction operation using Vector3's
#   pragma omp declare reduction(+ : Vector3 : omp_out = omp_out + omp_in)\
        initializer(omp_priv = Vector3::Zero())

    template<typename T, typename Container>
    T Field<T, Container>::sum() const
    {
        T ret;
        if constexpr (std::is_same_v<T, Vector3>)
            ret.setZero();
        else
            ret = 0;

#       pragma omp parallel for reduction(+ : ret)
        for (std::size_t i = 0; i < size(); ++i)
            ret += _container[i];
        return ret;
    }

    template<typename T, typename Container>
    T Field<T, Container>::mean() const
    {
        return sum() / static_cast<long double>(size());
    }

    template<typename T, typename Container>
    scalar Field<T, Container>::min() const
    {
        static_assert(
            std::is_arithmetic<T>(),
            "Field<...>.min() is only available on Field<scalar>");

        scalar minval = std::numeric_limits<scalar>::max();

#       pragma omp parallel for reduction(min : minval)
        for (std::size_t i = 0; i < size(); ++i)
            if (_container[i] < minval) minval = _container[i];

        return minval;
    }

    template<typename T, typename Container>
    scalar Field<T, Container>::max() const
    {
        static_assert(
            std::is_arithmetic<T>(),
            "Field<...>.max() is only available on Field<scalar>");

        scalar maxval = -std::numeric_limits<scalar>::max();

#       pragma omp parallel for reduction(max : maxval)
        for (std::size_t i = 0; i < size(); ++i)
        {
            if (_container[i] > maxval) maxval = _container[i];
        }

        return maxval;
    }

    template<typename T, typename Container>
    std::pair<scalar, scalar> Field<T, Container>::minmax() const
    {
        static_assert(
            std::is_arithmetic<T>(),
            "Field<...>.minmax() is only available on Field<scalar>");

        scalar minval = std::numeric_limits<scalar>::max();
        scalar maxval = -std::numeric_limits<scalar>::max();

#       pragma omp parallel for reduction(min : minval) reduction(max : maxval)
        for (std::size_t i = 0; i < size(); ++i)
        {
            if (_container[i] < minval) minval = _container[i];
            if (_container[i] > maxval) maxval = _container[i];
        }

        return { minval, maxval };
    }

    template<typename T, typename Container>
    scalar Field<T, Container>::min_component() const
    {
        static_assert(
            std::is_same_v<T, Vector3>,
            "Field<...>.min_component() is only available on Field<Vector3>");

        scalar minval = std::numeric_limits<scalar>::max();

#       pragma omp parallel for reduction(min : minval)
        for (std::size_t i = 0; i < size(); ++i)
            for (int dim = 0; dim < 3; ++dim)
                if (_container[i][dim] < minval) minval = _container[i][dim];

        return minval;
    }

    template<typename T, typename Container>
    scalar Field<T, Container>::max_component() const
    {
        static_assert(
            std::is_same_v<T, Vector3>,
            "Field<...>.max_component() is only available on Field<Vector3>");

        scalar maxval = -std::numeric_limits<scalar>::max();

#       pragma omp parallel for reduction(max : maxval)
        for (std::size_t i = 0; i < size(); ++i)
            for (int dim = 0; dim < 3; ++dim)
                if (_container[i][dim] > maxval) maxval = _container[i][dim];

        return maxval;
    }

    template<typename T, typename Container>
    std::pair<scalar, scalar> Field<T, Container>::minmax_component() const
    {
        static_assert(
            std::is_same_v<T, Vector3>,
            "Field<...>.minmax_component() is only available on Field<Vector3>");

        scalar minval = std::numeric_limits<scalar>::max();
        scalar maxval = -std::numeric_limits<scalar>::max();

#       pragma omp parallel for reduction(min : minval) reduction(max : maxval)
        for (std::size_t i = 0; i < size(); ++i)
        {
            for (int dim = 0; dim < 3; ++dim)
            {
                if (_container[i][dim] < minval) minval = _container[i][dim];
                if (_container[i][dim] > maxval) maxval = _container[i][dim];
            }
        }

        return { minval, maxval };
    }
}