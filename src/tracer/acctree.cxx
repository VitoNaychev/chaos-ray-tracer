#include <vector>
#include <stack>
#include <iostream>

#include "acctree.hxx"
#include "types.hxx"

vector<Triangle*> getIntersectingTriangles(const AABB& aabb, const vector<Triangle*>& triangles, AxisEnum axis) {
    vector<Triangle*> intersectingTriangles;
    for (auto tri : triangles) {
        auto triAABB = AABB::construct(*tri);
        if (triAABB.min[axis] > aabb.max[axis]) continue;
        if (triAABB.max[axis] < aabb.min[axis]) continue;
        intersectingTriangles.push_back(tri);
    }
    return intersectingTriangles;
}

AccTree::AccTree(vector<Triangle*> triangles, int maxDepth) : maxDepth(maxDepth) {
    TreeNode root {
        .parent = -1,
        .leftChild = -1,
        .rightChild = -1,

        .aabb = AABB::construct(triangles),
    };
    tree.push_back(root);

    if (triangles.size() == 1 || maxDepth == 1) {
        tree[0].triangles = triangles;
        return;
    }
    auto split = root.aabb.split(AxisEnum::X);

    tree[0].leftChild = buildBranch(0, triangles, split[0], AxisEnum::X, 1);
    tree[0].rightChild = buildBranch(0, triangles, split[1], AxisEnum::X, 1);
}


int AccTree::buildBranch(int parent, vector<Triangle*> parentTriangles, AABB aabb, AxisEnum axis, int depth) {
    auto triangles = getIntersectingTriangles(aabb, parentTriangles, axis);
    if (triangles.size() == 0) {
        return -1;
    }

    int index = tree.size();
    TreeNode node {
        .parent = parent,
        .leftChild = -1,
        .rightChild = -1,

        .aabb = aabb,
    };
    tree.push_back(node);

    if (triangles.size() == 1 || depth == maxDepth) {
        tree[index].triangles = triangles;
        return index;
    }

    AxisEnum nextAxis = AxisEnum((axis + 1) % AxisEnum::COUNT);
    auto split = aabb.split(nextAxis);

    tree[index].leftChild = buildBranch(index, triangles, split[0], nextAxis, depth + 1);
    tree[index].rightChild = buildBranch(index, triangles, split[1], nextAxis, depth + 1);
    
    return index;
}

vector<Triangle*> AccTree::intersect(const Ray& ray) {
    vector<Triangle*> intersectingTriangles;

    stack<int> nodeStack;
    nodeStack.push(0);

    while (!nodeStack.empty()) {
        int index = nodeStack.top();
        nodeStack.pop();

        auto& curNode = tree[index];
        if (doesIntersect(ray, curNode.aabb)) {
            if (curNode.leftChild != -1) {
                nodeStack.push(curNode.leftChild);
            }
            if (curNode.rightChild != -1) {
                nodeStack.push(curNode.rightChild);
            }
            if (curNode.triangles.size() != 0) {
                intersectingTriangles.insert(intersectingTriangles.end(),
                curNode.triangles.begin(),
                curNode.triangles.end());
            }
        }
    }
    return intersectingTriangles;
}

int AccTree::getSize() {
    return tree.size();
}

TreeNode AccTree::getNode(int index) {
    return tree[index];
}
