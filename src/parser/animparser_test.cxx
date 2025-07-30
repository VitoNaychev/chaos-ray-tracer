#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_exception.hpp>

#include <rapidjson/document.h>
#include <rapidjson/istreamwrapper.h>

#include <iostream>

#include "animation.hxx"
#include "animparser.hxx"

using namespace std;

TEST_CASE("Parses animations file") {
    stringstream input;
    input << "{\"not-settings\":\"example-value\"";

    REQUIRE_THROWS_MATCHES(
        parseAnimation(input),
        std::runtime_error,
        Catch::Matchers::Message("invalid JSON")
    );
}

TEST_CASE("Throws exception on missing clip field") {
    stringstream input;
    input << "{\"not-clip\":\"example-value\"}";

    REQUIRE_THROWS_MATCHES(
        parseAnimation(input),
        std::runtime_error,
        Catch::Matchers::Message("missing clip key")
    );
}

TEST_CASE("Throws exception on clip field not array") {
    stringstream input;
    input << "{\"clip\":\"example-value\"}";

    REQUIRE_THROWS_MATCHES(
        parseAnimation(input),
        std::runtime_error,
        Catch::Matchers::Message("clip is not an array")
    );
}


TEST_CASE("Throws exception on missing type field") {
    stringstream input;
    input << "{\"clip\":[";
    input << "{";
    input << "\"pipi\": \"pan\"";
    input << "}";
    input <<"]}";

    REQUIRE_THROWS_MATCHES(
        parseAnimation(input),
        std::runtime_error,
        Catch::Matchers::Message("missing type key")
    );
}

TEST_CASE("Throws exception on type value not a string") {
    stringstream input;
    input << "{\"clip\":[";
    input << "{";
    input << "\"type\": 1";
    input << "}";
    input <<"]}";

    REQUIRE_THROWS_MATCHES(
        parseAnimation(input),
        std::runtime_error,
        Catch::Matchers::Message("type is not a string")
    );
}


TEST_CASE("Throws exception on units key not a float") {
    stringstream input;
    input << "{\"clip\":[";
    input << "{";
    input << "\"type\": \"pan\",";
    input << "\"units\": \"bar\"";
    input << "}";
    input <<"]}";

    REQUIRE_THROWS_MATCHES(
        parseAnimation(input),
        std::runtime_error,
        Catch::Matchers::Message("units is not a float")
    );
}


TEST_CASE("Throws exception on missing frames field") {
    stringstream input;
    input << "{\"clip\":[";
    input << "{";
    input << "\"type\": \"pan\",";
    input << "\"units\": 1.5,";
    input << "\"pipi\": \"foo\"";
    input << "}";
    input <<"]}";

    REQUIRE_THROWS_MATCHES(
        parseAnimation(input),
        std::runtime_error,
        Catch::Matchers::Message("missing frames key")
    );
}

TEST_CASE("Throws exception on frames value not an int") {
    stringstream input;
    input << "{\"clip\":[";
    input << "{";
    input << "\"type\": \"pan\",";
    input << "\"units\": 1.5,";
    input << "\"frames\": \"foo\"";
    input << "}";
    input <<"]}";

    REQUIRE_THROWS_MATCHES(
        parseAnimation(input),
        std::runtime_error,
        Catch::Matchers::Message("frames is not an integer")
    );
}


TEST_CASE("Parses movements file") {
    stringstream input;
    input << "{\"clip\":[";
    input << "{";
    input << "\"type\": \"pan\",";
    input << "\"units\": 60,";
    input << "\"frames\": 120";
    input << "},";
    input << "{";
    input << "\"type\": \"dolly\",";
    input << "\"units\": 1.5,";
    input << "\"frames\": 100";
    input << "}";
    input <<"]}";

    auto animation = parseAnimation(input);

    REQUIRE(animation.size() == 2);

    REQUIRE(animation[0].movement == MovementType::Pan);
    REQUIRE(animation[0].units == 60);

    REQUIRE(animation[0].framesCount == 120);

    REQUIRE(animation[1].movement == MovementType::Dolly);
    REQUIRE(animation[1].units == 1.5);

    REQUIRE(animation[1].framesCount == 100);
}
