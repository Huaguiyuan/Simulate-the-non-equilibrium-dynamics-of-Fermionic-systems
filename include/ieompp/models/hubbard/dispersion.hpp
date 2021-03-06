#ifndef IEOMPP_MODELS_HUBBARD_DISPERSION_HPP_
#define IEOMPP_MODELS_HUBBARD_DISPERSION_HPP_

#include "ieompp/constants.hpp"
#include "ieompp/types/dot_product.hpp"
#include "ieompp/types/matrix.hpp"

#include <cmath>
#include <vector>

namespace ieompp
{
    namespace models
    {
        namespace hubbard
        {
            template <typename Float>
            Float calculate_fermi_momentum_1d(const Float& filling_factor)
            {
                return filling_factor * Pi<Float>::value;
            }

            template <typename FloatT>
            class Dispersion
            {
            public:
                using Float = FloatT;

            private:
                std::vector<Float> _values;

            public:
                template <typename MomentumSpace, typename Lattice>
                Dispersion(const MomentumSpace& momentum_space, const Lattice& lattice,
                           const Float& J = 1.)
                    : _values(momentum_space.size(), Float(0.))
                {
                    const auto N = momentum_space.size();

                    const auto lattice_vectors = lattice.lattice_vectors();

#pragma omp parallel for
                    for(typename MomentumSpace::SiteIndex i = 0; i < N; ++i) {
                        const auto& momentum = momentum_space[i];
                        Float val            = 0.;
                        for(const auto& vec : lattice_vectors) {
                            val += std::cos(types::dot_product(momentum, vec));
                        }
                        _values[i] = -2. * J * val;
                    }
                }

                const Float& operator()(typename std::vector<Float>::size_type idx) const
                {
                    return _values[idx];
                }
            };

            template <typename MomentumSpace, typename Lattice>
            Dispersion<typename MomentumSpace::Float>
            make_dispersion(const MomentumSpace& momentum_space, const Lattice& lattice,
                            const typename MomentumSpace::Float& J = 1.)
            {
                return Dispersion<typename MomentumSpace::Float>(momentum_space, lattice, J);
            }
        } // namespace hubbard
    }     // namespace models
} // namespace ieompp

#endif
