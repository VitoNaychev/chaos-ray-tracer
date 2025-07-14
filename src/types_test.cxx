#include <catch2/catch_test_macros.hpp>
#include <cmath>
#include <numbers>
#include <array>

#include "types.hpp"

TEST_CASE("Calculates cross product") {
    Vector want {1, 0, 0};

    Vector v0 {0, 1, 0};
    Vector v1 {0, 0, 1};

    Vector cross = v0.cross(v1);

    REQUIRE(want == cross);
}

TEST_CASE("Returns false when ray is parallel with triangle normal") {
    Ray ray {
        .origin = {0, 0, 0},
        .direction = {0, 0, 1}
    };

    // Triangle parallel to X plane
    Vector v0 {-2, 2, 2}; 
    Vector v1 {2, 2, 2};
    Vector v2 {2, 2, 4};
    Triangle tri(v0, v1, v2);

    bool doesIntersect = tri.intersects(ray);
    REQUIRE(doesIntersect == false);
}

TEST_CASE("Returns false when ray is really close to being parallel to triangle normal") {
    Ray ray {
        .origin = {0, 0, 0},
        .direction = {0, 0, -1}
    };

    // Triangle parallel to X plane
    Vector v0 {-2, 2, -2}; 
    Vector v1 {2, 2, -2};
    Vector v2 {2, 2 + 1e-10, -4};
    Triangle tri(v0, v1, v2);

    bool doesIntersect = tri.intersects(ray);
    REQUIRE(doesIntersect == false);
}

TEST_CASE("Returns false in case of back-face culling") {
    Ray ray {
        .origin = {0, 0, 0},
        .direction = {0, 0, -1}
    };

    Vector v0 {-1.75, -1.75, -3};
    Vector v1 {1.75, -1.75, -3};
    Vector v2 {0, 1.75, -3};
    Triangle tri(v2, v1, v0);

    bool doesIntersect = tri.intersects(ray);
    REQUIRE(doesIntersect == false);
}

TEST_CASE("Returns true when ray intersects with triangle") {
    Ray ray {
        .origin = {0, 0, 0},
        .direction = {0, 0, -1}
    };

    Vector v0 {-1.75, -1.75, -3};
    Vector v1 {1.75, -1.75, -3};
    Vector v2 {0, 1.75, -3};
    Triangle tri(v0, v1, v2);

    bool doesIntersect = tri.intersects(ray);
    REQUIRE(doesIntersect == true);
}

TEST_CASE("Returns false when ray doesn't intersect with triangle") {
    Ray ray {
        .origin = {0, 0, 0},
       .direction = {0, 0, 1}
    };

    Vector v0 {-2, 2, 2};
    Vector v1 {2, 2, 2};
    Vector v2 {2, 4, 2};
    Triangle tri(v0, v1, v2);

    bool doesIntersect = tri.intersects(ray);
    REQUIRE(doesIntersect == false);
}

TEST_CASE("Dollies camera forward") {
    Vector want {.z = -5};

    Vector position {.z = 0};
    Matrix rotation;

    Camera cam(position, rotation);
    cam.dolly(-5);

    REQUIRE(want == cam.getPosition());
}

TEST_CASE("Jibs camera upward") {
    Vector want {.y = 10};

    Vector position {.y = 0};
    Matrix rotation;

    Camera cam(position, rotation);
    cam.jib(10);

    REQUIRE(want == cam.getPosition());
}

TEST_CASE("Trucks camera to the right") {
    Vector want {.x = 7};

    Vector position {.x = 0};
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
    

    Vector position {.x = 0};
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