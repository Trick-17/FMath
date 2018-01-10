#pragma once

#include <cassert>
#include <vector>

#include <Eigen/Core>

#include <detail/SubSet.hpp>
#include <detail/Using.hpp>

namespace FMath::detail
{
    template<typename T, typename Container = std::vector<T>>
    class Field
    {
        Container _container;
        
        // Assignment function to expressly evaluate an expression
        template <typename Container2>
        void assign(Container & container_to, const Container2 & container_from);

    public:
        //////// Constructors ///////////////////////////////////////////

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

        //////// Basics /////////////////////////////////////////////////

        // If the container is a std::vector, data can be retrieved as a pointer
        T* data()
        { 
            static_assert(std::is_same_v<Container, std::vector<T>>,
                "data() is only available for evaluated Fields, not Expressions");
            return &_container[0];
        }

        // Size of underlying container
        std::size_t size()                         const { return _container.size(); }

        // Index operators
        T           operator[](const std::size_t i) const { return _container[i]; }
        T&          operator[](const std::size_t i)       { return _container[i]; }

        // Returns the underlying data
        const Container& contents()                 const { return _container; }
        Container&       contents()                       { return _container; }

        //////// Transformation /////////////////////////////////////////

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

        //////// Reductions /////////////////////////////////////////////

        // Returns the sum over all entries of the Field
        T sum();

        // Returns the average over all entries of the Field
        T mean();

        // TODO: reconsider what this function should do
        // If T is a scalar type, this will return the N-dimensional norm
        // If T is a non-scalar type this will return the 3N-dimensional norm
        scalar norm();

        // TODO: reconsider what this function should do
        // If T is a scalar type, this will return the minimum value.
        // If T is a non-scalar type this will return the element with minimum norm
        T min();

        // TODO: reconsider what this function should do
        // If T is a scalar type, this will return the maximum value.
        // If T is a non-scalar type this will return the element with maximum norm
        T max();

        // TODO: reconsider what this function should do
        // If T is a scalar type, this will return the minimum and maximum value.
        // If T is a non-scalar type this will return the elements with minimum and maximum norm
        std::pair<T, T> minmax();

        // TODO: reconsider what this function should do
        // This is only valid for Vector3 contents
        // Returns the minium and maximum value of the components of all vectorfield entries
        std::pair<scalar, scalar> minmax_component();

        //////// Operations on self /////////////////////////////////////

        // TODO: reconsider what this function should do
        // Normalizes the field components to norm 1.
        // If norm is zero, nothing is done.
        void normalize();

        // TODO: reconsider what this function should do
        // Normalizes the field components to norm 1.
        // If norm is zero, nothing is done.
        Field normalized();

        //////// Operations with others /////////////////////////////////

        // Element-wise dot-product between vector-fields, yielding a scalar-field
        template <typename Container2>
        Field<scalar> dot(const Field<Vector3,Container2> & field);

        // Element-wise dot-product between a vector-field and a vector, yielding a scalar-field
        template <typename Container2>
        Field<scalar> dot(const Vector3 & vec);

        // Element-wise cross-product between vector-fields, yielding a vector-field
        template <typename Container2>
        Field<Vector3> cross(const Field<Vector3,Container2> & field);

        // Element-wise cross-product between a vector-field and a vector, yielding a vector-field
        template <typename Container2>
        Field<Vector3> cross(const Vector3 & vec);

        //////// SubSet Extraction //////////////////////////////////////

        // Extract a subset of a Field's values via a list of indices
        Field<T, SubSet<T, Container>> operator[] (const std::vector<int> & indices)
        {
            return Field<T, SubSet<T, Container>>(SubSet<T, Container>(contents(), indices));
        }
    };
}