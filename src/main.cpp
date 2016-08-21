#include <iostream>
#include <vector>
using namespace std;

#include "ieompp/algebra/operator.hpp"
#include "ieompp/algebra/term.hpp"
#include "ieompp/discretization/square.hpp"
#include "ieompp/discretization/linear.hpp"
#include "ieompp/hubbard/explicit_momentum_space.hpp"
#include "ieompp/algebra/term_checks.hpp"
using namespace ieompp::algebra;

int main()
{
    const long N = 8;
    ieompp::discretization::LinearDiscretization<double, long> momentum_space(N);
    ieompp::discretization::LinearDiscretization<double, long> lattice(N, 1.);

    auto initial = make_term(1., {make_creator(0l, true)});
    /* auto initial = make_term(1., {make_creator(0l, true), make_creator(0l, false), make_annihilator(0l, false)}); */

    ieompp::hubbard::momentum_space::Hamiltonian<double> hamiltonian{1., 1.};
    std::vector<decltype(initial)> result;
    hamiltonian.generate_interaction_terms(initial, momentum_space, lattice, result);

    for(auto& t : result) {
        /* auto mom = ieompp::algebra::total_momentum<0>(t, momentum_space); */
        /* cout << mom(0) << "\t" << mom(1) << endl; */
        cout << t << endl;
    }

    return 0;
}
