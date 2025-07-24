#ifndef AABB_HXX
#define AABB_HXX

#include "types.hxx"

struct AABB {
    Vector max;
    Vector min;
};

bool doesIntersect(const Ray& ray, const AABB& aabb);

#endif // AABB_HXX