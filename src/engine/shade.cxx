
#include "shade.hxx"
#include "interx.hxx"
#include <numbers>
#include <iostream>
#include <typeinfo>


Vector calculateSmoothHitNormal(const Intersection& intersection) {
    const Triangle& tri = *intersection.triangle;

    Vector v0p = intersection.point - tri[0];
    Vector v0v1 = tri[1]- tri[0];
    Vector v0v2 = tri[2] - tri[0];

    float u = v0v1.cross(v0p).length() / v0v1.cross(v0v2).length();
    float v = v0v2.cross(v0p).length() / v0v1.cross(v0v2).length();

    const Vertex& v0N = dynamic_cast<const Vertex&>(tri[0]);
    const Vertex& v1N = dynamic_cast<const Vertex&>(tri[1]);
    const Vertex& v2N = dynamic_cast<const Vertex&>(tri[2]);

    Vector averagedNormal =  v * v1N.normal + u * v2N.normal + (1 - u - v) * v0N.normal;
    averagedNormal.normalize();

    return averagedNormal;
}

Color shade(const Intersection& intersection, const Material& material, const Light& light, const std::vector<Mesh>& objects) {
    Vector lightDir = light.position - intersection.point;
    float sphereRadius = lightDir.length();

    Vector hitNormal = intersection.triangle->getNormal();
    if (material.smooth) {
        hitNormal = calculateSmoothHitNormal(intersection);
    }  

    lightDir.normalize();
    float cosLaw = lightDir.dot(hitNormal);
    if (cosLaw <= 0) {
        return Color{0, 0, 0};
    }

    for (auto& object : objects) {
        for (auto& tri : object.getTriangles()) {
            Ray shadowRay = {
                .origin = intersection.point + hitNormal * 1e-2,
                .direction = lightDir,
            };
            Intersection shadowIntersection = intersect(shadowRay, tri);
            if (shadowIntersection.distance < sphereRadius) {
                return Color{0, 0, 0};
            }
        }
    }

    float sphereArea = 4 * std::numbers::pi * sphereRadius * sphereRadius;
    return material.albedo * (light.intensity / sphereArea * cosLaw);
}