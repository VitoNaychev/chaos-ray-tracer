#ifndef TRACER_HXX
#define TRACER_HXX

#include <vector>

#include "aabb.hxx"
#include "shader.hxx"
#include "scene.hxx"
#include "acctree.hxx"

using std::vector;

namespace tracer {

class TriangleWrapper : public Triangle {
public:
    TriangleWrapper(const Triangle& triangle, const Mesh* mesh);
    const Mesh* getContainingMesh() const;
private:
    const Mesh* containingMesh;
};

class Tracer : public shader::Tracer{
public:
    Tracer(const vector<Mesh>& objects, const vector<Material>& materials);
    shader::Intersection trace(const Ray& ray);
    shader::Intersection traceShadow(const Ray& ray, float maxT);
private:    
    AccTree accTree;
    const vector<Mesh>& objects;
    const vector<Material>& materials;
};

Tracer* factory(const Scene& scene);

}

#endif