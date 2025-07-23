#include "tracer.hxx"
#include "interx.hxx"

namespace tracer {

Tracer::Tracer(const vector<Mesh>& objects) : objects(objects) {
}

shader::Intersection Tracer::trace(const Ray& ray) {
    ::Intersection closestIntersection {
        .distance = INFINITY,
    };
    const Mesh* closesObject = nullptr;

    for (auto& object : objects) {
        for (auto& tri : object.getTriangles()) {
            ::Intersection i = intersect(ray, tri);
            if (i.distance < closestIntersection.distance) {
                closestIntersection = i;
                closesObject = &object;
            }
        }
    }

    return shader::Intersection {
        .point = closestIntersection.point,
        .distance = closestIntersection.distance,
        .direction = ray.direction,

        .triangle = closestIntersection.triangle,
        .object = closesObject,
    };
}

Tracer* factory(const Scene& scene) {
    return new Tracer(scene.objects);
}

}