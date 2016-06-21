#ifndef IEOMPP_ALGEBRA_TERM_HPP_
#define IEOMPP_ALGEBRA_TERM_HPP_

#include <algorithm>
#include <initializer_list>
#include <iterator>
#include <ostream>
#include <type_traits>
#include <vector>

namespace ieompp
{
    namespace algebra
    {
        template <typename PrefactorT, typename OperatorT,
                  typename ContainerT = std::vector<OperatorT>>
        struct Term {
            using Prefactor = PrefactorT;
            using Operator  = OperatorT;
            using Container = ContainerT;

            Prefactor prefactor;
            Container operators;

            Term conjugate() const
            {
                Term conj{prefactor, {}};
                std::reverse_copy(operators.begin(), operators.end(),
                                  std::back_inserter(conj.operators));
                for(auto& op : conj.operators) op.creator = !op.creator;
                return conj;
            }
        };

        template <typename Prefactor, typename Operator, typename Container = std::vector<Operator>>
        Term<Prefactor, Operator> make_term(Prefactor&& prefactor,
                                            std::initializer_list<Operator> operators)
        {
            return Term<Prefactor, Operator>{
                typename std::decay<Prefactor>::type(std::forward<Prefactor>(prefactor)),
                Container{operators}};
        }

        template <typename Prefactor, typename Operator, typename Container>
        std::ostream& operator<<(std::ostream& strm, const Term<Prefactor, Operator, Container>& t)
        {
            strm << t.prefactor << u8" ⋅ ";
            std::copy(t.operators.begin(), --t.operators.end(),
                      std::ostream_iterator<Operator>(strm, " "));
            strm << t.operators.back();
            return strm;
        }
    }
}

#endif
