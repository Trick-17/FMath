#pragma once

#include <FMath/detail/Field.hpp>

namespace FMath::detail
{
    // Elementwise element extraction via an index list
    template<typename T, typename Container>
    class SubSetEx
    {
        Container & _container;
        std::vector<std::size_t> const & _indices;

      public:
        SubSetEx(Container & container, const std::vector<std::size_t> & indices)
                : _container(container), _indices(indices)
        {
            assert(_container.size() >= _indices.size());
        }

        T operator[](const std::size_t i) const
        {
            return _container[_indices[i]];
        }

        T & operator[](const std::size_t i)
        {
            return _container[_indices[i]];
        }

        auto size() const
        {
            return _indices.size();
        }
    };
}