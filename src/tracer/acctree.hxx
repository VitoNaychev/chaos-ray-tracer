#ifndef ACCTREE_HXX
#define ACCTREE_HXX

#include <vector>
#include "aabb.hxx"
#include "types.hxx"

using namespace std;

struct TreeNode {
    int parent;
    int leftChild, rightChild;

    AABB aabb;
    vector<Triangle*> triangles;
};

class AccTree {
public:
    AccTree(vector<Triangle*> triangles, int maxDepth = 10);

    vector<Triangle*> intersect(const Ray& ray);

    int getSize();
    TreeNode getNode(int index);
private:
    int buildBranch(int parent, vector<Triangle*> parentTriangles, AABB aabb, AxisEnum axis, int depth = 0);

    vector<TreeNode> tree;
    int maxDepth;
};

vector<TreeNode> buildAccTree(const vector<Triangle*>& triangles);

#endif // ACCTREE_HXX