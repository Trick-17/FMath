#include <detail/Field.hpp>

namespace FMath::detail
{
    // Elementwise element extraction via a specified slice
    template<typename T, typename Container>
    class SliceEx
    {
        Container & _container;
        int begin, end, stride;

    public:
        SliceEx(Container & container, int i_begin, std::optional<int> i_end, int i_stride): _container(container), begin(i_begin), stride(i_stride)
        {
            if (i_end)
                end = *i_end;
            else
                end = _container.size()-1;

            assert(begin  >= 0);
            assert(end    <  _container.size());
            assert(stride <= end-begin);
        }

        T operator[](const std::size_t i) const
        {
            return _container[begin + i*stride];
        }

        T& operator[](const std::size_t i)
        {
            return _container[begin + i*stride];
        }

        auto size() const
        {
            return (end - begin) / stride;
        }
    };
}