#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_exception.hpp>

#include <rapidjson/document.h>
#include <rapidjson/istreamwrapper.h>

#include <iostream>

#include "crtscene.hpp"
#include "types.hxx"

using namespace std;
using namespace rapidjson;


TEST_CASE("Parses CRTScene file") {
    Settings settings {
        .width = 1920,
        .height = 1080,
        .background = {
            .r = 0,
            .g = 0.5,
            .b = 0,
        }
    };
    Vector position {0, 0, 0};
    Matrix rotation = Matrix::identity();
    vector<Vector> vertices = {
        {-1.75, -1.75, -3},
        {1.75, -1.75, -3},
        {0, 1.75, -3}
    };
    vector<int> triangleIndicies {0, 1, 2};


    stringstream input;
    input << "\
    {\
    \"settings\": {\
        \"image_settings\": {";
    input << "\"width\": " << settings.width << "," ;
    input << "\"height\": " << settings.height;
    input << "},";
    input << "\"background_color\":";
    input << "[" << settings.background.r << "," << settings.background.g << "," << settings.background.b << "]";
    input << "},";
    input << "\"camera\": {";
    input << "\"matrix\": [";
    input << rotation[0][0] << "," << rotation[0][1] << "," << rotation[0][2] << ",";
    input << rotation[1][0] << "," << rotation[1][1] << "," << rotation[1][2] << ",";
    input << rotation[2][0] << "," << rotation[2][1] << "," << rotation[2][2];
    input << "],";
    input << "\"position\":";
    input << "[" << position.x << "," << position.y << "," << position.z << "]";
    input << "},";
    input << "\"objects\": [";
    input << "{";
    input << "\"vertices\": [";
    input << vertices[0].x << "," << vertices[0].y << "," << vertices[0].z << ",";
    input << vertices[1].x << "," << vertices[1].y << "," << vertices[1].z << ",";
    input << vertices[2].x << "," << vertices[2].y << "," << vertices[2].z;
    input << "],";
    input << "\"triangles\":";
    input << "[" << triangleIndicies[0] << "," << triangleIndicies[1] << "," << triangleIndicies[2] << "]";
    input << "}";
    input << "]";
    input << "}";

    auto scene = parseCRTScene(input);

    REQUIRE(scene.settings.width == settings.width);
    REQUIRE(scene.settings.height == settings.height);
    REQUIRE(scene.settings.background == settings.background);

    REQUIRE(scene.camera.getPosition() == position);
    REQUIRE(scene.camera.getRotation() == rotation);

    REQUIRE(scene.objects[0].vertices == vertices);
    REQUIRE(scene.objects[0].triangleIndicies == triangleIndicies);
}

TEST_CASE("Throws exception on invalid JSON") {
    stringstream input;
    input << "{\"not-settings\":\"example-value\"";

    REQUIRE_THROWS_MATCHES(
        parseCRTScene(input),
        std::runtime_error,
        Catch::Matchers::Message("invalid JSON")
    );
}

TEST_CASE("Throws exception on no \"settings\" key") {
    stringstream input;
    input << "{\"not-settings\":\"example-value\"}";
    IStreamWrapper isw(input);

    Document doc;
    doc.ParseStream(isw);

    REQUIRE_THROWS_MATCHES(
        getSettings(doc),
        std::runtime_error,
        Catch::Matchers::Message("missing settings key")
    );
}

TEST_CASE("Throws exception on \"settings\" not an object") {
    stringstream input;
    input << "\
{\
    \"settings\": \"foo\"\
}";
    IStreamWrapper isw(input);

    Document doc;
    doc.ParseStream(isw);

    REQUIRE_THROWS_MATCHES(
        getSettings(doc),
        std::runtime_error,
        Catch::Matchers::Message("settings is not an object")
    );
}

TEST_CASE("Throws exception on no \"image_settings\" key inside \"settings\"") {
    stringstream input;
    input << "\
{\
    \"settings\": {\
        \"no-image_settings\": \"foo\"\
    }\
}";
    IStreamWrapper isw(input);

    Document doc;
    doc.ParseStream(isw);

    REQUIRE_THROWS_MATCHES(
        getSettings(doc),
        std::runtime_error,
        Catch::Matchers::Message("missing image_settings key")
    );
}

TEST_CASE("Throws exception on \"image_settings\" not an object") {
    stringstream input;
    input << "\
{\
    \"settings\": {\
        \"image_settings\": \"foo\"\
    }\
}";
    IStreamWrapper isw(input);

    Document doc;
    doc.ParseStream(isw);

    REQUIRE_THROWS_MATCHES(
        getSettings(doc),
        std::runtime_error,
        Catch::Matchers::Message("image_settings is not an object")
    );
}

