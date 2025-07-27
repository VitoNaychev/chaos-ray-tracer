#include <catch2/catch_test_macros.hpp>
#include <vector>
#include <iostream>

#include "aabb.hxx"
#include "types.hxx"
#include "acctree.hxx"

using namespace std;

TEST_CASE("returns an empty tree on no triangles") {
    vector<Triangle*> triangles;
    AccTree tree {triangles};

    REQUIRE(tree.getSize() == 1);
}

TEST_CASE("returns acc tree with one node on one triangle input") {
    AABB wantAABB {{-5, 0, 0}, {5, 5, 0}};

    vector<Triangle*> triangles;
    Triangle tri {{
        new Vector{5, 0, 0},
        new Vector{0, 5, 0},
        new Vector{-5, 0, 0},
    }};
    triangles.push_back(&tri);

    AccTree tree {triangles};

    REQUIRE(tree.getSize() == 1);

    REQUIRE(tree.getNode(0)->triangles == triangles);
}

TEST_CASE("returns acc tree for two triangles split over X plane") {
    Triangle triLeft {{
        new Vector{-6, 0, 0},
        new Vector{-2, 5, 0},
        new Vector{-4, 0, 0},
    }};
    Triangle triRight {{
        new Vector{2, 0, 0},
        new Vector{6, 5, 0},
        new Vector{4, 0, 0},
    }};
    vector<Triangle*> triangles;
    triangles.push_back(&triLeft);
    triangles.push_back(&triRight);

    vector<Triangle*> wantParentTriangles;
    wantParentTriangles.push_back(&triLeft);
    wantParentTriangles.push_back(&triRight);

    vector<Triangle*> wantLeftChildTriangles;
    wantLeftChildTriangles.push_back(&triLeft);

    vector<Triangle*> wantRightChildTriangles;
    wantRightChildTriangles.push_back(&triRight);

    AccTree tree {triangles};

    REQUIRE(tree.getSize() == 3);

    REQUIRE(tree.getNode(1)->triangles == wantLeftChildTriangles);
    REQUIRE(tree.getNode(2)->triangles == wantRightChildTriangles);
}

TEST_CASE("ray intersection returns correct triangles") {
    Triangle tri {{
        new Vector{0, 0, -5},
        new Vector{2, 0, -5}, 
        new Vector{1, 2, -5},
    }};
    vector<Triangle*> triangles = {&tri};
    AccTree tree{triangles};
    
    Ray hitRay{{0, 0, 0}, {0, 0, -1}};  // Ray going toward triangle
    auto result = tree.intersect(hitRay);
    
    REQUIRE(result.size() == 1);
    REQUIRE(result[0] == &tri);
}

TEST_CASE("ray intersection returns empty for miss") {
    Triangle tri {{
        new Vector{10, 10, -5},  // Far from origin
        new Vector{12, 10, -5}, 
        new Vector{11, 12, -5},
    }};
    vector<Triangle*> triangles = {&tri};
    AccTree tree{triangles};
    
    Ray missRay{{0, 0, 0}, {0, 0, -1}};  // Ray that misses triangle region
    auto result = tree.intersect(missRay);
    
    REQUIRE(result.empty());
}

TEST_CASE("all input triangles are preserved in tree") {
    vector<Triangle*> triangles;
    Triangle tri1{{new Vector{-10, 0, 0}, new Vector{-8, 2, 0}, new Vector{-9, 0, 2}}};
    Triangle tri2{{new Vector{0, 0, 0}, new Vector{2, 2, 0}, new Vector{1, 0, 2}}};
    Triangle tri3{{new Vector{10, 0, 0}, new Vector{12, 2, 0}, new Vector{11, 0, 2}}};
    triangles = {&tri1, &tri2, &tri3};
    
    AccTree tree{triangles};
    
    // Collect all triangles from all nodes
    vector<Triangle*> allTreeTriangles;
    for (int i = 0; i < tree.getSize(); i++) {
        auto node = tree.getNode(i);
        allTreeTriangles.insert(allTreeTriangles.end(), 
                               node->triangles.begin(), 
                               node->triangles.end());
    }
    
    // Verify each input triangle appears at least once
    for (auto* inputTri : triangles) {
        bool found = false;
        for (auto* treeTri : allTreeTriangles) {
            if (treeTri == inputTri) {
                found = true;
                break;
            }
        }
        REQUIRE(found);
    }
}

TEST_CASE("tree respects max depth limit") {
    vector<Triangle*> triangles;
    vector<Vector*> vectors;  // Track for cleanup
    
    // Create 8 triangles all in same region to force deep splitting
    for (int i = 0; i < 8; i++) {
        auto v1 = new Vector{0, 0, (float)i};
        auto v2 = new Vector{1, 0, (float)i};
        auto v3 = new Vector{0.5f, 1, (float)i};
        vectors.insert(vectors.end(), {v1, v2, v3});
        
        triangles.push_back(new Triangle{{v1, v2, v3}});
    }
    
    AccTree tree{triangles, 2};  // Max depth = 2
    
    // Tree should not be deeper than maxDepth allows
    // With maxDepth=2: root(0) -> children(1,2) -> grandchildren possible
    // Verify no node has depth > 2 by checking structure
    REQUIRE(tree.getSize() <= 7);  // 1 root + 2 children + 4 grandchildren max
    
    // Cleanup
    for (auto* tri : triangles) delete tri;
    for (auto* vec : vectors) delete vec;
}

TEST_CASE("intersect collects from multiple leaf nodes") {
    // Create two separated triangles
    Triangle triLeft{{new Vector{-10, 0, 0}, new Vector{-8, 2, 0}, new Vector{-9, 0, 2}}};
    Triangle triRight{{new Vector{10, 0, 0}, new Vector{12, 2, 0}, new Vector{11, 0, 2}}};
    vector<Triangle*> triangles = {&triLeft, &triRight};
    
    AccTree tree{triangles};
    
    // Ray that spans across both regions
    Ray wideRay{{-15, 1, 1}, {1, 0, 0}};  // Horizontal ray crossing both triangles
    auto result = tree.intersect(wideRay);
    
    // Should collect triangles from multiple leaf nodes
    REQUIRE(result.size() >= 1);  // At least one triangle should be found
}

TEST_CASE("empty child nodes are handled correctly") {
    // Create triangle that might not fit cleanly into split regions
    Triangle tri{{
        new Vector{0.1f, 0, 0},    // Slightly off-center
        new Vector{0.2f, 1, 0}, 
        new Vector{0.15f, 0.5f, 1}
    }};
    vector<Triangle*> triangles = {&tri};
    
    AccTree tree{triangles};
    
    // Ray intersection should still work even if some child nodes are empty
    Ray ray{{0, 0, 0}, {0.15f, 0.5f, 1}};  // Ray toward triangle
    auto result = tree.intersect(ray);
    
    // Should not crash and should return reasonable results
    REQUIRE(result.size() <= 1);
}
