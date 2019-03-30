#pragma once

#include <FMath/detail/Field.hpp>

#include <functional>

namespace FMath::detail
{
    // Elementwise lambda(Field)
    template<typename T, typename Op1, typename Lambda>
    class FieldLambda
    {
        const Op1 & _op1;
        const Lambda & _lambda;

      public:
        FieldLambda(const Op1 & a, const Lambda & l) : _op1(a), _lambda(l)
        {
            static_assert(
                std::is_convertible<
                    Lambda, std::function<void(std::size_t, const T &)>>::value,
                "FMATH USAGE ERROR: you cannot use FieldLambda expressions with a type "
                "that's not convertible to std::function<void(std::size_t, const T &)>.");
        }

        auto size() const
        {
            return _op1.size();
        }

        T operator[](const std::size_t i) const
        {
            return _lambda(i, _op1[i]);
        }
    };
}