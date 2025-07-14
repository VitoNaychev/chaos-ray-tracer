#ifndef CRTSCENE_HPP
#define CRTSCENE_HPP

#include <iostream>
#include <rapidjson/document.h>

#include "types.hpp"

std::string getStringValue(std::istream&);
Scene parseCRTScene(std::istream& input);

Settings getSettings(rapidjson::Document& doc);
Camera getCamera(rapidjson::Document& doc);
std::vector<Mesh> getObjects(rapidjson::Document& doc);


#endif