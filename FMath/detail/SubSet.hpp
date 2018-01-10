#pragma once

#include <detail/Field.hpp>

namespace FMath::detail
{
    // Elementwise element extraction via an index list
    template<typename T, typename Container>
    class SubSet
    {
        Container & _container;
        std::vector<int> const& _indices;

    public:
        SubSet(Container & container, const std::vector<int> & indices): _container(container), _indices(indices)
        {
            assert(_container.size() >= _indices.size());
        }

        T operator[](const std::size_t i) const
        {
            return _container[_indices[i]];
        }

        T& operator[](const std::size_t i)
        {
            return _container[_indices[i]];
        }

        std::size_t size() const
        { 
            return _indices.size(); 
        }
    };
}