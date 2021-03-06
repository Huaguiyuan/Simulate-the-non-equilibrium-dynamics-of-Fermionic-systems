#ifndef IEOMPP_MODELS_HUBBARD_REAL_SPACE_BASIS_HPP_
#define IEOMPP_MODELS_HUBBARD_REAL_SPACE_BASIS_HPP_

#include "ieompp/models/hubbard/operator_traits.hpp"

#include <vector>

namespace ieompp
{
    namespace models
    {
        namespace hubbard_real_space
        {
            template <typename MonomialT>
            struct Basis1Operator : public std::vector<MonomialT> {
                using Monomial   = MonomialT;
                using BasisIndex = typename std::vector<Monomial>::size_type;

                const BasisIndex N;

                template <typename Lattice>
                Basis1Operator(const Lattice& lattice) : N(lattice.size())
                {
                    static_assert(hubbard::IsHubbardOperator<typename Monomial::Operator>::value,
                                  "Operator must be of Hubbard type");

                    this->reserve(N);

                    for(auto i : lattice) {
                        this->emplace_back(Monomial{{{true, i, true}}});
                    }
                }

                Basis1Operator get_conjugate() const
                {
                    Basis1Operator conj_basis(*this);

                    const auto size = this->size();
#pragma omp parallel for
                    for(BasisIndex i = 0; i < size; ++i) {
                        conj_basis[i].conjugate();
                    }

                    return conj_basis;
                }
            };

            template <typename MonomialT>
            struct Basis3Operator : public std::vector<MonomialT> {
                using Monomial   = MonomialT;
                using BasisIndex = typename std::vector<Monomial>::size_type;

                const BasisIndex N;
                const BasisIndex N_squared;

                template <typename Lattice>
                Basis3Operator(const Lattice& lattice) : N(lattice.size()), N_squared(N * N)
                {
                    static_assert(hubbard::IsHubbardOperator<typename Monomial::Operator>::value,
                                  "Operator must be of Hubbard type");

                    this->reserve(N * (N * N + 1));

                    for(auto i : lattice) {
                        this->emplace_back(Monomial{{{true, i, true}}});
                    }
                    for(auto i1 : lattice) {
                        for(auto i2 : lattice) {
                            for(auto i3 : lattice) {
                                this->emplace_back(Monomial{
                                    {{true, i1, true}, {true, i2, false}, {false, i3, false}}});
                            }
                        }
                    }
                }

                BasisIndex get_3op_index(BasisIndex i1, BasisIndex i2, BasisIndex i3) const
                {
                    return N + N_squared * i1 + N * i2 + i3;
                }

                Basis3Operator get_conjugate() const
                {
                    Basis3Operator conj_basis(*this);

                    const auto size = this->size();
#pragma omp parallel for
                    for(BasisIndex i = 0; i < size; ++i) {
                        conj_basis[i].conjugate();
                    }

                    return conj_basis;
                }
            };

            template <typename Basis>
            struct IsOneOperatorBasis {
                static constexpr bool value = false;
            };

            template <typename Basis>
            struct IsThreeOperatorBasis {
                static constexpr bool value = false;
            };

            template <typename Monomial>
            struct IsOneOperatorBasis<Basis1Operator<Monomial>> {
                static constexpr bool value = true;
            };

            template <typename Monomial>
            struct IsThreeOperatorBasis<Basis3Operator<Monomial>> {
                static constexpr bool value = true;
            };
        } // namespace hubbard_real_space
    }     // namespace models
} // namespace ieompp

#endif
