#include <catch2/catch_test_macros.hpp>
#include <cmath>
#include <numbers>
#include <array>

#include "types.hxx"

TEST_CASE("Calculates cross product") {
    Vector want(1, 0, 0);

    Vector v0(0, 1, 0);
    Vector v1(0, 0, 1);

    Vector cross = v0.cross(v1);

    REQUIRE(want == cross);
}
