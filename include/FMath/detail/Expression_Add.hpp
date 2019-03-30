#pragma once

#include <FMath/detail/Field.hpp>

namespace FMath::detail
{
    // Elementwise Field + Field
    template<typename T, typename Op1, typename Op2>
    class FieldFieldAdd
    {
        const Op1 & _op1;
        const Op2 & _op2;

      public:
        FieldFieldAdd(const Op1 & a, const Op2 & b) : _op1(a), _op2(b)
        {
            assert(_op1.size() == _op2.size());
        }

        auto size() const
        {
            return _op1.size();
        }

        T operator[](const std::size_t i) const
        {
            return _op1[i] + _op2[i];
        }
    };

    // Entity + Field
    template<typename T, typename Op1, typename Op2>
    class EntityFieldAdd
    {
        const Op1 & _ent;
        const Op2 & _op2;

      public:
        EntityFieldAdd(const Op1 & ent, const Op2 & op2) : _ent(ent), _op2(op2) {}

        auto size() const
        {
            return _op2.size();
        }

        T operator[](const std::size_t i) const
        {
            return _ent + _op2[i];
        }
    };
}