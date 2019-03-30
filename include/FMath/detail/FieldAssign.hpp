#pragma once

namespace FMath::detail
{
    // Assignment function to evaluate container_from into container_to
    template<typename T, typename Container>
    template<typename Container2>
    void Field<T, Container>::assign(const Container2 & container_from)
    {
        assert(this->size() == container_from.size());
#       pragma omp parallel for
        for (std::size_t i = 0; i < this->size(); ++i)
            this->_container[i] = container_from[i];
    }

    // Assignment function to place value into container_to
    template<typename T, typename Container>
    void Field<T, Container>::assign(const T & value_from)
    {
#       pragma omp parallel for
        for (std::size_t i = 0; i < this->size(); ++i)
            this->_container[i] = value_from;
    }
}