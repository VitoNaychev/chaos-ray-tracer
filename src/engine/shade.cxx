
#include "shade.hxx"
#include "interx.hxx"
#include <numbers>

Color shade(Intersection intersection, Light light, std::vector<Triangle> triangles) {
    Vector lightDir = light.position - intersection.point;

    float sr = lightDir.length();

    lightDir.normalize();
    float cosLaw = lightDir.dot(intersection.normal);
    if (cosLaw <= 0) {
        return Color{0, 0, 0};
    }

    for (auto tri : triangles) {
        Ray shadowRay = {
            .origin = intersection.point + intersection.normal * 1e-4,
            .direction = lightDir,
        };
        Intersection shadowIntersection = intersect(shadowRay, tri);
        if (shadowIntersection.distance < sr) {
            return Color{0, 0, 0};
        }
    }

    float sphereArea = 4 * std::numbers::pi * sr * sr;
    return intersection.albedo * (light.intensity / sphereArea * cosLaw);
}