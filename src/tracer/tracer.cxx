#include <iostream>
#include "tracer.hxx"
#include "interx.hxx"

namespace tracer {

Tracer::Tracer(const vector<Mesh>& objects, const vector<Material>& materials) : objects(objects), materials(materials) {
}

shader::Intersection Tracer::trace(const Ray& ray) {
    Intersection closestIntersection {
        .distance = INFINITY,
    };
    const Mesh* closesObject = nullptr;

    for (auto& object : objects) {
        const Material& material = materials[object.getMaterialIndex()];

        for (auto& tri : object.getTriangles()) {
            Intersection i = intersect(ray, tri, material.smooth);
            if (i.distance < closestIntersection.distance) {
                closestIntersection = i;
                closesObject = &object;
            }
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