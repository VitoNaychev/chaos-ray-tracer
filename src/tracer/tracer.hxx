#ifndef TRACER_HXX
#define TRACER_HXX

#include <vector>

#include "shader.hxx"
#include "scene.hxx"

using std::vector;

namespace tracer {

class Tracer : public shader::Tracer{
public:
    Tracer(const vector<Mesh>& objects);
    shader::Intersection trace(const Ray& ray);
private:
    const vector<Mesh>& objects;
};

Tracer* factory(const Scene& scene);

}

#endif