TEST_CASE("Populates settings fields") {
    int wantWidth = 1920;
    int wantHeight = 1080;
    Color wantBackground = {
        .r = 0,
        .g = 0.5,
        .b = 0,
    };

    stringstream input;
    input << "\
{\
    \"settings\": {\
        \"image_settings\": {";
    input << "\"width\": " << wantWidth << "," ;
    input << "\"height\": " << wantHeight;
    input << "},";
    input << "\"background_color\":";
    input << "[" << wantBackground.r << "," << wantBackground.g << "," << wantBackground.b << "]";
    input << "\
    }\
}";

    IStreamWrapper isw(input);

    Document doc;
    doc.ParseStream(isw);

    Settings settings = getSettings(doc);

    REQUIRE(settings.width == wantWidth);
    REQUIRE(settings.height == wantHeight);
    REQUIRE(settings.background == wantBackground);
}


TEST_CASE("Throws exception on width not an integer") {

    stringstream input;
    input << "\
{\
    \"settings\": {\
        \"image_settings\": {";
    input << "\"width\": \"hello-world!\"," ;
    input << "\"height\": 1080\
        },";
    input << "\"background_color\": [0, 0, 0]";
    input << "\
    }\
}";
    IStreamWrapper isw(input);


    Document doc;
    doc.ParseStream(isw);

    REQUIRE_THROWS_MATCHES(
        getSettings(doc),
        std::runtime_error,
        Catch::Matchers::Message("width is not an integer")
    );
}

TEST_CASE("Throws exception on height not an integer") {

    stringstream input;
    input << "\
{\
    \"settings\": {\
        \"image_settings\": {";
    input << "\"width\": 1920," ;
    input << "\"height\": \"hello world!\"\
        },";
    input << "\"background_color\": [0, 0, 0]";
    input << "\
    }\
}";
    IStreamWrapper isw(input);

    Document doc;
    doc.ParseStream(isw);

    REQUIRE_THROWS_MATCHES(
        getSettings(doc),
        std::runtime_error,
        Catch::Matchers::Message("height is not an integer")
    );
}

TEST_CASE("Throws exception on missing width key") {
    stringstream input;
    input << "\
{\
    \"settings\": {\
        \"image_settings\": {";
    input << "\"height\": 1080\
        },";
    input << "\"background_color\": [0, 0, 0]";
    input << "\
    }\
}";
    IStreamWrapper isw(input);

    Document doc;
    doc.ParseStream(isw);

    REQUIRE_THROWS_MATCHES(
        getSettings(doc),
        std::runtime_error,
        Catch::Matchers::Message("missing width key")
    );
}

TEST_CASE("Throws exception on missing height key") {
    stringstream input;
    input << "\
{\
    \"settings\": {\
        \"image_settings\": {";
    input << "\"width\": 1920\
        },";
    input << "\"background_color\": [0, 0, 0]";
    input << "\
    }\
}";

    IStreamWrapper isw(input);

    Document doc;
    doc.ParseStream(isw);

    REQUIRE_THROWS_MATCHES(
        getSettings(doc),
        std::runtime_error,
        Catch::Matchers::Message("missing height key")
    );
}

TEST_CASE("Throws exception on missing background_color key") {
    stringstream input;
    input << "\
{\
    \"settings\": {\
        \"image_settings\": {";
    input << "\"width\": 1920," ;
    input << "\"height\": 1080\
        }";
    input << "\
    }\
}";

    IStreamWrapper isw(input);

    Document doc;
    doc.ParseStream(isw);

    REQUIRE_THROWS_MATCHES(
        getSettings(doc),
        std::runtime_error,
        Catch::Matchers::Message("missing background_color key")
    );
}

TEST_CASE("Throws exception on background_color not an array") {
    stringstream input;
    input << "\
{\
    \"settings\": {\
        \"image_settings\": {";
    input << "\"width\": 1920," ;
    input << "\"height\": 1080\
        },";
    input << "\"background_color\": \"foo\"";
    input << "\
    }\
}";

    IStreamWrapper isw(input);

    Document doc;
    doc.ParseStream(isw);

    REQUIRE_THROWS_MATCHES(
        getSettings(doc),
        std::runtime_error,
        Catch::Matchers::Message("background_color is not an array")
    );
}


