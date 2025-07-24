#ifndef CRTSCENE_HPP
#define CRTSCENE_HPP

#include <iostream>
#include <rapidjson/document.h>

#include "scene.hxx"
#include "types.hxx"

Scene parseCRTScene(std::istream& input);

std::string getStringValue(std::istream&);

Settings getSettings(rapidjson::Document& doc);
Camera getCamera(rapidjson::Document& doc);
std::vector<Mesh> getObjects(rapidjson::Document& doc);


#endif