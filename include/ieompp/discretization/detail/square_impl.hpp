#include "ieompp/discretization/square.hpp"

#include <cassert>

#include "ieompp/constants.hpp"

namespace ieompp
{
    namespace discretization
    {
        template <typename Real, typename Index>
        SquareDiscretization<Real, Index>::SquareDiscretization(const Index& num_x,
                                                                const Index& num_y)
            : _num_x(num_x), _num_y(num_y), _num(num_x * num_y), _first(0), _last(_num - 1),
              _x_min(-Pi<Real>::value), _x_max(Pi<Real>::value), _x_length(TwoPi<Real>::value),
              _dx(TwoPi<Real>::value / num_x), _y_min(-Pi<Real>::value), _y_max(Pi<Real>::value),
              _y_length(TwoPi<Real>::value), _dy(TwoPi<Real>::value / num_y),
              _lattice_vectors{{Vector(_dx, 0.), Vector(0., _dy)}}
        {
            assert(_num_x > 0);
            assert(_num_y > 0);
            assert(_num > 0);
        }

        template <typename Real, typename Index>
        SquareDiscretization<Real, Index>::SquareDiscretization(const Index& num_x,
                                                                const Index& num_y, const Real& dx,
                                                                const Real& dy)
            : _num_x(num_x), _num_y(num_y), _num(num_x * num_y), _first(0), _last(_num - 1),
              _x_min(0.), _x_max((num_x - 1) * dx), _x_length(_num_x * dx), _dx(dx), _y_min(0.),
              _y_max((num_y - 1) * dy), _y_length(_num_y * dy), _dy(dy),
              _lattice_vectors{{Vector(_dx, 0.), Vector(0., _dy)}}
        {
            assert(_num_x > 0);
            assert(_num_y > 0);
            assert(_num > 0);
        }

        template <typename Real, typename Index>
        typename SquareDiscretization<Real, Index>::Index
        SquareDiscretization<Real, Index>::index(const Index& i, const Index& j) const
        {
            return i * _num_x + j;
        }

        template <typename Real, typename Index>
        std::array<typename SquareDiscretization<Real, Index>::Index, 4>
        SquareDiscretization<Real, Index>::neighbours(const Index& idx) const
        {
            const auto i = idx / _num_y;
            const auto j = idx % _num_x;
            return std::array<Index, 4>{
                {index((i == 0) ? _num_x - 1 : i - 1, j), index(i, (j == 0) ? _num_y - 1 : j - 1),
                 index((i == _num_x - 1) ? 0 : i + 1, j), index(i, (j == _num_y - 1) ? 0 : j + 1)}};
        }

        template <typename Real, typename Index>
        std::array<typename SquareDiscretization<Real, Index>::Index, 2>
        SquareDiscretization<Real, Index>::unique_neighbours(const Index& idx) const
        {
            const auto i = idx / _num_y;
            const auto j = idx % _num_x;
            return std::array<Index, 2>{
                {index((i == _num_x - 1) ? 0 : i + 1, j), index(i, (j == _num_y - 1) ? 0 : j + 1)}};
        }

        template <typename Real, typename Index>
        typename SquareDiscretization<Real, Index>::Index
        SquareDiscretization<Real, Index>::closest(Vector v) const
        {
            const auto dx2 = _dx / 2;
            const auto dy2 = _dy / 2;
            while(v(0) < _x_min - dx2) v(0) += _x_length;
            while(v(0) > _x_max + dx2) v(0) -= _x_length;
            while(v(1) < _y_min - dy2) v(1) += _y_length;
            while(v(1) > _y_max + dy2) v(1) -= _y_length;
            Real min_dist = (v - (*this)[Index(0)]).dot(v - (*this)[Index(0)]), dist;
            Index min = 0;
            for(auto idx : *this) {
                dist = (v - (*this)[idx]).dot(v - (*this)[idx]);
                if(dist < min_dist) {
                    std::swap(min_dist, dist);
                    min = idx;
                }
            }
            return min;
        }

        template <typename Real, typename Index>
        const typename SquareDiscretization<Real, Index>::Index&
        SquareDiscretization<Real, Index>::num() const
        {
            return _num;
        }

