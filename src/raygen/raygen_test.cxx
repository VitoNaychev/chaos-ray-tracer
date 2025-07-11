#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <cmath>
#include <numbers>

#include "types.hpp"
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
        Vector{
            .x = -0.5,
            .y = 0.5,
            .z = -1,
        }, 
        Vector{
            .x = -0.5,
            .y = -0.5,
            .z = -1,
        }, 
        Vector{
            .x = 0.5,
            .y = 0.5,
            .z = -1,
        },
        Vector{
            .x = 0.5,
            .y = -0.5,
            .z = -1,
        }
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
        Vector{
            .x = float(std::sin(angle)),
            .y = float(std::cos(angle)),
            .z = 0,
        }, 
        Vector{
            .x = float(std::sin(angle)),
            .y = 0,
            .z = float(std::cos(angle)),
        }, 
        Vector{
            .x = 0,
            .y = float(std::sin(angle)),
            .z = float(std::cos(angle)),
        },
    };

    std::vector<Vector> vectors = {
        Vector{
            .x = float(5*std::sin(angle)),
            .y = float(5*std::cos(angle)),
            .z = 0,
        }, 
        Vector{
            .x = float(3*std::sin(angle)),
            .y = 0,
            .z = float(3*std::cos(angle)),
        }, 
        Vector{
            .x = 0,
            .y = float(10*std::sin(angle)),
            .z = float(10*std::cos(angle)),
        },
    };

    normalizeVectors(vectors);

    REQUIRE(nearlyEqual(want[0], vectors[0]));
    REQUIRE(nearlyEqual(want[1], vectors[1]));
    REQUIRE(nearlyEqual(want[2], vectors[2]));
}
