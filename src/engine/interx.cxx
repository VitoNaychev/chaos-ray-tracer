#include <cmath>
#include <numbers>
#include <algorithm>
#include <iostream>

#include "interx.hxx"

bool isApproxZero(float value, float epsilon = 1e-5f) {
    return std::fabs(value) < epsilon;
}

bool leftOfEdge(Vector p, Vector v0, Vector v1, Vector normal) {
    Vector e = v0 - v1;
    Vector vp = v0 - p;

    return normal.dot(e.cross(vp)) > 0;
}

Intersection intersect(const Ray& ray, const Triangle& tri) {
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
            .triangle = &tri,
        };
    } else {
        return Intersection{
            .distance = INFINITY,
        };
    }
}

bool intersectExists(const Ray& ray, const Triangle& tri) {
    Intersection i = intersect(ray, tri);
    return i.distance != INFINITY;
}
