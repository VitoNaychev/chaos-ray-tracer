#include <rapidjson/document.h>
#include <rapidjson/istreamwrapper.h>
#include <stdexcept>
#include <format>

using namespace std;
using namespace rapidjson;

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

float requireFloat(Value& doc, const char* name) {
    if (!doc.HasMember(name)) {
        throw runtime_error(format("missing {} key", name));
    }
    if (!doc[name].IsNumber()) {
        throw runtime_error(format("{} is not a float", name));
    }

    return doc[name].GetFloat();
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