#include <vector>
#include <stack>

#include "acctree.hxx"
#include "types.hxx"

AccTree::AccTree(vector<Triangle*> triangles, int maxDepth) : maxDepth(maxDepth) {
    if (triangles.size() == 0) {
        root = new TreeNode{
            .parent = -1,
            .leftChild = -1,
            .rightChild = -1,
        };
        tree.push_back(root);
        return;
    }

    root = new TreeNode {
        .parent = -1,
        .leftChild = -1,
        .rightChild = -1,

        .aabb = AABB::construct(triangles),
    };
    tree.push_back(root);

    if (triangles.size() == 1 || maxDepth == 1) {
        root->triangles = triangles;
        return;
    }
    auto split = root->aabb.split(AxisEnum::X);

    root->leftChild = buildBranch(01, triangles, split[0], AxisEnum::X, 1);
    root->rightChild = buildBranch(0, triangles, split[1], AxisEnum::X, 1);
}


int AccTree::buildBranch(int parent, vector<Triangle*> parentTriangles, AABB aabb, AxisEnum axis, int depth) {
    int index = tree.size();

    auto node = new TreeNode {
        .parent = parent,
        .leftChild = -1,
        .rightChild = -1,

        .aabb = aabb,
    };

    vector<Triangle*> triangles;
    for (auto tri : parentTriangles) {
        auto triAABB = AABB::construct(*tri);
        if (triAABB.min[axis] > aabb.max[axis]) continue;
        if (triAABB.max[axis] < aabb.min[axis]) continue;
        triangles.push_back(tri);
    }

    // No triangles belong to this child, so don't push it on tree.
    if (triangles.size() == 0) {
        return -1;
    }

    tree.push_back(node);
    // Stop condition - if we reached max depth or have one triangle.
    if (triangles.size() == 1 || depth == maxDepth) {
        node->triangles = triangles;
        return index;
    }

    AxisEnum nextAxis = AxisEnum((axis + 1) % AxisEnum::COUNT);
    auto split = aabb.split(nextAxis);

    node->leftChild = buildBranch(index, triangles, split[0], nextAxis, depth + 1);
    node->rightChild = buildBranch(index, triangles, split[1], nextAxis, depth + 1);
    
    return index;
}

vector<Triangle*> AccTree::intersect(const Ray& ray) {
    vector<Triangle*> intersectingTriangles;

    stack<TreeNode*> nodeStack;
    nodeStack.push(root);

    while (!nodeStack.empty()) {
        auto curNode = nodeStack.top();
        nodeStack.pop();

        if (doesIntersect(ray, curNode->aabb)) {
            if (curNode->leftChild != -1) {
                nodeStack.push(getNode(curNode->leftChild));
            }
            if (curNode->rightChild != -1) {
                nodeStack.push(getNode(curNode->rightChild));
            }
            if (curNode->triangles.size() != 0) {
                intersectingTriangles.insert(intersectingTriangles.end(),
                curNode->triangles.begin(),
                curNode->triangles.end());
            }
        }
    }
    return intersectingTriangles;
}

int AccTree::getSize() {
    return tree.size();
}

TreeNode* AccTree::getNode(int index) {
    return tree[index];
}