TEST_CASE("Throws exception on background_color does not contain exactly 3 elements") {
    stringstream input;
    input << "\
{\
    \"settings\": {\
        \"image_settings\": {";
    input << "\"width\": 1920," ;
    input << "\"height\": 1080\
        },";
    input << "\"background_color\": [0, 0, 0, 1, 1, 1]";
    input << "\
    }\
}";

    IStreamWrapper isw(input);

    Document doc;
    doc.ParseStream(isw);

    REQUIRE_THROWS_MATCHES(
        getSettings(doc),
        std::runtime_error,
        Catch::Matchers::Message("background_color must contain exactly 3 elements")
    );
}

TEST_CASE("Throws exception on color not in the range [0-1]") {
    stringstream input;
    input << "\
{\
    \"settings\": {\
        \"image_settings\": {";
    input << "\"width\": 1920," ;
    input << "\"height\": 1080\
        },";
    input << "\"background_color\": [0.2, 255, 0.2]";
    input << "\
    }\
}";

    IStreamWrapper isw(input);

    Document doc;
    doc.ParseStream(isw);

    REQUIRE_THROWS_MATCHES(
        getSettings(doc),
        std::runtime_error,
        Catch::Matchers::Message("colors must be in the range [0-1]")
    );
}

// Camera parsing tests
TEST_CASE("Parser camera position and rotation") {
    Vector position {0, 0, 0};
    Matrix rotation = Matrix::identity();

    stringstream input;
    input << "\
{\
    \"camera\": {";
    input << "\"matrix\": [";
    input << rotation[0][0] << "," << rotation[0][1] << "," << rotation[0][2] << ",";
    input << rotation[1][0] << "," << rotation[1][1] << "," << rotation[1][2] << ",";
    input << rotation[2][0] << "," << rotation[2][1] << "," << rotation[2][2];
    input << "],";
    input << "\"position\": [";
    input << position.x << "," << position.y << "," << position.z;
    input << "]" ;
    input << "}\
}";

    // char line[300];
    // input.getline(line,300);
    // cout << line << endl;

    IStreamWrapper isw(input);

    Document doc;
    doc.ParseStream(isw);

    auto camera = getCamera(doc);

    REQUIRE(camera.getPosition() == position);
    REQUIRE(camera.getRotation() == rotation);
}

TEST_CASE("Throws exception on missing \"camera\" key") {
    Vector position {0, 0, 0};
    Matrix rotation = Matrix::identity();

    stringstream input;
    input << "\
{";
    input << "\"matrix\": [\
            1, 0, 0,\
            0, 1, 0,\
            0, 0, 1\
        ],";
    input << "\"position\": [\
            0, 0, 0\
        ]" ;
    input << "}";

    IStreamWrapper isw(input);

    Document doc;
    doc.ParseStream(isw);

    REQUIRE_THROWS_MATCHES(
        getCamera(doc),
        std::runtime_error,
        Catch::Matchers::Message("missing camera key")
    );
}

TEST_CASE("Throws exception if \"camera\" is not an object") {
    Vector position {0, 0, 0};
    Matrix rotation = Matrix::identity();

    stringstream input;
    input << "{\"camera\": \"hello-world!\"}";

    IStreamWrapper isw(input);

    Document doc;
    doc.ParseStream(isw);

    REQUIRE_THROWS_MATCHES(
        getCamera(doc),
        std::runtime_error,
        Catch::Matchers::Message("camera is not an object")
    );
}


TEST_CASE("Throws exception on missing \"matrix\" key") {
    Vector position {0, 0, 0};
    Matrix rotation = Matrix::identity();

    stringstream input;
    input << "\
{\
    \"camera\": {";
    input << "\"position\": [\
            0, 0, 0\
        ]" ;
    input << "}\
}";

    IStreamWrapper isw(input);

    Document doc;
    doc.ParseStream(isw);

    REQUIRE_THROWS_MATCHES(
        getCamera(doc),
        std::runtime_error,
        Catch::Matchers::Message("missing matrix key")
    );
}

TEST_CASE("Throws exception on \"matrix\" not an array") {
    Vector position {0, 0, 0};
    Matrix rotation = Matrix::identity();

    stringstream input;
    input << "\
{\
    \"camera\": {";
    input << "\"matrix\": \"foo bar\",";
    input << "\"position\": [\
            0, 0, 0\
        ]" ;
    input << "}\
}";

    IStreamWrapper isw(input);

    Document doc;
    doc.ParseStream(isw);


    REQUIRE_THROWS_MATCHES(
        getCamera(doc),
        std::runtime_error,
        Catch::Matchers::Message("matrix is not an array")
    );
}

