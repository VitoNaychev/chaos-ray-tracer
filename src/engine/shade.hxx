
#ifndef SHADE_HXX
#define SHADE_HXX

#include <vector>
#include "interx.hxx"
#include "types.hxx"
#include "scene.hxx"

Color shade(Intersection intersection, Light light, std::vector<Mesh>& objects);

#endif // SHADE_HXX