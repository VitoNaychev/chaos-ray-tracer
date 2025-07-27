#ifndef AABB_HXX
#define AABB_HXX

#include "types.hxx"
#include <ostream>
#include <vector>

struct AABB {
    AABB();
    AABB(Vector min, Vector max);
    AABB(const AABB& other);

    static AABB  construct(Triangle& tri);
    static AABB construct(std::vector<Triangle*> triangles);

    friend auto operator<=>(const AABB& aabb1, const AABB& aabb2) = default;

    std::array<AABB, 2> split(AxisEnum axis);

    Vector max;
    Vector min;
};

std::ostream& operator<<(std::ostream& os, const AABB& aabb);

bool doesIntersect(const Ray& ray, const AABB& aabb);

#endif // AABB_HXX