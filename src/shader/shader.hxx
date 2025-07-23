#ifndef SHADER_HXX
#define SHADER_HXX

#include <vector>

#include "types.hxx"
#include "scene.hxx"
#include "engine.hxx"

using std::vector;

namespace shader {

struct Intersection {
    Vector point;
    float distance;
    Vector direction;

    const Triangle* triangle;
    const Mesh* object;
};

class Tracer {
public:
    virtual ~Tracer() = default;
    virtual Intersection trace(const Ray& ray) = 0;
};

typedef std::function<Tracer*(const Scene&)> TracerFactory;

class Shader : public engine::Shader{
public:
    Shader(Tracer* tracer, const vector<Mesh>& objects, const vector<Material>& materials, const vector<Light>& lights, const Color background);
    Color shade(const Ray& ray) override;
private:
    Tracer* tracer;
    const vector<Mesh>& objects;
    const vector<Material>& materials;
    const vector<Light>& lights;
    const Color background;
    
    Color shadeReflective(const Intersection& interx);
    Color shadeDiffuse(const Intersection& interx);
    Color shadeDiffuseSingleLight(const Intersection& interx, const Light& light);

    Ray generateReflectionRay(const Intersection& interx);
    Ray generateShadowRay(const Intersection& interx, const Vector& hitNormal, const Vector& lightDir);
};

class ShaderFactory {
public:
    ShaderFactory(TracerFactory tracerFactory);
    Shader* factory(const Scene& scene);
private:
    TracerFactory tracerFactory;
};

}

#endif