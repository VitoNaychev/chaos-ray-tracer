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


Mesh::Mesh(vector<Vertex> vertices, vector<int> triangleIndicies, int materialIndex): 
    vertices {vertices}, triangleIndicies {triangleIndicies}, materialIndex {materialIndex} {
    for (size_t i = 0; i < triangleIndicies.size(); i += 3) {
        Triangle triangle({
            &(this->vertices[triangleIndicies[i]]),
            &(this->vertices[triangleIndicies[i+1]]),
            &(this->vertices[triangleIndicies[i+2]]),
        });
        triangles.push_back(triangle);
    }

    for (size_t i = 0; i < triangleIndicies.size(); i += 3) {
        Vector faceNormal = triangles[i/3].getNormal();
        this->vertices[triangleIndicies[i]].normal += faceNormal;
        this->vertices[triangleIndicies[i+1]].normal += faceNormal;
        this->vertices[triangleIndicies[i+2]].normal += faceNormal;
    }
    for (auto& vertex : this->vertices) {
        vertex.normal.normalize();
    }
}

const std::vector<Vertex>& Mesh::getVertices() {
    return vertices;
}

const std::vector<int>& Mesh::getTriangleIndicies() {
    return triangleIndicies;
}

const int Mesh::getMaterialIndex() {
    return materialIndex;
}

const std::vector<Triangle>& Mesh::getTriangles() const {
    return triangles;
}