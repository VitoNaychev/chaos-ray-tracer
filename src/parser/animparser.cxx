#include "animparser.hxx"

#include <iostream>
#include <stdexcept>
#include <vector>

#include <rapidjson/document.h>
#include <rapidjson/istreamwrapper.h>

#include "require.hxx"
#include "animation.hxx"

using namespace std;
using namespace rapidjson;

MovementType getMovement(string str) {
    if (str == "pan") {
        return MovementType::Pan;
    } else if (str == "dolly") {
        return MovementType::Dolly;
    } else if (str == "jib") {
        return MovementType::Jib;
    } else if (str == "truck") {
        return MovementType::Truck;
    } else if (str == "tilt") {
        return MovementType::Tilt;
    } else if (str == "roll") {
        return MovementType::Roll;
    } else if (str == "arc") {
        return MovementType::Arc;
    } else if (str == "zoom") {
        return MovementType::Zoom;
    }
    throw runtime_error("unknown movement type: " + str);
}

vector<Clip> parseAnimation(istream& input) {
    vector<Clip> movements;
    IStreamWrapper isw(input);

    Document doc;
    doc.ParseStream(isw);
    if (doc.HasParseError()) {
        throw runtime_error("invalid JSON");
    }

    auto& clipArray = requireArray(doc, "clip");
    for (auto& clip : clipArray.GetArray()) {
        movements.push_back(Clip{
            .movement = getMovement(requireString(clip, "type")),
            .units = requireFloat(clip, "units"),

            .framesCount = requireInt(clip, "frames"),
        });
    }

    return movements;
}