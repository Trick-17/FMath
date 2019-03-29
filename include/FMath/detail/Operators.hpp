#pragma once

#include <iostream>

#include <FMath/detail/Expression_Add.hpp>
#include <FMath/detail/Expression_Mult.hpp>
#include <FMath/detail/Field.hpp>

namespace FMath::detail
{
    // Function template for the + operator between two fields
    template<typename T, typename R1, typename R2>
    auto operator+(const Field<T, R1> & a, const Field<T, R2> & b)
    {
        return Field<T, FieldFieldAdd<T, R1, R2>>(
            FieldFieldAdd<T, R1, R2>(a.contents(), b.contents()));
    }

    // Function templates for the + operator between an entity and a field
    template<typename T, typename R1, typename R2>
    auto operator+(const R1 & a, const Field<T, R2> & b)
    {
        const bool condition = (std::is_arithmetic<R1>() && std::is_arithmetic<R1>()) ||
                               (!std::is_arithmetic<R1>() && !std::is_arithmetic<R1>());
        static_assert(
            condition, "FMATH USAGE ERROR: scalar + Field is only available "
                       "using arithmetic types");
        if constexpr (condition)
        {
            return Field<T, EntityFieldAdd<T, R1, R2>>(
                EntityFieldAdd<T, R1, R2>(a, b.contents()));
        }
    }
    template<typename T, typename R1, typename R2>
    auto operator+(const Field<T, R2> & b, const R1 & a)
    {
        const bool condition = (std::is_arithmetic<R1>() && std::is_arithmetic<R1>()) ||
                               (!std::is_arithmetic<R1>() && !std::is_arithmetic<R1>());
        static_assert(
            condition, "FMATH USAGE ERROR: scalar + Field is only available "
                       "using arithmetic types");
        if constexpr (condition)
        {
            return Field<T, EntityFieldAdd<T, R1, R2>>(
                EntityFieldAdd<T, R1, R2>(a, b.contents()));
        }
    }

    // Function template for the * operator between two fields
    template<typename T, typename R1, typename R2>
    auto operator*(const Field<T, R1> & a, const Field<T, R2> & b)
    {
        return Field<T, FieldFieldMultiplication<T, R1, R2>>(
            FieldFieldMultiplication<T, R1, R2>(a.contents(), b.contents()));
    }

    // Function templates for the * operator between an entity and a field (symmetric)
    template<typename T, typename R1, typename R2>
    auto operator*(const R1 & a, const Field<T, R2> & b)
    {
        const bool condition = (std::is_arithmetic<R1>() && std::is_arithmetic<R1>()) ||
                               (!std::is_arithmetic<R1>() && !std::is_arithmetic<R1>());
        static_assert(
            condition, "FMATH USAGE ERROR: scalar * Field is only available "
                       "using arithmetic types");
        if constexpr (condition)
        {
            return Field<T, EntityFieldMultiplication<T, R1, R2>>(
                EntityFieldMultiplication<T, R1, R2>(a, b.contents()));
        }
    }
    template<typename T, typename R1, typename R2>
    auto operator*(const Field<T, R2> & b, const R1 & a)
    {
        const bool condition = (std::is_arithmetic<R1>() && std::is_arithmetic<R1>()) ||
                               (!std::is_arithmetic<R1>() && !std::is_arithmetic<R1>());
        static_assert(
            condition, "FMATH USAGE ERROR: Field * scalar is only available "
                       "using arithmetic types");
        if constexpr (condition)
        {
            return Field<T, EntityFieldMultiplication<T, R1, R2>>(
                EntityFieldMultiplication<T, R1, R2>(a, b.contents()));
        }
    }

    // Function template for << operator
    template<typename T>
    std::ostream & operator<<(std::ostream & os, const Field<T> & cont)
    {
        std::cout << std::endl;
        for (std::size_t i = 0; i < cont.size(); ++i)
            os << cont[i] << ' ';
        os << std::endl;
        return os;
    }
}