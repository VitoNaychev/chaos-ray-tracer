#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <cmath>
#include <numbers>

#include "types.hxx"
#include "raygen.hxx"

bool nearlyEqual(Vector a, Vector b, float epsilon = 1e-4) {
    Vector diff = a - b;

    return std::fabs(diff.x) < epsilon && std::fabs(diff.y) < epsilon && std::fabs(diff.z) < epsilon;
}

TEST_CASE("Get pixels for image with width and height") {
    std::vector<Pixel> want = {
        Pixel{
            .x = 0,
            .y = 0,
        }, 
        Pixel{
            .x = 0,
            .y = 1,
        }, 
        Pixel{
            .x = 1,
            .y = 0,
        },
        Pixel{
            .x = 1,
            .y = 1,
        }
    };

    int width = 2;
    int height = 2;

    std::vector<Pixel> got;

    got = getPixels(width, height);

    REQUIRE(want == got);
}

TEST_CASE("Get pixel centers") {
    std::vector<Pixel> want = {
        Pixel{
            .x = 0.5,
            .y = 0.5,
        }, 
        Pixel{
            .x = 0.5,
            .y = 1.5,
        }, 
        Pixel{
            .x = 1.5,
            .y = 0.5,
        },
        Pixel{
            .x = 1.5,
            .y = 1.5,
        }
    };

    std::vector<Pixel> pixels = {
        Pixel{
            .x = 0,
            .y = 0,
        }, 
        Pixel{
            .x = 0,
            .y = 1,
        }, 
        Pixel{
            .x = 1,
            .y = 0,
        },
        Pixel{
            .x = 1,
            .y = 1,
        }
    };

    getPixelCenters(pixels);

    REQUIRE(want == pixels);
}

TEST_CASE("Normalize pixels") {
    std::vector<Pixel> want = {
        Pixel{
            .x = 0.25,
            .y = 0.25,
        }, 
        Pixel{
            .x = 0.25,
            .y = 0.75,
        }, 
        Pixel{
            .x = 0.75,
            .y = 0.25,
        },
        Pixel{
            .x = 0.75,
            .y = 0.75,
        }
    };

    int width = 2;
    int height = 2;

    std::vector<Pixel> pixels = {
        Pixel{
            .x = 0.5,
            .y = 0.5,
        }, 
        Pixel{
            .x = 0.5,
            .y = 1.5,
        }, 
        Pixel{
            .x = 1.5,
            .y = 0.5,
        },
        Pixel{
            .x = 1.5,
            .y = 1.5,
        }
    };

    normalize(pixels, width, height);

    REQUIRE(want == pixels);
}

TEST_CASE("Get camera view") {
    std::vector<Pixel> want = {
        Pixel{
            .x = -0.5,
            .y = 0.5,
        }, 
        Pixel{
            .x = -0.5,
            .y = -0.5,
        }, 
        Pixel{
            .x = 0.5,
            .y = 0.5,
        },
        Pixel{
            .x = 0.5,
            .y = -0.5,
        }
    };

    int width = 2;
    int height = 2;

    std::vector<Pixel> pixels = {
        Pixel{
            .x = 0.25,
            .y = 0.25,
        }, 
        Pixel{
            .x = 0.25,
            .y = 0.75,
        }, 
        Pixel{
            .x = 0.75,
            .y = 0.25,
        },
        Pixel{
            .x = 0.75,
            .y = 0.75,
        }
    };

    getCV(pixels);

    REQUIRE(want == pixels);
}


TEST_CASE("Get vectors") {
    std::vector<Vector> want = {
        Vector(-0.5, 0.5, -1), 
        Vector(-0.5, -0.5, -1), 
        Vector(0.5, 0.5, -1),
        Vector(0.5, -0.5, -1)
    };

    std::vector<Pixel> pixels = {
        Pixel{
            .x = -0.5,
            .y = 0.5,
        }, 
        Pixel{
            .x = -0.5,
            .y = -0.5,
        }, 
        Pixel{
            .x = 0.5,
            .y = 0.5,
        },
        Pixel{
            .x = 0.5,
            .y = -0.5,
        }
    };

    std::vector<Vector> got;
    got = getVectors(pixels);

    REQUIRE(want == got);
}

TEST_CASE("Normalize vectors") {
    double angle = std::numbers::pi/4/3;
    std::vector<Vector> want = {
        Vector(float(std::sin(angle)), float(std::cos(angle)), 0), 
        Vector(float(std::sin(angle)), 0, float(std::cos(angle))), 
        Vector(0, float(std::sin(angle)), float(std::cos(angle))),
    };

    std::vector<Vector> vectors = {
        Vector(float(5*std::sin(angle)), float(5*std::cos(angle)), 0), 
        Vector(float(3*std::sin(angle)), 0, float(3*std::cos(angle))), 
        Vector(0, float(10*std::sin(angle)), float(10*std::cos(angle))),
    };

    normalizeVectors(vectors);

    REQUIRE(nearlyEqual(want[0], vectors[0]));
    REQUIRE(nearlyEqual(want[1], vectors[1]));
    REQUIRE(nearlyEqual(want[2], vectors[2]));
}
