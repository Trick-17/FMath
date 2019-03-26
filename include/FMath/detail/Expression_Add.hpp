#pragma once

#include <FMath/detail/Field.hpp>

namespace FMath::detail
{
    // Elementwise Field + Field
    template<typename T, typename Op1, typename Op2>
    class FieldFieldAdd
    {
        Op1 const & _op1;
        Op2 const & _op2;

      public:
        FieldFieldAdd(Op1 const & a, Op2 const & b) : _op1(a), _op2(b)
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
        Op1 const & _ent;
        Op2 const & _op2;

      public:
        EntityFieldAdd(Op1 const & ent, Op2 const & op2) : _ent(ent), _op2(op2) {}

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