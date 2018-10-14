#pragma once

#include <FMath/detail/Field.hpp>

namespace FMath::detail
{
    // For a VectorField, this returns a Field of the Vector3 norms
    template< typename T, typename Op >
    class NormEx
    {
        Op const& _op;

    public:
        NormEx(Op const& a): _op(a)
        {
            static_assert(std::is_same_v<T, Vector3>, "Field<...>.norm() is only available on Field<Vector3>");
        }

        auto size() const { return _op.size(); }

        scalar operator[](const std::size_t i) const
        { 
            return _op[i].norm();
        }
    };

    // For a VectorField, this returns a Field of the Vector3 squared norms
    template< typename T, typename Op >
    class SquaredNormEx
    {
        Op const& _op;

    public:
        SquaredNormEx(Op const& a): _op(a)
        {
            static_assert(std::is_same_v<T, Vector3>, "Field<...>.squaredNorm() is only available on Field<Vector3>");
        }

        auto size() const { return _op.size(); }

        scalar operator[](const std::size_t i) const
        { 
            return _op[i].squaredNorm();
        }
    };

    // For a VectorField, this normalizes each Vector3 of the Field
    template< typename T, typename Op >
    class NormalizeEx
    {
        Op const& _op;

    public:
        NormalizeEx(Op const& a): _op(a)
        {
            static_assert(std::is_same_v<T, Vector3>, "Field<...>.normalize() is only available on Field<Vector3>");
        }

        auto size() const { return _op.size(); }

        void operator[](const std::size_t i) const
        { 
            _op[i].normalize();
        }
    };

    // For a VectorField, this returns normalized Vector3 entries
    template< typename T, typename Op >
    class NormalizedEx
    {
        Op const& _op;

    public:
        NormalizedEx(Op const& a): _op(a)
        {
            static_assert(std::is_same_v<T, Vector3>, "Field<...>.normalized() is only available on Field<Vector3>");
        }

        auto size() const { return _op.size(); }

        T operator[](const std::size_t i) const
        { 
            return _op[i].normalized();
        }
    };

    // For two VectorFields, this returns a ScalarField of the Vector3 dot products
    template< typename T, typename Op1, typename Op2 >
    class FieldDotFieldEx
    {
        Op1 const& _op1;
        Op2 const& _op2;

    public:
        FieldDotFieldEx(Op1 const& a, Op2 const& b): _op1(a), _op2(b)
        {
            static_assert(std::is_same_v<T, Vector3>, "Field<...>.dot(...) is only available on Field<Vector3>");
            assert(_op1.size() == _op2.size());
        }

        auto size() const { return _op1.size(); }

        scalar operator[](const std::size_t i) const
        { 
            return _op1[i].dot(_op2[i]);
        }
    };

    // For a VectorField, this returns a ScalarField of the dot products with a Vector3
    template< typename T, typename Op >
    class VectorDotFieldEx
    {
        Op const& _op;
        Vector3 const& _vec;

    public:
        VectorDotFieldEx(Op const& a, Vector3 const& b): _op(a), _vec(b)
        {
            static_assert(std::is_same_v<T, Vector3>, "Field<...>.dot(...) is only available on Field<Vector3>");
        }

        auto size() const { return _op.size(); }

        scalar operator[](const std::size_t i) const
        { 
            return _op[i].dot(_vec);
        }
    };

    // For two VectorFields, this returns a VectorField of the Vector3 cross products
    template< typename T, typename Op1, typename Op2 >
    class FieldCrossFieldEx
    {
        Op1 const& _op1;
        Op2 const& _op2;

    public:
        FieldCrossFieldEx(Op1 const& a, Op2 const& b): _op1(a), _op2(b)
        {
            static_assert(std::is_same_v<T, Vector3>, "Field<...>.cross(...) is only available on Field<Vector3>");
            assert(_op1.size() == _op2.size());
        }

        auto size() const { return _op1.size(); }

        T operator[](const std::size_t i) const
        { 
            return _op1[i].cross(_op2[i]);
        }
    };

    // For two VectorFields, this returns a VectorField of the Vector3 cross products
    template< typename T, typename Op1 >
    class VectorCrossFieldEx
    {
        Op1 const& _op1;
        Vector3 const& _vec;

    public:
        VectorCrossFieldEx(Op1 const& a, Vector3 const& b): _op1(a), _vec(b)
        {
            static_assert(std::is_same_v<T, Vector3>, "Field<...>.cross(...) is only available on Field<Vector3>");
        }

        auto size() const { return _op1.size(); }

        T operator[](const std::size_t i) const
        { 
            return _op1[i].cross(_vec);
        }
    };
}
