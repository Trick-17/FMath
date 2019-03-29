#pragma once

#include <FMath/detail/Field.hpp>

#include <functional>

namespace FMath::detail
{
    // Elementwise lambda(Field)
    template<typename T, typename Op1>
    class FieldLambda
    {
        using Lambda = std::function<void(std::size_t, T &)>;

        Op1 & _op1;
        Lambda const & _lambda;

      public:
        FieldLambda(Op1 & a, Lambda const & b) : _op1(a), _lambda(b) {}

        auto size() const
        {
            return _op1.size();
        }

        T operator[](const std::size_t i) const
        {
            _lambda(i, _op1[i]);
            return _op1[i];
        }
    };
}