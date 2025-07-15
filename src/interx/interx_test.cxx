#include <catch2/catch_test_macros.hpp>
#include <cmath>
#include <numbers>

#include "interx.hxx"
#include "types.hxx"

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

    bool doesIntersect = intersectExists(ray, tri);
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

    bool doesIntersect = intersectExists(ray, tri);
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

    bool doesIntersect = intersectExists(ray, tri);
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

    bool doesIntersect = intersectExists(ray, tri);
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

    bool doesIntersect = intersectExists(ray, tri);
    REQUIRE(doesIntersect == false);
}