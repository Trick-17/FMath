#pragma once

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

    // Assignment function to place value into container_to
    template<typename T, typename Container>
    void Field<T, Container>::assign(Container & container_to, const T & value)
    {
#       pragma omp parallel for
        for (std::size_t i = 0; i < container_to.size(); ++i)
            container_to[i] = value;
    }
}