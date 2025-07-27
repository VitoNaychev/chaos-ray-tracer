#include <iostream>
#include <algorithm> 

#include "tracer.hxx"
#include "interx.hxx"
#include "aabb.hxx"
#include "acctree.hxx"

namespace tracer {

TriangleWrapper::TriangleWrapper(const Triangle& triangle, const Mesh* mesh) 
    : Triangle(triangle), containingMesh(mesh) {
}

const Mesh* TriangleWrapper::getContainingMesh() const {
    return containingMesh;
}

vector<Triangle*> createWrappedTriangles(const vector<Mesh>& objects) {
    vector<Triangle*> allTriangles;
    for (auto& object : objects) {
        for (auto& tri : object.getTriangles()) {
            allTriangles.push_back(new TriangleWrapper(tri, &object));
        }
    }
    return allTriangles;
}

Tracer::Tracer(const vector<Mesh>& objects, const vector<Material>& materials) 
    : objects(objects), materials(materials), accTree(createWrappedTriangles(objects), 1) {
}

shader::Intersection Tracer::trace(const Ray& ray) {
    Intersection closestIntersection {
        .distance = INFINITY,
    };

    const Mesh* closesObject = nullptr;

    vector<Triangle*> triangles = accTree.intersect(ray);

    for (auto tri : triangles) {
        auto wrappedTriangle = *(dynamic_cast<TriangleWrapper*>(tri)); 
        auto object = wrappedTriangle.getContainingMesh();
        auto material = materials[object->getMaterialIndex()];

        Intersection i = intersect(ray, *tri, material.smooth);
        if (i.distance < closestIntersection.distance) {
            closestIntersection = i;
            closesObject = object;
        }
    }
    
    if (closestIntersection.distance != INFINITY) {
        return shader::Intersection {
            .point = closestIntersection.point,
            .distance = closestIntersection.distance,
            .direction = ray.direction,
    
            .normal = closestIntersection.normal,
            .materialIndex = closesObject->getMaterialIndex(),
        };
    } else {
        return shader::Intersection {
            .distance = INFINITY,
        };
    }
}

Tracer* factory(const Scene& scene) {
    return new Tracer(scene.objects, scene.materials);
}

}