TEST_CASE("Throws exception on \"matrix\" does not contain exactly 9 elements") {
    Vector position {0, 0, 0};
    Matrix rotation = Matrix::identity();

    stringstream input;
    input << "\
{\
    \"camera\": {";
    input << "\"matrix\": [\
            1, 0, 0,\
            0, 1, 0,\
            0, 0\
        ],";
    input << "\"position\": [\
            0, 0, 0\
        ]" ;
    input << "}\
}";

    IStreamWrapper isw(input);

    Document doc;
    doc.ParseStream(isw);


    REQUIRE_THROWS_MATCHES(
        getCamera(doc),
        std::runtime_error,
        Catch::Matchers::Message("matrix must contain exactly 9 elements")
    );
}

TEST_CASE("Throws exception on missing \"position\" key") {
    Vector position {0, 0, 0};
    Matrix rotation = Matrix::identity();

    stringstream input;
    input << "\
{\
    \"camera\": {";
    input << "\"matrix\": [\
            1, 0, 0,\
            0, 1, 0,\
            0, 0, 1\
        ]";
    input << "}\
}";

    IStreamWrapper isw(input);

    Document doc;
    doc.ParseStream(isw);

    REQUIRE_THROWS_MATCHES(
        getCamera(doc),
        std::runtime_error,
        Catch::Matchers::Message("missing position key")
    );
}

TEST_CASE("Throws exception on \"position\" not an array") {
    Vector position {0, 0, 0};
    Matrix rotation = Matrix::identity();

    stringstream input;
    input << "\
{\
    \"camera\": {";
    input << "\"matrix\": [\
            1, 0, 0,\
            0, 1, 0,\
            0, 0, 1\
        ],";
    input << "\"position\": \"foo bar\"";
    input << "}\
}";

    IStreamWrapper isw(input);

    Document doc;
    doc.ParseStream(isw);

    REQUIRE_THROWS_MATCHES(
        getCamera(doc),
        std::runtime_error,
        Catch::Matchers::Message("position is not an array")
    );
}

TEST_CASE("Throws exception on \"position\" does not contain exactly 3 elements") {
    Vector position {0, 0, 0};
    Matrix rotation = Matrix::identity();

    stringstream input;
    input << "\
{\
    \"camera\": {";
    input << "\"matrix\": [\
            1, 0, 0,\
            0, 1, 0,\
            0, 0, 1\
        ],";
    input << "\"position\": [\
            0, 0, 0, 0\
        ]" ;
    input << "}\
}";

    IStreamWrapper isw(input);

    Document doc;
    doc.ParseStream(isw);

    REQUIRE_THROWS_MATCHES(
        getCamera(doc),
        std::runtime_error,
        Catch::Matchers::Message("position must contain exactly 3 elements")
    );
}

TEST_CASE("Parses objects") {
    vector<Vector> vertices = {
        {-1.75, -1.75, -3},
        {1.75, -1.75, -3},
        {0, 1.75, -3}
    };
    vector<int> triangleIndicies {0, 1, 2};
    stringstream input;
    input << "\
{\
    \"objects\": [";
    input << "{";
    input << "\"vertices\": [";
    input << vertices[0].x << "," << vertices[0].y << "," << vertices[0].z << ",";
    input << vertices[1].x << "," << vertices[1].y << "," << vertices[1].z << ",";
    input << vertices[2].x << "," << vertices[2].y << "," << vertices[2].z;
    input << "],";
    input << "\"triangles\": [";
    input << triangleIndicies[0] << "," << triangleIndicies[1] << "," << triangleIndicies[2];
    input << "]";
    input << "}";
    input << "]\
}";

    IStreamWrapper isw(input);

    Document doc;
    doc.ParseStream(isw);

    auto objects = getObjects(doc);

    REQUIRE(objects[0].vertices == vertices);
    REQUIRE(objects[0].triangleIndicies == triangleIndicies);
}


TEST_CASE("Example objects test case") {
    vector<Vector> vertices = {
        {-1.75, -1.75, -3},
        {1.75, -1.75, -3},
        {0, 1.75, -3}
    };
    vector<int> triangleIndicies {0, 1, 2};
    stringstream input;
    input << "\
{\
    \"objects\": [";
    input << "{";
    input << "\"vertices\": [\
            -1.75, -1.75, -3,\
            1.75, -1.75, -3,\
            0, 1.75, -3\
        ],";
    input << "\"triangles\": [\
            0, 1, 2\
        ]" ;
    input << "}";
    input << "]\
}";

    IStreamWrapper isw(input);

    Document doc;
    doc.ParseStream(isw);
}