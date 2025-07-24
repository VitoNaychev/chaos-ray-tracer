#include <iostream>
#include <algorithm> 

#include "tracer.hxx"
#include "interx.hxx"
#include "aabb.hxx"

namespace tracer {

Tracer::Tracer(const vector<Mesh>& objects, const vector<Material>& materials) : objects(objects), materials(materials) {
    constructAABB();
}

void Tracer::constructAABB() {
    for (auto& object : objects) {
        for (auto& tri : object.getTriangles()) {
            for (int i = 0; i < 3; ++ i) {
                aabb.min.x = std::min(aabb.min.x, tri[i].x);
                aabb.min.y = std::min(aabb.min.y, tri[i].y);
                aabb.min.z = std::min(aabb.min.z, tri[i].z);

                aabb.max.x = std::max(aabb.max.x, tri[i].x);
                aabb.max.y = std::max(aabb.max.y, tri[i].y);
                aabb.max.z = std::max(aabb.max.z, tri[i].z);
            }
        }
    }
}

shader::Intersection Tracer::trace(const Ray& ray) {
    if (!doesIntersect(ray, aabb)) {
        return shader::Intersection {
            .distance = INFINITY,
        };
    }

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