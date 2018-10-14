#pragma once

#include <cassert>
#include <vector>

#include <Eigen/Core>

#include <FMath/detail/Expression_SubSet.hpp>
#include <FMath/detail/Expression_Slice.hpp>
#include <FMath/detail/Expression_Function.hpp>
#include <FMath/detail/Using.hpp>

namespace FMath::detail
{
    template<typename T, typename Container = std::vector<T>>
    class Field
    {
        Container _container;
        
        // Assignment function which explicitly evaluates an expression
        template <typename Container2>
        void assign(Container & container_to, const Container2 & container_from);

    public:
        ///////////// Constructors //////////////////////////////////////////////////////////

        // Field with initial size
        Field(const std::size_t n) : _container(n) {}

        // Field with initial size and value
        Field(const std::size_t n, const T initialValue) : _container(n, initialValue) {}

        // Field via initializer list
        Field(std::initializer_list<T> list) : _container(list) {}

        // Constructor for underlying container
        Field(const Container& other) : _container(other) {}

        // Assignment operator for Field of different type
        template<typename T2, typename R2>
        Field& operator= (const Field<T2, R2>& other)
        {
            assert(size() == other.size());
            this->assign(_container, other);
            return *this;
        }

        // A Field can be constructed such as to force its evaluation.
        template <typename T2, typename R2>
        Field(Field<T2, R2> const& other) : _container(other.size())
        {
            this->assign(_container, other);
        }

        ///////////// Basics ////////////////////////////////////////////////////////////////

        // If the container is a std::vector, data can be retrieved as a pointer
        T* data()
        { 
            static_assert(std::is_same_v<Container, std::vector<T>>,
                "data() is only available for evaluated Fields, not Expressions");
            return &_container[0];
        }

        // Size of underlying container
        auto size() const { return _container.size(); }

        // Index operators
        T  operator[](const std::size_t i) const { return _container[i]; }
        T& operator[](const std::size_t i)       { return _container[i]; }

        // Returns the underlying data
        const Container& contents() const { return _container; }
        Container&       contents()       { return _container; }

        ///////////// Transformation ////////////////////////////////////////////////////////

        // Re-interpretation as a reference to an Eigen::VectorX
        template <typename RefT>
        Eigen::Ref<RefT> asRef()
        {
            // Field<Vector3> to VectorX of size 3*N
            if constexpr (std::is_same_v<T, Vector3>)
                return Eigen::Ref<RefT>( Eigen::Map<RefT>(this->_container[0].data(), 3*this->size()) );
            // Field<scalar> etc. to VectorX of size N
            else
                return Eigen::Ref<RefT>( Eigen::Map<RefT>(this->data(), this->size()) );
        }

        ///////////// Reductions ////////////////////////////////////////////////////////////

        // Returns the sum over all entries of the Field
        T sum();

        // Returns the average over all entries of the Field
        T mean();

        // This is only valid for scalar contents
        // This will return the minimum value.
        scalar min();

        // This is only valid for scalar contents
        // This will return themaximum value.
        scalar max();

        // This is only valid for scalar contents
        // This will return the minimum and maximum value.
        std::pair<scalar, scalar> minmax();

        // This is only valid for Vector3 contents
        // Returns the minium and maximum value of the components of all vectorfield entries
        scalar min_component();

        // This is only valid for Vector3 contents
        // Returns the minium and maximum value of the components of all vectorfield entries
        scalar max_component();

        // This is only valid for Vector3 contents
        // Returns the minium and maximum value of the components of all vectorfield entries
        std::pair<scalar, scalar> minmax_component();

        ///////////// VectorField Operations on self ////////////////////////////////////////

        // For a VectorField, this returns a Field of the Vector3 norms
        auto norm()
        {
            return Field<scalar, NormEx<T, Container>>(NormEx<T, Container>(contents()));
        }

        // For a VectorField, this returns a Field of the squared Vector3 norms
        auto squaredNorm()
        {
            return Field<scalar, SquaredNormEx<T, Container>>(SquaredNormEx<T, Container>(contents()));
        }

        // Normalizes the Vector3 entries of a VectorField to norm 1.
        // If a norm is zero, nothing is done.
        void normalize()
        {
            return Field<T, NormalizeEx<T, Container>>(NormalizeEx<T, Container>(contents()));
        }

        // Normalizes the Vector3 entries of a VectorField to norm 1.
        // If a norm is zero, nothing is done.
        auto normalized()
        {
            return Field<T, NormalizedEx<T, Container>>(NormalizedEx<T, Container>(contents()));
        }

        ///////////// VectorField Operations with others ////////////////////////////////////

        // Element-wise dot-product between vector-fields, yielding a scalar-field
        template <typename Container2>
        auto dot(const Field<Vector3, Container2> & field)
        {
            return Field<scalar, FieldDotFieldEx<T, Container, Container2>>(FieldDotFieldEx<T, Container, Container2>(contents(), field.contents()));
        }

        // Element-wise dot-product between a vector-field and a vector, yielding a scalar-field
        auto dot(const Vector3 & vec)
        {
            return Field<scalar, VectorDotFieldEx<T, Container>>(VectorDotFieldEx<T, Container>(contents(), vec));
        }

        // Element-wise cross-product between vector-fields, yielding a vector-field
        template <typename Container2>
        auto cross(const Field<Vector3,Container2> & field)
        {
            return Field<Vector3, FieldCrossFieldEx<T, Container, Container2>>(FieldCrossFieldEx<T, Container, Container2>(contents(), field.contents()));
        }

        // Element-wise cross-product between a vector-field and a vector, yielding a vector-field
        auto cross(const Vector3 & vec)
        {
            return Field<Vector3, VectorCrossFieldEx<T, Container>>(VectorCrossFieldEx<T, Container>(contents(), vec));
        }

        ///////////// SubSet Extraction /////////////////////////////////////////////////////

        // Extract a 1D slice of a Field
        auto slice(int begin=0, std::optional<int> end={}, int stride=1)
        {
            return Field<T, SliceEx<T, Container>>(SliceEx<T, Container>(contents(), begin, end, stride));
        }

        // Extract a subset of a Field's values via a list of indices
        auto operator[] (const std::vector<int> & indices)
        {
            return Field<T, SubSetEx<T, Container>>(SubSetEx<T, Container>(contents(), indices));
        }
    };
}