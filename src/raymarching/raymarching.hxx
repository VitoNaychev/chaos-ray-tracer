#ifndef RAYMARCHING_HXX
#define RAYMARCHING_HXX

#include <vector>
#include "shader.hxx"
#include "scene.hxx"

using std::vector;

namespace raymarching {

class RayMarchingTracer : public shader::Tracer {
public:
    RayMarchingTracer(const vector<Material>& materials, FractalType fractalType, Vector juliaC);
    shader::Intersection trace(const Ray& ray) override;

private:
    const vector<Material>& materials;
    FractalType fractalType;
    Vector juliaC;
    
    float evaluateSDF(const Vector& pos);
    float mandelbulbSDF(const Vector& pos);
    float juliaSetSDF(const Vector& pos);
    Vector calculateSDFNormal(const Vector& point);
};

RayMarchingTracer* factory(const Scene& scene);

}

#endif