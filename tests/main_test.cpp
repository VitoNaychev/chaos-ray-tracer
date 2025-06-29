#include <catch2/catch_test_macros.hpp>
#include <fstream>
#include <sstream>
#include <iostream>

#include "../src/ppm.hpp"
#include "../src/pixeldrawer.hpp"

using namespace std;

TEST_CASE("Example test case") {
    REQUIRE(1 + 1 == 2);
}




TEST_CASE("Writes PPM header") {
    int wantWidth = 250;
    int wantHeight = 100;
    stringstream buffer;

    PixelDrawer* drawer = new PPMPixelDrawer(buffer, wantWidth, wantHeight);

    string line;

    // Writes correct file format
    REQUIRE(getline(buffer, line));
    REQUIRE(line  == "P3");

    // Writes image width and height
    REQUIRE(getline(buffer, line));
    REQUIRE(line  == "250 100");

    // Writes the max of uint8 for color component
    REQUIRE(getline(buffer, line));
    REQUIRE(line  == "255");
}

TEST_CASE("Draws a pixel") {
    Color c = Color {
        .r = 100,
        .g = 0,
        .b = 200
    };

    int wantWidth = 250;
    int wantHeight = 100;
    stringstream buffer;

    PixelDrawer* drawer = new PPMPixelDrawer(buffer, wantWidth, wantHeight);

    string line;
    drawer->draw(c);

    string got_red, got_green, got_blue;
    buffer >> got_red >> got_green >> got_blue;

    REQUIRE(to_string(c.r) == got_red);
    REQUIRE(to_string(c.g) == got_green);
    REQUIRE(to_string(c.b) == got_blue);
}

TEST_CASE("Draws two pixels") {
    Color c = Color {
        .r = 100,
        .g = 0,
        .b = 200
    };

    int wantWidth = 250;
    int wantHeight = 100;
    stringstream buffer;

    PixelDrawer* drawer = new PPMPixelDrawer(buffer, wantWidth, wantHeight);

    string line;
    drawer->draw(c);
    drawer->draw(c);
    string got_red, got_green, got_blue;

    buffer >> got_red >> got_green >> got_blue;
    REQUIRE(to_string(c.r) == got_red);
    REQUIRE(to_string(c.g) == got_green);
    REQUIRE(to_string(c.b) == got_blue);

    buffer >> got_red >> got_green >> got_blue;
    REQUIRE(to_string(c.r) == got_red);
    REQUIRE(to_string(c.g) == got_green);
    REQUIRE(to_string(c.b) == got_blue);
}
