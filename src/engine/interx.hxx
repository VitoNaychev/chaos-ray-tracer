#ifndef INTERX_HXX
#define INTERX_HXX

#include "types.hxx"
#include <vector>

struct Intersection {
    Vector point;
    float distance;

    Triangle const* triangle;
    Color albedo;
};

/**
 * Helper function to determine if a point is on the left side of an edge.
 * 
 * @param p The point to test
 * @param v0 First vertex of the edge
 * @param v1 Second vertex of the edge
 * @param normal The normal vector of the triangle
 * @return True if the point is on the left side of the edge
 */
bool leftOfEdge(Vector p, Vector v0, Vector v1, Vector normal);

/**
 * Calculate the intersection between a ray and a triangle.
 * 
 * @param ray The ray to test for intersection
 * @param tri The triangle to test for intersection
 * @return Intersection struct with point, distance, and normal, or distance=INFINITY if no intersection
 */
Intersection intersect(const Ray& ray, const Triangle& tri);

/**
 * Check if a ray intersects with a triangle.
 * 
 * @param ray The ray to test for intersection
 * @param tri The triangle to test for intersection
 * @return True if the ray intersects the triangle, false otherwise
 */
bool intersectExists(const Ray& ray, const Triangle& tri);


#endif // INTERX_HXX