#include <cmath>
#include <numbers>
#include <algorithm>
#include <iostream>

#include "interx.hxx"
#include "scene.hxx"

bool isApproxZero(float value, float epsilon = 1e-5f) {
    return std::fabs(value) < epsilon;
}

Vector calculateSmoothHitNormal(const Vector& point, const Triangle& tri) {
    Vector v0p = point - tri[0];
    Vector v0v1 = tri[1]- tri[0];
    Vector v0v2 = tri[2] - tri[0];

    float u = v0v1.cross(v0p).length() / v0v1.cross(v0v2).length();
    float v = v0v2.cross(v0p).length() / v0v1.cross(v0v2).length();

    const Vertex& v0 = dynamic_cast<const Vertex&>(tri[0]);
    const Vertex& v1 = dynamic_cast<const Vertex&>(tri[1]);
    const Vertex& v2 = dynamic_cast<const Vertex&>(tri[2]);

    Vector averagedNormal =  v * v1.normal + u * v2.normal + (1 - u - v) * v0.normal;
    averagedNormal.normalize();

    return averagedNormal;
}

bool leftOfEdge(Vector p, Vector v0, Vector v1, Vector normal) {
    Vector e = v0 - v1;
    Vector vp = v0 - p;

    return normal.dot(e.cross(vp)) > 0;
}

Intersection intersect(const Ray& ray, const Triangle& tri, bool isSmooth) {
    Vector triNormal = tri.getNormal();

    float rayProj = triNormal.dot(ray.direction);
    if (isApproxZero(abs(rayProj))) {
        return Intersection{
            .distance = INFINITY,
        };
    }

    float rayDist = (tri[0] - ray.origin).dot(triNormal);

    float t = rayDist/rayProj;
    if (t <= 0) {
        return Intersection{
            .distance = INFINITY,
        };
    }
    
    Vector p = ray.origin + t * ray.direction;

    bool leftOfE0 = leftOfEdge(p, tri[0], tri[1], triNormal);
    bool leftOfE1 = leftOfEdge(p, tri[1], tri[2], triNormal);
    bool leftOfE2 = leftOfEdge(p, tri[2], tri[0], triNormal);

    if(leftOfE0 && leftOfE1 && leftOfE2) {
        return Intersection{
            .point = p,
            .distance = t,
            .normal = isSmooth ? calculateSmoothHitNormal(p, tri) : triNormal,
        };
    } else {
        return Intersection{
            .distance = INFINITY,
        };
    }
}

bool intersectExists(const Ray& ray, const Triangle& tri) {
    Intersection i = intersect(ray, tri, false);
    return i.distance != INFINITY;
}
