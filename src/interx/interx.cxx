#include <cmath>
#include <numbers>
#include <algorithm>

#include "interx.hxx"

bool isApproxZero(float value, float epsilon = 1e-5f) {
    return std::fabs(value) < epsilon;
}

bool leftOfEdge(Vector p, Vector v0, Vector v1, Vector normal) {
    Vector e = v0 - v1;
    Vector vp = v0 - p;

    return normal.dot(e.cross(vp)) > 0;
}

Intersection intersect(Ray ray, Triangle tri) {
    Vector triNormal = tri.getNormal();

    float rayProj = abs(triNormal.dot(ray.direction));
    if (isApproxZero(rayProj)) {
        return Intersection{
            .distance = INFINITY,
        };
    }
    
    float rayDist = abs((tri.getV0() - ray.origin).dot(triNormal));

    float t = rayDist/rayProj;
    Vector p = ray.origin + t * ray.direction;

    bool leftOfE0 = leftOfEdge(p, tri.getV0(), tri.getV1(), triNormal);
    bool leftOfE1 = leftOfEdge(p, tri.getV1(), tri.getV2(), triNormal);
    bool leftOfE2 = leftOfEdge(p, tri.getV2(), tri.getV0(), triNormal);

    if(leftOfE0 && leftOfE1 && leftOfE2) {
        return Intersection{
            .point = p,
            .distance = t,
            .normal = triNormal,
            .albedo = Color{1, 0, 0}
        };
    } else {
        return Intersection{
            .distance = INFINITY,
        };
    }
}

bool intersectExists(Ray ray, Triangle tri) {
    Intersection i = intersect(ray, tri);
    return i.distance != INFINITY;
}

Color shade(Intersection intersection, Light light, std::vector<Triangle> triangles) {
    Vector lightDir = light.position - intersection.point;

    float sr = lightDir.length();

    lightDir.normalize();
    float cosLaw = lightDir.dot(intersection.normal);
    if (cosLaw <= 0) {
        return Color{0, 0, 0};
    }

    // for (auto tri : triangles) {
    //     Ray shadowRay = {
    //         .origin = intersection.point + lightDir * 0.1,
    //         .direction = lightDir,
    //     };
    //     Intersection shadowIntersection = intersect(shadowRay, tri);
    //     if (shadowIntersection.distance < sr) {
    //         return Color{0, 0, 0};
    //     }
    // }

    float sphereArea = 4 * std::numbers::pi * sr * sr;
    return intersection.albedo * (light.intensity / sphereArea * cosLaw);
}