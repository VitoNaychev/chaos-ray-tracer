#include <rapidjson/document.h>
#include <rapidjson/istreamwrapper.h>
#include <iostream>
#include <format>

#include "types.hpp"

using namespace rapidjson;
using namespace std;

Value& requireObject(Value& doc, const char* name) {
    if (!doc.HasMember(name)) {
        throw runtime_error(std::format("missing {} key", name));
    }
    if (!doc[name].IsObject()) {
        throw runtime_error(std::format("{} is not an object", name));
    }

    return doc[name];
}

Value& requireArray(Value& doc, const char* name) {
    if (!doc.HasMember(name)) {
        throw runtime_error(std::format("missing {} key", name));
    }
    if (!doc[name].IsArray()) {
        throw runtime_error(std::format("{} is not an array", name));
    }

    return doc[name];
}

Value& requireArray(Value& doc, const char* name, int capacity) {
    if (!doc.HasMember(name)) {
        throw runtime_error(std::format("missing {} key", name));
    }
    if (!doc[name].IsArray()) {
        throw runtime_error(std::format("{} is not an array", name));
    }
    if (doc[name].Capacity() != capacity) {
        throw runtime_error(std::format("{} must contain exactly {} elements", name, capacity));
    }

    return doc[name];
}

int requireInt(Value& doc, const char* name) {
    if (!doc.HasMember(name)) {
        throw runtime_error(format("missing {} key", name));
    }
    if (!doc[name].IsInt()) {
        throw runtime_error(format("{} is not an integer", name));
    }

    return doc[name].GetInt();
}

Settings getSettings(Document& doc) {
     auto& settings = requireObject(doc, "settings");

    auto& image_settings = requireObject(settings, "image_settings");
    
   
    int width = requireInt(image_settings, "width");
    int height = requireInt(image_settings, "height");


    auto& backgroundField = requireArray(settings, "background_color", 3);

    float r = backgroundField[0].GetFloat();
    float g = backgroundField[1].GetFloat();
    float b = backgroundField[2].GetFloat();
    if ((r < 0 || r > 1) || (g < 0 || g > 1) || (b < 0 || b > 1)) {
        throw runtime_error("colors must be in the range [0-1]");
    }

    return Settings {
        .width = width,
        .height = height,
        .background = Color {
            .r = r,
            .g = g,
            .b = b,
        },
    };
}

Camera getCamera(Document& doc) {
    auto& camera = requireObject(doc, "camera");

    auto& rotationField = requireArray(camera, "matrix", 9);

    array<array<float, 3>, 3> matrixArr{};
    for (size_t i = 0; i < 3; ++i) {
        for (size_t j = 0; j < 3; ++j) {
            matrixArr[i][j] = rotationField[i * 3 + j].GetFloat();
        }
    }
    Matrix rotation(matrixArr);

    auto& positionField = requireArray(camera, "position", 3);
    Vector position {
        .x = positionField[0].GetFloat(),
        .y = positionField[1].GetFloat(),
        .z = positionField[2].GetFloat(),
    };
    
    return Camera(position, rotation);
}

vector<Mesh> getObjects(rapidjson::Document& doc) {
    vector<Mesh> objects;

    auto& objectsField = requireArray(doc, "objects");
    for (auto& object : objectsField.GetArray()) {
        auto& verticesField = requireArray(object, "vertices");
        vector<Vector> vertices;
        for (size_t i = 0; i < verticesField.Capacity(); i+=3) {
            vertices.push_back({
                .x = verticesField[i].GetFloat(),
                .y = verticesField[i+1].GetFloat(),
                .z = verticesField[i+2].GetFloat(),
            });
        }

        auto& triangleIndiciesField = requireArray(object, "triangles");
        vector<int> triangleIndicies;
        for (auto& triangle : triangleIndiciesField.GetArray()) {
            triangleIndicies.push_back(triangle.GetInt());
        }

        objects.push_back({
            .vertices = vertices,
            .triangleIndicies = triangleIndicies,
        });
    }

    return objects;
}

Scene parseCRTScene(std::istream& input) {
    IStreamWrapper isw(input);

    Document doc;
    doc.ParseStream(isw);
    if (doc.HasParseError()) {
        throw runtime_error("invalid JSON");
    }
    
    return Scene{
        .settings = getSettings(doc),
        .camera = getCamera(doc),
        .objects = getObjects(doc),
    };
}