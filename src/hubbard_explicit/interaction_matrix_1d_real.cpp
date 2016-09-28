#include <iostream>
#include <fstream>
using namespace std;

#include <ieompp/algebra/operator.hpp>
#include <ieompp/algebra/term.hpp>
#include <ieompp/discretization/linear.hpp>
#include <ieompp/io/file_header.hpp>
#include <ieompp/io/eigen_triplets.hpp>
#include <ieompp/models/hubbard_explicit/basis.hpp>
#include <ieompp/models/hubbard_explicit/matrix.hpp>
#include <ieompp/platform.hpp>
using namespace ieompp;

#include <boost/program_options.hpp>
namespace po = boost::program_options;

int main(int argc, char** argv)
{
    Eigen::initParallel();

    po::options_description description("Calculate the interaction matrix for the 1D Hubbard model on a linear lattice in real space\n\nOptions");
    description.add_options()
        ("help", "print this help message")
        ("N", po::value<size_t>()->default_value(16), "number of lattice sites")
        ("U", po::value<double>()->default_value(1.), "interaction strength")
        ("o", po::value<string>()->default_value("interaction_matrix_1d_real.txt"), "output file");

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, description), vm);
    po::notify(vm);

    if(vm.count("help")) {
        cout << description << '\n';
        return 1;
    }

    const auto N        = vm["N"].as<size_t>();
    const auto U        = vm["U"].as<double>();
    const auto out_path = vm["o"].as<string>();

    discretization::LinearDiscretization<double, size_t> lattice(N, 1.);

    using Operator = algebra::Operator<size_t, bool>;
    using Term     = algebra::Term<double, Operator>;

    // init operator basis
    hubbard::real_space::Basis3Operator<Term> basis(lattice);

    std::vector<Eigen::Triplet<double>> elements;
    hubbard::real_space::init_interaction_matrix(elements, basis, U);

    ofstream file(out_path.c_str());
    io::write_header(file, {get_description(Platform()), get_description<decltype(elements)>()});
    file << '\n' << basis.size() << 'x' << basis.size() << '\n';
    io::write_triplet_list(file, elements);
    file.close();

    return 0;
}
