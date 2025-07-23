
#ifndef ENGINE_HXX
#define ENGINE_HXX

#include <vector>
#include "types.hxx"
#include "interx.hxx"
#include "scene.hxx"
#include "raygen.hxx"

class Drawer {
public:
    virtual ~Drawer() = 0;
    virtual void draw(int x, int y, const Color& c) = 0;
};

class Engine {
public:
    Engine(Scene scene, Drawer& drawer);
    void render();
    
private:
    Scene scene;
    Drawer& drawer;
    
    std::vector<Triangle> triangles;
    raygen::RayGenerator raygen;
    
    Color trace(const Ray& ray, int depth = 0);
    Color shade(const Intersection& intersection, const Material& material, const Light& light);
};

#endif // ENGINE_HXX