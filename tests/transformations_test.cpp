#include <catch2/catch_test_macros.hpp>
#include <iostream>

#include "../src/types.hpp"
#include "../src/transformations.hpp"

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

    std::vector<Vector> vectors = {
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

    normalizeVectors(vectors);

    REQUIRE(want == vectors);
}