#pragma once

#include <cassert>
#include <vector>

#include <Eigen/Dense>

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
        // Field with initial size
        Field(const std::size_t n) : _container(n){}

        // Field with initial size and value
        Field(const std::size_t n, const T initialValue) : _container(n, initialValue){}

        // Constructor for underlying container
        Field(const Container& other) : _container(other){}

        // Assignment operator for Field of different type
        template<typename T2, typename R2>
        Field& operator= (const Field<T2, R2>& other)
        {
            assert(size() == other.size());
            assign(_container, other);
            return *this;
        }
        // A Field can be constructed such as to force its evaluation.
        template <typename T2, typename R2>
        Field(Field<T2, R2> const& other) : _container(other.size())
        {
            assign(_container, other);
        }

        // If the container is a std::vector, data can be retrieved as a pointer
        T* data() const
        { 
            static_assert(decltype(Container) == decltype(std::vector<T>),
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
    };
}

#include <detail/FieldFunctions.hpp>
#include <detail/FieldExpression.hpp>