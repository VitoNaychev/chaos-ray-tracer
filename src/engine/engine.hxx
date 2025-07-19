
#ifndef ENGINE_HXX
#define ENGINE_HXX

#include <vector>
#include "types.hxx"
#include "interx.hxx"
#include "scene.hxx"
#include "raygen.hxx"
#include "pixeldrawer.hpp"

class Engine {
public:
    Engine(Scene scene, pixeldrawer::PixelDrawer& drawer);
    void render();
    
private:
    Scene scene;
    pixeldrawer::PixelDrawer& drawer;
    
    std::vector<Triangle> triangles;
    RayGenerator raygen;
    
    Color trace(const Ray& ray, int depth = 0);
    Color shade(const Intersection& intersection, const Material& material, const Light& light);
};

#endif // ENGINE_HXX