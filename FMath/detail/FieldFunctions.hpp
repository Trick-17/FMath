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
    scalar Field<T, Container>::norm()
    {
        // TODO
        return 0;
    }

    template<typename T, typename Container>
    T Field<T, Container>::min()
    {
        T ret;
        if constexpr (std::is_same_v<T, Vector3>)
            ret.setZero();
        else
            ret = 0;
        
        // TODO

        return ret;
    }

    template<typename T, typename Container>
    T Field<T, Container>::max()
    {
        T ret;
        if constexpr (std::is_same_v<T, Vector3>)
            ret.setZero();
        else
            ret = 0;
        
        // TODO
        
        return ret;
    }

    template<typename T, typename Container>
    std::pair<T, T> Field<T, Container>::minmax()
    {
        T ret_min, ret_max;
        if constexpr (std::is_same_v<T, Vector3>)
        {
            ret_min.setZero();
            ret_max.setZero();
        }
        else
        {
            ret_min = 0;
            ret_max = 0;
        }
        
        // TODO
        
        return {ret_min, ret_max};
    }

    template<typename T, typename Container>
    std::pair<scalar, scalar> Field<T, Container>::minmax_component()
    {
        static_assert(std::is_same_v<T, Vector3>, "Field<...>.minmax_component() is only available on Field<Vector3>");

        scalar ret_min, ret_max;
        ret_min = 0;
        ret_max = 0;
        
        // TODO
        
        return {ret_min, ret_max};
    }

    //////// Operations on self /////////////////////////////////////

    template<typename T, typename Container>
    void Field<T, Container>::normalize()
    {
        if (norm() > 0)
        {
            // TODO
        }
    }

    // template<typename T, typename Container>
    // Field Field<T, Container>::normalized()
    // {
    //     if (norm() > 0)
    //     {
    //         // TODO: move this into a new expression
    //     }
    //     else
    //         // TODO: create trivial expression
    // }

    //////// Operations with others /////////////////////////////////

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