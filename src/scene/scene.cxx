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