#pragma once

#include <cassert>
#include <vector>

#include <Eigen/Core>

#include <FMath/detail/Expression_Function.hpp>
#include <FMath/detail/Expression_Lambda.hpp>
#include <FMath/detail/Expression_Slice.hpp>
#include <FMath/detail/Expression_SubSet.hpp>
#include <FMath/detail/Using.hpp>

namespace FMath::detail
{
    template<typename T, typename Container = std::vector<T>>
    class Field
    {
        Container _container;

        // Assignment function which explicitly evaluates an expression
        template<typename Container2>
        void assign(const Container2 & container_from);
        void assign(const T & value);

      public:
        ///////////// Constructors //////////////////////////////////////////////////////

        // Field with initial size
        Field(const std::size_t n) : _container(n) {}

        // Field with initial size and value
        Field(const std::size_t n, const T & initialValue) : _container(n, initialValue) {}

        // Field via initializer list
        Field(const std::initializer_list<T> & list) : _container(list) {}

        // Constructor for underlying container
        Field(const Container & other) : _container(other) {}

        // A Field can be constructed such as to force its evaluation.
        template<typename T2, typename R2>
        Field(const Field<T2, R2> & other) : _container(other.size())
        {
            static_assert(
                std::is_same_v<T, T2>, "FMATH USAGE ERROR: Field<> template parameter "
                                       "must be identical for assignment to work");
            if constexpr (std::is_same_v<T, T2>)
            {
                this->assign(other);
            }
        }

        // Assignment operator for Field of same type (copy assignment)
        Field & operator=(const Field & other)
        {
            assert(size() == other.size());
            this->assign(other);
            return *this;
        }

        ///////////// Assignment ////////////////////////////////////////////////////////

        // Assignment operator for Field of different type
        template<typename T2, typename R2>
        Field & operator=(const Field<T2, R2> & other)
        {
            static_assert(
                std::is_same_v<T, T2>, "FMATH USAGE ERROR: Field<> template parameter "
                                       "must be identical for assignment to work");
            if constexpr (std::is_same_v<T, T2>)
            {
                assert(size() == other.size());
                this->assign(other);
                return *this;
            }
        }

        // Assignment operator for entity
        Field & operator=(const T & value)
        {
            this->assign(value);
            return *this;
        }

        ///////////// Basics ////////////////////////////////////////////////////////////

        void resize(const std::size_t size)
        {
            _container.resize(size);
        }
        void resize(const std::size_t size, const T & value)
        {
            _container.resize(size, value);
        }

        // If the container is a std::vector, data can be retrieved as a pointer
        T * data()
        {
            static_assert(
                std::is_same_v<Container, std::vector<T>>,
                "FMATH USAGE ERROR: data() is only available for evaluated "
                "Fields, not "
                "Expressions");
            if constexpr (std::is_same_v<Container, std::vector<T>>)
            {
                return _container.data();
            }
        }

        // Size of underlying container
        auto size() const
        {
            return _container.size();
        }

        // Index operators
        T operator[](const std::size_t i) const
        {
            return _container[i];
        }
        T & operator[](const std::size_t i)
        {
            return _container[i];
        }

        // Returns the underlying data
        const Container & contents() const
        {
            return _container;
        }
        Container & contents()
        {
            return _container;
        }

        ///////////// Transformation ////////////////////////////////////////////////////

        // Re-interpretation as a reference to an Eigen::VectorX
        template<typename RefT>
        Eigen::Ref<RefT> asRef()
        {
            // Field<Vector3> to VectorX of size 3*N
            if constexpr (std::is_same_v<T, Vector3>)
                return Eigen::Ref<RefT>(
                    Eigen::Map<RefT>(this->_container[0].data(), 3 * this->size()));
            // Field<scalar> etc. to VectorX of size N
            else
                return Eigen::Ref<RefT>(Eigen::Map<RefT>(this->data(), this->size()));
        }

        ///////////// Lambda ////////////////////////////////////////////////////////////

        // Applies a given lambda to every entry of the Field.
        // The lambda is passed the index and the entry corresponding to the index.
        // This function is applied immediately, not on assignment.
        template<typename Lambda>
        void apply_lambda(const Lambda & lambda)
        {
            this->assign(Field<T, FieldLambda<T, Container, Lambda>>(
                FieldLambda<T, Container, Lambda>(this->contents(), lambda)));
        }

        // Applies a given lambda to every entry of the Field.
        // The lambda is passed the index and the entry corresponding to the index.
        template<typename Lambda>
        auto applied_lambda(const Lambda & lambda)
        {
            return Field<T, FieldLambda<T, Container, Lambda>>(
                FieldLambda<T, Container, Lambda>(this->contents(), lambda));
        }

        ///////////// Reductions ////////////////////////////////////////////////////////

        // Returns the sum over all entries of the Field
        T sum() const;

        // Returns the average over all entries of the Field
        T mean() const;

        // This is only valid for scalar contents
        // This will return the minimum value.
        scalar min() const;

        // This is only valid for scalar contents
        // This will return the maximum value.
        scalar max() const;

        // This is only valid for scalar contents
        // This will return the minimum and maximum value.
        std::pair<scalar, scalar> minmax() const;

