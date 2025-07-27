#include <vector>
#include <array>

#include "aabb.hxx"
#include "types.hxx"

using namespace std;

AABB::AABB() {
    max = Vector{-INFINITY, -INFINITY,-INFINITY};
    min = Vector{INFINITY, INFINITY, INFINITY};
}

AABB::AABB(Vector min, Vector max) : min {min}, max {max} {}

AABB::AABB(const AABB& other) : min {other.min}, max {other.max} {}

AABB AABB::construct(Triangle& tri) {
    AABB aabb{};

    for (int i = 0; i < 3; ++ i) {
        aabb.min[AxisEnum::X] = std::min(aabb.min[AxisEnum::X], tri[i][AxisEnum::X]);
        aabb.min[AxisEnum::Y] = std::min(aabb.min[AxisEnum::Y], tri[i][AxisEnum::Y]);
        aabb.min[AxisEnum::Z] = std::min(aabb.min[AxisEnum::Z], tri[i][AxisEnum::Z]);

        aabb.max[AxisEnum::X] = std::max(aabb.max[AxisEnum::X], tri[i][AxisEnum::X]);
        aabb.max[AxisEnum::Y] = std::max(aabb.max[AxisEnum::Y], tri[i][AxisEnum::Y]);
        aabb.max[AxisEnum::Z] = std::max(aabb.max[AxisEnum::Z], tri[i][AxisEnum::Z]);
    }
    return aabb;
}

AABB AABB::construct(vector<Triangle*> triangles) {
    AABB aabb{};
    for (auto tri : triangles) {
        for (int i = 0; i < 3; ++ i) {
            aabb.min[AxisEnum::X] = std::min(aabb.min[AxisEnum::X], (*tri)[i][AxisEnum::X]);
            aabb.min[AxisEnum::Y] = std::min(aabb.min[AxisEnum::Y], (*tri)[i][AxisEnum::Y]);
            aabb.min[AxisEnum::Z] = std::min(aabb.min[AxisEnum::Z], (*tri)[i][AxisEnum::Z]);

            aabb.max[AxisEnum::X] = std::max(aabb.max[AxisEnum::X], (*tri)[i][AxisEnum::X]);
            aabb.max[AxisEnum::Y] = std::max(aabb.max[AxisEnum::Y], (*tri)[i][AxisEnum::Y]);
            aabb.max[AxisEnum::Z] = std::max(aabb.max[AxisEnum::Z], (*tri)[i][AxisEnum::Z]);
        }
    }
    return aabb;
}

 array<AABB, 2> AABB::split(AxisEnum axis) {
    AABB first {*this};
    AABB second {*this};

    float middle;
    switch (axis) {
        case AxisEnum::X:
            middle = (min[AxisEnum::X] + max[AxisEnum::X]) / 2;
            first.max[AxisEnum::X] = middle;
            second.min[AxisEnum::X] = middle;
            break;
        case AxisEnum::Y:
            middle = (min[AxisEnum::Y] + max[AxisEnum::Y]) / 2;
            first.max[AxisEnum::Y] = middle;
            second.min[AxisEnum::Y] = middle;
            break;
        case AxisEnum::Z:
            middle = (min[AxisEnum::Z] + max[AxisEnum::Z]) / 2;
            first.max[AxisEnum::Z] = middle;
            second.min[AxisEnum::Z] = middle;
            break;
        default:
            throw runtime_error("cannot split over undefined axis");
    }
    return {first, second};
 }



bool laysOnSide(const Ray& ray, const AABB& aabb, float side, float originSideAxis, float directionSideAxis) {
    float t = (side - originSideAxis) / directionSideAxis;
    if (t < 0) {
        return false;
    }

    Vector p = ray.origin + t * ray.direction;
    return p >= aabb.min && p <= aabb.max;
}

bool doesIntersect(const Ray& ray, const AABB& aabb) {
    return laysOnSide(ray, aabb, aabb.min[AxisEnum::X], ray.origin[AxisEnum::X], ray.direction[AxisEnum::X]) ||
        laysOnSide(ray, aabb, aabb.min[AxisEnum::Y], ray.origin[AxisEnum::Y], ray.direction[AxisEnum::Y]) ||
        laysOnSide(ray, aabb, aabb.min[AxisEnum::Z], ray.origin[AxisEnum::Z], ray.direction[AxisEnum::Z]) ||
        laysOnSide(ray, aabb, aabb.max[AxisEnum::X], ray.origin[AxisEnum::X], ray.direction[AxisEnum::X]) ||
        laysOnSide(ray, aabb, aabb.max[AxisEnum::Y], ray.origin[AxisEnum::Y], ray.direction[AxisEnum::Y]) ||
        laysOnSide(ray, aabb, aabb.max[AxisEnum::Z], ray.origin[AxisEnum::Z], ray.direction[AxisEnum::Z]);
}

std::ostream& operator<<(std::ostream& os, const AABB& aabb) {
    os << "AABB{min: " << aabb.min << ", max: " << aabb.max << "}";
    return os;
}