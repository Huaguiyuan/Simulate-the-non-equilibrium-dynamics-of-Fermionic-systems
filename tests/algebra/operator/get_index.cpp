#include "operator.hpp"
#include <ieompp/algebra/operator/get_index.hpp>
using namespace ieompp::algebra;

// check IndexType
static_assert(std::is_same<IndexType<0, Operator1>::Type, std::size_t>::value,
              "Index 0 of Operator1 should be of type std::size_t");

static_assert(std::is_same<IndexType<0, Operator2>::Type, std::size_t>::value,
              "Index 0 of Operator2 should be of type std::size_t");
static_assert(std::is_same<IndexType<1, Operator2>::Type, bool>::value,
              "Index 1 of Operator2 should be of type bool");

static_assert(std::is_same<IndexType<0, Operator3>::Type, std::size_t>::value,
              "Index 0 of Operator3 should be of type std::size_t");
static_assert(std::is_same<IndexType<1, Operator3>::Type, bool>::value,
              "Index 1 of Operator3 should be of type bool");
static_assert(std::is_same<IndexType<2, Operator3>::Type, char>::value,
              "Index 2 of Operator3 should be of type char");

TEST_CASE("get_index1")
{
    const auto op = Operator1::make_creator(1ul);

    REQUIRE(get_index<0>(op) == 1ul);
}

TEST_CASE("get_index2")
{
    const auto op = Operator2::make_creator(1ul, true);

    REQUIRE(get_index<0>(op) == 1ul);
    REQUIRE(get_index<1>(op) == true);
}

TEST_CASE("get_index3")
{
    const auto op = Operator3::make_creator(1ul, true, 'a');

    REQUIRE(get_index<0>(op) == 1ul);
    REQUIRE(get_index<1>(op) == true);
    REQUIRE(get_index<2>(op) == 'a');
}
