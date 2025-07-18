
#ifndef SHADE_HXX
#define SHADE_HXX

#include <vector>
#include "interx.hxx"
#include "types.hxx"
#include "scene.hxx"

Color shade(const Intersection& intersection, const Material& material, const Light& light, const std::vector<Mesh>& objects);

#endif // SHADE_HXX