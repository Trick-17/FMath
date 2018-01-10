#pragma once

#include <iostream>

#include <detail/Field.hpp>

namespace FMath::detail
{
    // Elementwise Field + Field
    template<typename T, typename Op1, typename Op2>
    class FieldFieldAdd
    {
        Op1 const& _op1;
        Op2 const& _op2;

    public:
        FieldFieldAdd(Op1 const& a, Op2 const& b): _op1(a), _op2(b)
        {
            assert(_op1.size() == _op2.size());
        }

        T operator[](const std::size_t i) const
        { 
            return _op1[i] + _op2[i]; 
        }

        std::size_t size() const
        { 
            return _op1.size(); 
        }
    };

    // Entity + Field
    template<typename T, typename Op1 , typename Op2>
    class EntityFieldAdd
    {
        Op1 const& _ent;
        Op2 const& _op2;

    public:
        EntityFieldAdd(Op1 const& ent, Op2 const& op2): _ent(ent), _op2(op2) {}

        T operator[](const std::size_t i) const
        { 
            return _ent + _op2[i]; 
        }

        std::size_t size() const
        { 
            return _op2.size(); 
        }
    };

    // Elementwise Field * Field
    template< typename T, typename Op1 , typename Op2 >
    class FieldFieldMultiplication
    {
        Op1 const& _op1;
        Op2 const& _op2;

    public:
        FieldFieldMultiplication(Op1 const& a, Op2 const& b ): _op1(a), _op2(b)
        {
            assert(_op1.size() == _op2.size());
        }

        T operator[](const std::size_t i) const
        { 
            return _op1[i] * _op2[i]; 
        }

        std::size_t size() const
        { 
            return _op1.size(); 
        }
    };

    // entity * Field
    template< typename T, typename Op1, typename Op2 >
    class EntityFieldMultiplication
    {
        Op1 const& _ent;
        Op2 const& _op2;

    public:
        EntityFieldMultiplication(const Op1 & ent, const Op2 & a ): _ent(ent), _op2(a) {}

        T operator[](const std::size_t i) const
        { 
            return _ent * _op2[i]; 
        }

        std::size_t size() const
        { 
            return _op2.size(); 
        }
    };

    // Function template for the + operator between two fields
    template<typename T, typename R1, typename R2>
    Field<T, FieldFieldAdd<T, R1, R2>> operator+ (const Field<T, R1>& a, const Field<T, R2>& b)
    {
        return Field<T, FieldFieldAdd<T, R1, R2>>(FieldFieldAdd<T, R1, R2 >(a.contents(), b.contents()));
    }

    // Function templates for the + operator between an entity and a field
    template<typename T, typename R1, typename R2>
    Field<T, EntityFieldAdd<T, R1, R2>> operator+ (const R1 & a, const Field<T, R2> & b)
    {
        static_assert((std::is_arithmetic<R1>() && std::is_arithmetic<R1>()) || (!std::is_arithmetic<R1>() && !std::is_arithmetic<R1>()),
            "scalar + Field is only available using arithmetic types");
        return Field<T, EntityFieldAdd<T, R1, R2>>(EntityFieldAdd<T, R1, R2 >(a, b.contents()));
    }
    template<typename T, typename R1, typename R2>
    Field<T, EntityFieldAdd<T, R1, R2>> operator+ (const Field<T, R2>& b, const R1 & a)
    {
        static_assert((std::is_arithmetic<R1>() && std::is_arithmetic<R1>()) || (!std::is_arithmetic<R1>() && !std::is_arithmetic<R1>()),
            "scalar + Field is only available using arithmetic types");
        return Field<T, EntityFieldAdd<T, R1, R2>>(EntityFieldAdd<T, R1, R2 >(a, b.contents()));
    }

    // Function template for the * operator between two fields
    template<typename T, typename R1, typename R2>
    Field<T, FieldFieldMultiplication< T, R1, R2>> operator* (const Field<T, R1>& a, const Field<T, R2>& b)
    {
        return Field<T, FieldFieldMultiplication<T, R1, R2>>(FieldFieldMultiplication<T, R1, R2 >(a.contents(), b.contents()));
    }

    // Function templates for the * operator between an entity and a field (symmetric)
    template<typename T, typename R1, typename R2>
    Field<T, EntityFieldMultiplication<T, R1, R2>> operator* (const R1 & a, const Field<T, R2> & b)
    {
        static_assert((std::is_arithmetic<R1>() && std::is_arithmetic<R1>()) || (!std::is_arithmetic<R1>() && !std::is_arithmetic<R1>()),
            "scalar * Field is only available using arithmetic types");
        return Field<T, EntityFieldMultiplication<T, R1, R2>>(EntityFieldMultiplication<T, R1, R2>(a, b.contents()));
    }
    template<typename T, typename R1, typename R2>
    Field<T, EntityFieldMultiplication<T, R1, R2>> operator* (const Field<T, R2> & b, const R1 & a)
    {
        static_assert((std::is_arithmetic<R1>() && std::is_arithmetic<R1>()) || (!std::is_arithmetic<R1>() && !std::is_arithmetic<R1>()),
            "Field * scalar is only available using arithmetic types");
        return Field<T, EntityFieldMultiplication<T, R1, R2>>(EntityFieldMultiplication<T, R1, R2>(a, b.contents()));
    }

    // Function template for << operator
    template<typename T> std::ostream& operator<< (std::ostream& os, const Field<T>& cont)
    {
        std::cout << std::endl;
        for (int i=0; i<cont.size(); ++i)
            os << cont[i] << ' ';
        os << std::endl;
        return os;
    }
}