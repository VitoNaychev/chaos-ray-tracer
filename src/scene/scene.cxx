#include <cmath>

#include "scene.hxx"
#include "types.hxx"

using namespace std;

Vector Camera::getPosition() {
    return position;
}

Matrix Camera::getRotation() {
    return rotation;
}

Camera::Camera(Vector pos, Matrix rot) : position {pos}, rotation {rot} {}

void Camera::dolly(const float z) {
    position.z += z;
}

void Camera::jib(const float y) {
    position.y += y;
}

void Camera::truck(const float x) {
    position.x += x;
}

void Camera::pan(const float angle) {
    array<array<float, 3>, 3> panArr = {{
        {{cos(angle), 0.0f, -sin(angle)}},
        {{0.0f, 1.0f, 0.0f}},
        {{sin(angle), 0.0f, cos(angle)}}
    }};
    Matrix panMatrix {panArr};

    rotation = rotation * panMatrix;
}


Mesh::Mesh (vector<Vertex> vertices, vector<int> triangleIndicies): vertices {vertices}, triangleIndicies {triangleIndicies} {
    for (size_t i = 0; i < triangleIndicies.size(); i += 3) {
        const Vertex& v0 = this->vertices[triangleIndicies[i]];
        const Vertex& v1 = this->vertices[triangleIndicies[i+1]];
        const Vertex& v2 = this->vertices[triangleIndicies[i+2]];

        Triangle triangle(v0, v1, v2);
        triangles.push_back(triangle);
    }
    // Calculate vertex normals by averaging adjacent face normals
    for (size_t i = 0; i < triangleIndicies.size(); i += 3) {
        Vector faceNormal = triangles[i/3].getNormal();
        vertices[triangleIndicies[i]].normal += faceNormal;
        vertices[triangleIndicies[i+1]].normal += faceNormal;
        vertices[triangleIndicies[i+2]].normal += faceNormal;
    }
    for (auto& vertex : vertices) {
        vertex.normal.normalize();
    }
}

const std::vector<Vertex>& Mesh::getVertices() {
    return vertices;
}

const std::vector<int>& Mesh::getTriangleIndicies() {
    return triangleIndicies;
}

const std::vector<Triangle>& Mesh::getTriangles() {
    return triangles;
}