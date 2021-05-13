#include <catch2/catch.hpp>
#include <factorial.h>

TEST_CASE("Check factorial")
{
    CHECK(factorial(3) == 6);
    CHECK(factorial(5) == 120);
}
