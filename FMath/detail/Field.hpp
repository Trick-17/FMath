#pragma once

#include <cassert>
#include <vector>

#include <Eigen/Dense>

#include <detail/Using.hpp>

namespace FMath::detail
{
    // Field
    // template<typename T, typename Alloc = std::allocator<T> >
    // using Field       = std::vector<T, Alloc>;
    
    template<typename T, typename Container = std::vector<T>>
    class Field
    {
        Container _container;

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
            for (std::size_t i = 0; i < _container.size(); ++i)
                _container[i] = other[i];
            return *this;
        }
        // A Field can be constructed such as to force its evaluation.
        template <typename T2, typename R2>
        Field(Field<T2, R2> const& field) : _container(field.size())
        {
            for (size_t i = 0; i != field.size(); ++i)
            {
                _container[i] = field[i];
            }
        }

        // Size of underlying container
        std::size_t size()                          const { return _container.size(); }

        // Index operators
        T           operator[](const std::size_t i) const { return _container[i]; }
        T&          operator[](const std::size_t i)       { return _container[i]; }

        // Returns the underlying data
        const Container& data()                     const { return _container; }
        Container&       data()                           { return _container; }

        template <typename R2>
        Field<scalar> dot(const Field<Vector3,R2> & field)
        {
            static_assert(std::is_same_v<T, Vector3>, "Field<...>.dot(...) is only available on Field<Vector3>");

            // TODO: move this into a new expression
            assert(size() == field.size());
            Field<scalar> ret(size());
            for (size_t i = 0; i != size(); ++i)
            {
                ret[i] = _container[i].dot(field[i]);
            }
            return ret;
        }
        template <typename R2>
        Field<scalar> dot(const Vector3 & vec)
        {
            static_assert(std::is_same_v<T, Vector3>, "Field<...>.dot(...) is only available on Field<Vector3>");

            // TODO: move this into a new expression
            Field<scalar> ret(size());
            for (size_t i = 0; i != size(); ++i)
            {
                ret[i] = _container[i].dot(vec);
            }
            return ret;
        }

        template <typename R2>
        Field<Vector3> cross(const Field<Vector3,R2> & field)
        {
            static_assert(std::is_same_v<T, Vector3>, "Field<...>.cross(...) is only available on Field<Vector3>");
            
            // TODO: move this into a new expression
            assert(size() == field.size());
            Field<Vector3> ret(size());
            for (size_t i = 0; i != size(); ++i)
            {
                ret[i] = _container[i].cross(field[i]);
            }
            return ret;
        }
        template <typename R2>
        Field<Vector3> cross(const Vector3 & vec)
        {
            static_assert(std::is_same_v<T, Vector3>, "Field<...>.cross(...) is only available on Field<Vector3>");
            
            // TODO: move this into a new expression
            Field<Vector3> ret(size());
            for (size_t i = 0; i != size(); ++i)
            {
                ret[i] = _container[i].cross(vec);
            }
            return ret;
        }
    };

}
#include <detail/FieldExpression.hpp>