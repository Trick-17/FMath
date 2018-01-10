#pragma once

#include <Eigen/Dense>

namespace FMath::detail
{
    // Assignment function to evaluate container_from into container_to
    template<typename T, typename Container> template <typename Container2>
    void Field<T, Container>::assign(Container & container_to, const Container2 & container_from)
    {
        assert(container_to.size() == container_from.size());
#       pragma omp parallel for
        for (std::size_t i = 0; i < container_to.size(); ++i)
            container_to[i] = container_from[i];
    }

    //////// Reductions /////////////////////////////////////////////

    // Definition for OpenMP reduction operation using Vector3's
    #pragma omp declare reduction (+: Vector3: omp_out=omp_out+omp_in)\
        initializer(omp_priv=Vector3::Zero())

    template<typename T, typename Container>
    T Field<T, Container>::sum()
    {
        T ret;
        if constexpr (std::is_same_v<T, Vector3>)
            ret.setZero();
        else
            ret = 0;

        #pragma omp parallel for reduction(+:ret)
        for (std::size_t i = 0; i < size(); ++i)
            ret += _container[i];
        return ret;
    }

    template<typename T, typename Container>
    T Field<T, Container>::mean()
    {
        return sum()/static_cast<long double>(size());
    }

    template<typename T, typename Container>
    scalar Field<T, Container>::min()
    {
        static_assert(std::is_arithmetic<T>(), "Field<...>.min() is only available on Field<scalar>");

        scalar minval =  std::numeric_limits<scalar>::max();

#       pragma omp parallel for reduction(min: minval)
        for (unsigned int i = 0; i < size(); ++i)
            if (_container[i] < minval) minval = _container[i];

        return minval;
    }

    template<typename T, typename Container>
    scalar Field<T, Container>::max()
    {
        static_assert(std::is_arithmetic<T>(), "Field<...>.max() is only available on Field<scalar>");

        scalar maxval = -std::numeric_limits<scalar>::max();

#       pragma omp parallel for reduction(max : maxval)
        for (unsigned int i = 0; i < size(); ++i)
        {
            if (_container[i] > maxval) maxval = _container[i];
        }

        return maxval;
    }

    template<typename T, typename Container>
    std::pair<scalar, scalar> Field<T, Container>::minmax()
    {
        static_assert(std::is_arithmetic<T>(), "Field<...>.minmax() is only available on Field<scalar>");

        scalar minval =  std::numeric_limits<scalar>::max();
        scalar maxval = -std::numeric_limits<scalar>::max();

#       pragma omp parallel for reduction(min: minval) reduction(max : maxval)
        for (unsigned int i = 0; i < size(); ++i)
        {
            if (_container[i] < minval) minval = _container[i];
            if (_container[i] > maxval) maxval = _container[i];
        }

        return {minval, maxval};
    }

    template<typename T, typename Container>
    scalar Field<T, Container>::min_component()
    {
        static_assert(std::is_same_v<T, Vector3>, "Field<...>.min_component() is only available on Field<Vector3>");

        scalar minval =  std::numeric_limits<scalar>::max();

#       pragma omp parallel for reduction(min: minval)
        for (unsigned int i = 0; i < size(); ++i)
            for (int dim = 0; dim < 3; ++dim)
                if (_container[i][dim] < minval) minval = _container[i][dim];

        return minval;
    }

    template<typename T, typename Container>
    scalar Field<T, Container>::max_component()
    {
        static_assert(std::is_same_v<T, Vector3>, "Field<...>.max_component() is only available on Field<Vector3>");

        scalar maxval = -std::numeric_limits<scalar>::max();

#       pragma omp parallel for reduction(max : maxval)
        for (unsigned int i = 0; i < size(); ++i)
            for (int dim = 0; dim < 3; ++dim)
                if (_container[i][dim] > maxval) maxval = _container[i][dim];

        return maxval;
    }
    
    template<typename T, typename Container>
    std::pair<scalar, scalar> Field<T, Container>::minmax_component()
    {
        static_assert(std::is_same_v<T, Vector3>, "Field<...>.minmax_component() is only available on Field<Vector3>");

        scalar minval =  std::numeric_limits<scalar>::max();
        scalar maxval = -std::numeric_limits<scalar>::max();

#       pragma omp parallel for reduction(min: minval) reduction(max : maxval)
        for (unsigned int i = 0; i < size(); ++i)
        {
            for (int dim = 0; dim < 3; ++dim)
            {
                if (_container[i][dim] < minval) minval = _container[i][dim];
                if (_container[i][dim] > maxval) maxval = _container[i][dim];
            }
        }

        return {minval, maxval};
    }

