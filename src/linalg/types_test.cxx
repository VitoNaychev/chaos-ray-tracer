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

TEST_CASE("Vector array access with enum") {
    Vector v(1.5f, 2.5f, 3.5f);
    
    REQUIRE(v[AxisEnum::X] == 1.5f);
    REQUIRE(v[AxisEnum::Y] == 2.5f);
    REQUIRE(v[AxisEnum::Z] == 3.5f);
    
    v[AxisEnum::X] = 10.0f;
    REQUIRE(v[AxisEnum::X] == 10.0f);
}
