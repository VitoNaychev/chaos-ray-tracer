#include <catch2/catch_test_macros.hpp>
#include <cmath>
#include <numbers>
#include <array>

#include "scene.hxx"
#include "types.hxx"

TEST_CASE("Dollies camera forward") {
    Vector want(0, 0, -5);

    Vector position(0, 0, 0);
    Matrix rotation;

    Camera cam(position, rotation);
    cam.dolly(-5);

    REQUIRE(want == cam.getPosition());
}

TEST_CASE("Jibs camera upward") {
    Vector want(0, 10, 0);

    Vector position(0, 0, 0);
    Matrix rotation;

    Camera cam(position, rotation);
    cam.jib(10);

    REQUIRE(want == cam.getPosition());
}

TEST_CASE("Trucks camera to the right") {
    Vector want(7, 0, 0);

    Vector position(0, 0, 0);
    Matrix rotation;

    Camera cam(position, rotation);
    cam.truck(7);

    REQUIRE(want == cam.getPosition());
}

TEST_CASE("Pans camera +30 degrees") {
    float angle = std::numbers::pi/6;
    std::array<std::array<float, 3>, 3> rotatedMatrix = {{
        {{std::cos(angle), 0.0f, -std::sin(angle)}},
        {{0.0f, 1.0f, 0.0f}},
        {{std::sin(angle), 0.0f, std::cos(angle)}}
    }};
    Matrix want {rotatedMatrix};
    

    Vector position(0, 0, 0);
    std::array<std::array<float, 3>, 3> initialMatrix = {{
        {{1.0f, 0.0f, 0.0f}},
        {{0.0f, 1.0f, 0.0f}},
        {{0.0f, 0.0f, 1.0f}}
    }};    
    Matrix rotation {initialMatrix};

    Camera cam(position, rotation);
    cam.pan(angle);

    REQUIRE(want == cam.getRotation());
}