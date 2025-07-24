#include <rapidjson/document.h>
#include <rapidjson/istreamwrapper.h>
#include <iostream>
#include <format>

#include "scene.hxx"
#include "types.hxx"

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

bool requireBool(Value& doc, const char* name) {
    if (!doc.HasMember(name)) {
        throw runtime_error(format("missing {} key", name));
    }
    if (!doc[name].IsBool()) {
        throw runtime_error(format("{} is not a bool", name));
    }

    return doc[name].GetBool();
}

string requireString(Value& doc, const char* name) {
    if (!doc.HasMember(name)) {
        throw runtime_error(format("missing {} key", name));
    }
    if (!doc[name].IsString()) {
        throw runtime_error(format("{} is not a string", name));
    }

    return doc[name].GetString();
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
    Vector position(
        positionField[0].GetFloat(),
        positionField[1].GetFloat(),
        positionField[2].GetFloat()
    );
    
    return Camera(position, rotation);
}

vector<Light> getLights(rapidjson::Document& doc) {
    vector<Light> lights;

    auto& lightsField = requireArray(doc, "lights");
    for (auto& light : lightsField.GetArray()) {
        auto& positionField = requireArray(light, "position", 3);

        lights.push_back({
            .position = Vector(
                positionField[0].GetFloat(),
                positionField[1].GetFloat(),
                positionField[2].GetFloat()
            ),
            .intensity = requireInt(light, "intensity")
        });
    }

    return lights;
}

MaterialType getMaterialType(string materialString) {
    if (materialString == "diffuse") {
        return MaterialType::Diffuse;
    } else if (materialString == "reflective") {
        return MaterialType::Reflective;
    } else {
        throw runtime_error("unkown material type");
    }
}

Color getMaterialAlbedo(rapidjson::Value& albedoField) {
    return Color {
        .r = albedoField[0].GetFloat(),
        .g = albedoField[1].GetFloat(),
        .b = albedoField[2].GetFloat(),
    };
}

vector<Material> getMaterials(rapidjson::Document& doc) {
    vector<Material> materials;

    auto& materialsField = requireArray(doc, "materials");
    for (auto& materialField : materialsField.GetArray()) {
        materials.push_back({
            .type = getMaterialType(requireString(materialField, "type")),
            .albedo = getMaterialAlbedo(requireArray(materialField, "albedo", 3)),
            .smooth = requireBool(materialField, "smooth_shading"),
        });
    }

    return materials;
}

vector<Mesh> getObjects(rapidjson::Document& doc) {
    vector<Mesh> objects;

    auto& objectsField = requireArray(doc, "objects");
    for (auto& object : objectsField.GetArray()) {
        vector<Vertex> vertices;
        vector<int> triangleIndicies;

        auto& verticesField = requireArray(object, "vertices");
        for (size_t i = 0; i < verticesField.Capacity(); i+=3) {
            Vertex vertex {verticesField[i].GetFloat(), verticesField[i+1].GetFloat(), verticesField[i+2].GetFloat()};
            vertices.push_back(vertex);
        }

        auto& triangleIndiciesField = requireArray(object, "triangles");
        for (auto& triangle : triangleIndiciesField.GetArray()) {
            triangleIndicies.push_back(triangle.GetInt());
        }
        
        objects.push_back(Mesh{vertices, triangleIndicies, requireInt(object, "material_index")});
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

        .lights = getLights(doc),
        .materials = getMaterials(doc),
        .objects = getObjects(doc),
    };
}