        // This is only valid for Vector3 contents
        // Returns the minium and maximum value of the components of all vectorfield
        // entries
        scalar min_component() const;

        // This is only valid for Vector3 contents
        // Returns the minium and maximum value of the components of all vectorfield
        // entries
        scalar max_component() const;

        // This is only valid for Vector3 contents
        // Returns the minium and maximum value of the components of all vectorfield
        // entries
        std::pair<scalar, scalar> minmax_component() const;

        ///////////// VectorField Operations on self ////////////////////////////////////

        // For a VectorField, this returns a Field of the Vector3 norms
        auto norm() const
        {
            static_assert(
                std::is_same_v<T, Vector3>,
                "FMATH USAGE ERROR: norm() is only available on Field<Vector3>");
            if constexpr (std::is_same_v<T, Vector3>)
            {
                return Field<scalar, NormEx<T, Container>>(
                    NormEx<T, Container>(this->contents()));
            }
        }

        // For a VectorField, this returns a Field of the squared Vector3 norms
        auto squaredNorm() const
        {
            static_assert(
                std::is_same_v<T, Vector3>,
                "FMATH USAGE ERROR: squaredNorm() is only available on "
                "Field<Vector3>");
            if constexpr (std::is_same_v<T, Vector3>)
            {
                return Field<scalar, SquaredNormEx<T, Container>>(
                    SquaredNormEx<T, Container>(this->contents()));
            }
        }

        // Normalizes the Vector3 entries of a VectorField to norm 1.
        // If a norm is zero, nothing is done.
        // This function is applied immediately, not on assignment.
        void normalize()
        {
            static_assert(
                std::is_same_v<T, Vector3>,
                "FMATH USAGE ERROR: normalize() is only available on Field<Vector3>");
            if constexpr (std::is_same_v<T, Vector3>)
            {
                this->assign(Field<T, NormalizedEx<T, Container>>(
                    NormalizedEx<T, Container>(this->contents())));
            }
        }

        // Normalizes the Vector3 entries of a VectorField to norm 1.
        // If a norm is zero, nothing is done.
        auto normalized() const
        {
            static_assert(
                std::is_same_v<T, Vector3>,
                "FMATH USAGE ERROR: normalized() is only available on "
                "Field<Vector3>");
            if constexpr (std::is_same_v<T, Vector3>)
            {
                return Field<T, NormalizedEx<T, Container>>(
                    NormalizedEx<T, Container>(this->contents()));
            }
        }

        ///////////// VectorField Operations with others ////////////////////////////////

        // Element-wise dot-product between vector-fields, yielding a scalar-field
        template<typename Container2>
        auto dot(const Field<Vector3, Container2> & field) const
        {
            static_assert(
                std::is_same_v<T, Vector3>,
                "FMATH USAGE ERROR: dot() is only available on Field<Vector3>");
            if constexpr (std::is_same_v<T, Vector3>)
            {
                return Field<scalar, FieldDotFieldEx<T, Container, Container2>>(
                    FieldDotFieldEx<T, Container, Container2>(
                        this->contents(), field.contents()));
            }
        }

        // Element-wise dot-product between a vector-field and a vector, yielding a scalar-field
        auto dot(const Vector3 & vec) const
        {
            static_assert(
                std::is_same_v<T, Vector3>,
                "FMATH USAGE ERROR: dot() is only available on Field<Vector3>");
            if constexpr (std::is_same_v<T, Vector3>)
            {
                return Field<scalar, VectorDotFieldEx<T, Container>>(
                    VectorDotFieldEx<T, Container>(this->contents(), vec));
            }
        }

        // Element-wise cross-product between vector-fields, yielding a vector-field
        template<typename Container2>
        auto cross(const Field<Vector3, Container2> & field) const
        {
            static_assert(
                std::is_same_v<T, Vector3>,
                "FMATH USAGE ERROR: cross() is only available on Field<Vector3>");
            if constexpr (std::is_same_v<T, Vector3>)
            {
                return Field<Vector3, FieldCrossFieldEx<T, Container, Container2>>(
                    FieldCrossFieldEx<T, Container, Container2>(
                        this->contents(), field.contents()));
            }
        }

        // Element-wise cross-product between a vector-field and a vector, yielding a vector-field
        auto cross(const Vector3 & vec) const
        {
            static_assert(
                std::is_same_v<T, Vector3>,
                "FMATH USAGE ERROR: cross() is only available on Field<Vector3>");
            if constexpr (std::is_same_v<T, Vector3>)
            {
                return Field<Vector3, VectorCrossFieldEx<T, Container>>(
                    VectorCrossFieldEx<T, Container>(this->contents(), vec));
            }
        }

        ///////////// SubSet Extraction /////////////////////////////////////////////////

        // Extract a 1D slice of a Field
        auto slice(
            const std::size_t begin = 0, const std::optional<std::size_t> end = {},
            const std::size_t stride = 1)
        {
            return Field<T, SliceEx<T, Container>>(
                SliceEx<T, Container>(this->contents(), begin, end, stride));
        }

        // Extract a subset of a Field's values via a list of indices
        auto operator[](const std::vector<std::size_t> & indices)
        {
            return Field<T, SubSetEx<T, Container>>(
                SubSetEx<T, Container>(this->contents(), indices));
        }
    };
}