    //////// VectorField Operations on self /////////////////////////

    // For a VectorField, this returns a Field of the Vector3 norms
    template<typename T, typename Container>
    Field<scalar> Field<T, Container>::norm()
    {
        static_assert(std::is_same_v<T, Vector3>, "Field<...>.norm() is only available on Field<Vector3>");

        // TODO: move this into a new expression
        Field<scalar> ret(size());

#       pragma omp parallel for
        for (std::size_t i = 0; i < size(); ++i)
            ret[i] = _container[i].norm();

        return ret;
    }
    
    template<typename T, typename Container>
    Field<scalar> Field<T, Container>::squaredNorm()
    {
        static_assert(std::is_same_v<T, Vector3>, "Field<...>.norm() is only available on Field<Vector3>");

        // TODO: move this into a new expression
        Field<scalar> ret(size());

#       pragma omp parallel for
        for (std::size_t i = 0; i < size(); ++i)
            ret[i] = _container[i].squaredNorm();

        return ret;
    }

    template<typename T, typename Container>
    void Field<T, Container>::normalize()
    {
        static_assert(std::is_same_v<T, Vector3>, "Field<...>.normalize() is only available on Field<Vector3>");

#       pragma omp parallel for
        for (std::size_t i = 0; i < size(); ++i)
            _container[i].normalize();
    }

    template<typename T, typename Container>
    Field<Vector3> Field<T, Container>::normalized()
    {
        static_assert(std::is_same_v<T, Vector3>, "Field<...>.nonormalizedrm() is only available on Field<Vector3>");

        // TODO: move this into a new expression
        Field<Vector3> ret(size());

#       pragma omp parallel for
        for (std::size_t i = 0; i < size(); ++i)
            ret[i] = _container[i].normalized();

        return ret;
    }

    //////// VectorField Operations with others /////////////////////

    template<typename T, typename Container> template <typename R2>
    Field<scalar> Field<T, Container>::dot(const Field<Vector3,R2> & field)
    {
        static_assert(std::is_same_v<T, Vector3>, "Field<...>.dot(...) is only available on Field<Vector3>");

        // TODO: move this into a new expression
        assert(size() == field.size());
        Field<scalar> ret(size());

#       pragma omp parallel for
        for (std::size_t i = 0; i < size(); ++i)
            ret[i] = _container[i].dot(field[i]);

        return ret;
    }

    template<typename T, typename Container> template <typename R2>
    Field<scalar> Field<T, Container>::dot(const Vector3 & vec)
    {
        static_assert(std::is_same_v<T, Vector3>, "Field<...>.dot(...) is only available on Field<Vector3>");

        // TODO: move this into a new expression
        Field<scalar> ret(size());

#       pragma omp parallel for
        for (std::size_t i = 0; i < size(); ++i)
            ret[i] = _container[i].dot(vec);

        return ret;
    }

    template<typename T, typename Container> template <typename R2>
    Field<Vector3> Field<T, Container>::cross(const Field<Vector3,R2> & field)
    {
        static_assert(std::is_same_v<T, Vector3>, "Field<...>.cross(...) is only available on Field<Vector3>");
        
        // TODO: move this into a new expression
        assert(size() == field.size());
        Field<Vector3> ret(size());

#       pragma omp parallel for
        for (std::size_t i = 0; i < size(); ++i)
            ret[i] = _container[i].cross(field[i]);

        return ret;
    }

    template<typename T, typename Container> template <typename R2>
    Field<Vector3> Field<T, Container>::cross(const Vector3 & vec)
    {
        static_assert(std::is_same_v<T, Vector3>, "Field<...>.cross(...) is only available on Field<Vector3>");
        
        // TODO: move this into a new expression
        Field<Vector3> ret(size());

#       pragma omp parallel for
        for (std::size_t i = 0; i < size(); ++i)
            ret[i] = _container[i].cross(vec);

        return ret;
    }
}