        template <typename Real, typename Index>
        const typename SquareDiscretization<Real, Index>::Index&
        SquareDiscretization<Real, Index>::first() const
        {
            return _first;
        }

        template <typename Real, typename Index>
        const typename SquareDiscretization<Real, Index>::Index&
        SquareDiscretization<Real, Index>::last() const
        {
            return _last;
        }

        template <typename Real, typename Index>
        const Real& SquareDiscretization<Real, Index>::x_min() const
        {
            return _x_min;
        }

        template <typename Real, typename Index>
        const Real& SquareDiscretization<Real, Index>::x_max() const
        {
            return _x_max;
        }

        template <typename Real, typename Index>
        const Real& SquareDiscretization<Real, Index>::x_length() const
        {
            return _x_length;
        }

        template <typename Real, typename Index>
        const Real& SquareDiscretization<Real, Index>::dx() const
        {
            return _dx;
        }

        template <typename Real, typename Index>
        const Real& SquareDiscretization<Real, Index>::y_min() const
        {
            return _y_min;
        }

        template <typename Real, typename Index>
        const Real& SquareDiscretization<Real, Index>::y_max() const
        {
            return _y_max;
        }

        template <typename Real, typename Index>
        const Real& SquareDiscretization<Real, Index>::y_length() const
        {
            return _y_length;
        }

        template <typename Real, typename Index>
        const Real& SquareDiscretization<Real, Index>::dy() const
        {
            return _dy;
        }

        template <typename Real, typename Index>
        const std::array<typename SquareDiscretization<Real, Index>::Vector, 2>&
        SquareDiscretization<Real, Index>::lattice_vectors() const
        {
            return _lattice_vectors;
        }

        template <typename Real, typename Index>
        typename SquareDiscretization<Real, Index>::ConstIndexIterator
        SquareDiscretization<Real, Index>::begin() const
        {
            return ConstIndexIterator(_first);
        }

        template <typename Real, typename Index>
        typename SquareDiscretization<Real, Index>::ConstIndexIterator
        SquareDiscretization<Real, Index>::end() const
        {
            return ConstIndexIterator(_last + 1);
        }

        template <typename Real, typename Index>
        typename SquareDiscretization<Real, Index>::IndexIterator
        SquareDiscretization<Real, Index>::begin()
        {
            return IndexIterator(_first);
        }

        template <typename Real, typename Index>
        typename SquareDiscretization<Real, Index>::IndexIterator
        SquareDiscretization<Real, Index>::end()
        {
            return IndexIterator(_last + 1);
        }

        template <typename Real, typename Index>
        typename SquareDiscretization<Real, Index>::ConstIndexIterator
        SquareDiscretization<Real, Index>::cbegin() const
        {
            return ConstIndexIterator(_first);
        }

        template <typename Real, typename Index>
        typename SquareDiscretization<Real, Index>::ConstIndexIterator
        SquareDiscretization<Real, Index>::cend() const
        {
            return ConstIndexIterator(_last + 1);
        }

        template <typename Real, typename Index>
        template <typename IndexT>
        typename std::enable_if<std::is_same<Index, IndexT>::value,
                                typename SquareDiscretization<Real, Index>::Vector>::type
            SquareDiscretization<Real, Index>::
            operator[](const IndexT& idx) const
        {
            const auto i = idx / _num_x;
            const auto j = idx % _num_x;
            return Vector(_x_min + i * _dx, _y_min + j * _dy);
        }

        template <typename Real, typename Index>
        template <typename VectorT>
        typename std::enable_if<std::is_same<typename SquareDiscretization<Real, Index>::Vector,
                                             VectorT>::value,
                                Index>::type SquareDiscretization<Real, Index>::
        operator()(VectorT v) const
        {
            while(v(0) < _x_min) v(0) += _x_length;
            while(v(0) > _x_max) v(0) -= _x_length;
            while(v(1) < _y_min) v(1) += _y_length;
            while(v(1) > _y_max) v(1) -= _y_length;
            const auto i = Index(std::round((v(0) - _x_min) / _dx));
            const auto j = Index(std::round((v(1) - _y_min) / _dy));
            return i * _num_x + j;
        }
    }
}