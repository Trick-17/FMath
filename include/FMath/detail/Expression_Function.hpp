#pragma once

#include <FMath/detail/Field.hpp>

namespace FMath::detail
{
    // For a VectorField, this returns a Field of the Vector3 norms
    template<typename T, typename Op>
    class NormEx
    {
        const Op & _op;

      public:
        NormEx(const Op & a) : _op(a)
        {
            static_assert(
                std::is_same_v<T, Vector3>,
                "FMATH USAGE ERROR: NormEx can only be used with Vector3 as value type.");
        }

        auto size() const
        {
            return _op.size();
        }

        scalar operator[](const std::size_t i) const
        {
            return _op[i].norm();
        }
    };

    // For a VectorField, this returns a Field of the Vector3 squared norms
    template<typename T, typename Op>
    class SquaredNormEx
    {
        const Op & _op;

      public:
        SquaredNormEx(const Op & a) : _op(a)
        {
            static_assert(
                std::is_same_v<T, Vector3>, "FMATH USAGE ERROR: SquaredNormEx can only "
                                            "be used with Vector3 as value type.");
        }

        auto size() const
        {
            return _op.size();
        }

        scalar operator[](const std::size_t i) const
        {
            return _op[i].squaredNorm();
        }
    };

    // For a VectorField, this returns normalized Vector3 entries
    template<typename T, typename Op>
    class NormalizedEx
    {
        const Op & _op;

      public:
        NormalizedEx(const Op & a) : _op(a)
        {
            static_assert(
                std::is_same_v<T, Vector3>, "FMATH USAGE ERROR: NormalizedEx can only be "
                                            "used with Vector3 as value type.");
        }

        auto size() const
        {
            return _op.size();
        }

        T operator[](const std::size_t i) const
        {
            return _op[i].normalized();
        }
    };

    // For two VectorFields, this returns a ScalarField of the Vector3 dot products
    template<typename T, typename Op1, typename Op2>
    class FieldDotFieldEx
    {
        const Op1 & _op1;
        const Op2 & _op2;

      public:
        FieldDotFieldEx(const Op1 & a, const Op2 & b) : _op1(a), _op2(b)
        {
            static_assert(
                std::is_same_v<T, Vector3>, "FMATH USAGE ERROR: FieldDotFieldEx can only "
                                            "be used with Vector3 as value type.");
            assert(_op1.size() == _op2.size());
        }

        auto size() const
        {
            return _op1.size();
        }

        scalar operator[](const std::size_t i) const
        {
            return _op1[i].dot(_op2[i]);
        }
    };

    // For a VectorField, this returns a ScalarField of the dot products with a Vector3
    template<typename T, typename Op>
    class VectorDotFieldEx
    {
        const Op & _op;
        const Vector3 & _vec;

      public:
        VectorDotFieldEx(const Op & a, const Vector3 & b) : _op(a), _vec(b)
        {
            static_assert(
                std::is_same_v<T, Vector3>, "FMATH USAGE ERROR: VectorDotFieldEx can "
                                            "only be used with Vector3 as value type.");
        }

        auto size() const
        {
            return _op.size();
        }

        scalar operator[](const std::size_t i) const
        {
            return _op[i].dot(_vec);
        }
    };

    // For two VectorFields, this returns a VectorField of the Vector3 cross products
    template<typename T, typename Op1, typename Op2>
    class FieldCrossFieldEx
    {
        const Op1 & _op1;
        const Op2 & _op2;

      public:
        FieldCrossFieldEx(const Op1 & a, const Op2 & b) : _op1(a), _op2(b)
        {
            static_assert(
                std::is_same_v<T, Vector3>, "FMATH USAGE ERROR: FieldCrossFieldEx can "
                                            "only be used with Vector3 as value type.");
            assert(_op1.size() == _op2.size());
        }

        auto size() const
        {
            return _op1.size();
        }

        T operator[](const std::size_t i) const
        {
            return _op1[i].cross(_op2[i]);
        }
    };

    // For two VectorFields, this returns a VectorField of the Vector3 cross products
    template<typename T, typename Op1>
    class VectorCrossFieldEx
    {
        const Op1 & _op1;
        const Vector3 & _vec;

      public:
        VectorCrossFieldEx(const Op1 & a, const Vector3 & b) : _op1(a), _vec(b)
        {
            static_assert(
                std::is_same_v<T, Vector3>, "FMATH USAGE ERROR: VectorCrossFieldEx can "
                                            "only be used with Vector3 as value type.");
        }

        auto size() const
        {
            return _op1.size();
        }

        T operator[](const std::size_t i) const
        {
            return _op1[i].cross(_vec);
        }
    };
}