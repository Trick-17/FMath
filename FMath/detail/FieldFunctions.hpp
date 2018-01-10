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