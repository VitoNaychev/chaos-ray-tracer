#include "aabb.hxx"

bool laysOnSide(const Ray& ray, const AABB& aabb, float side, float originSideAxis, float directionSideAxis) {
    float t = (side - originSideAxis) / directionSideAxis;
    if (t < 0) {
        return false;
    }

    Vector p = ray.origin + t * ray.direction;
    return p >= aabb.min && p <= aabb.max;
}

bool doesIntersect(const Ray& ray, const AABB& aabb) {
    return laysOnSide(ray, aabb, aabb.min.x, ray.origin.x, ray.direction.x) ||
        laysOnSide(ray, aabb, aabb.min.y, ray.origin.y, ray.direction.y) ||
        laysOnSide(ray, aabb, aabb.min.z, ray.origin.z, ray.direction.z) ||
        laysOnSide(ray, aabb, aabb.max.x, ray.origin.x, ray.direction.x) ||
        laysOnSide(ray, aabb, aabb.max.y, ray.origin.y, ray.direction.y) ||
        laysOnSide(ray, aabb, aabb.max.z, ray.origin.z, ray.direction.z);
}