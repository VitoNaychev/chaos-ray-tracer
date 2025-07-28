#include <vector>
#include <array>
#include <algorithm>
#include <cmath>

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



bool doesIntersect(const Ray& ray, const AABB& aabb) {
    float tmin = 0.0f;
    float tmax = INFINITY;
    
    for (int axis = 0; axis < 3; axis++) {
        float origin = ray.origin[static_cast<AxisEnum>(axis)];
        float direction = ray.direction[static_cast<AxisEnum>(axis)];
        float minPlane = aabb.min[static_cast<AxisEnum>(axis)];
        float maxPlane = aabb.max[static_cast<AxisEnum>(axis)];
        
        if (abs(direction) < 1e-9f) {
            // Ray is parallel to slab
            if (origin < minPlane || origin > maxPlane) {
                return false;
            }
        } else {
            float t1 = (minPlane - origin) / direction;
            float t2 = (maxPlane - origin) / direction;
            
            if (t1 > t2) {
                std::swap(t1, t2);
            }
            
            tmin = std::max(tmin, t1);
            tmax = std::min(tmax, t2);
            
            if (tmin > tmax) {
                return false;
            }
        }
    }
    
    return tmax >= 0;
}

std::ostream& operator<<(std::ostream& os, const AABB& aabb) {
    os << "AABB{min: " << aabb.min << ", max: " << aabb.max << "}";
    return os;
}