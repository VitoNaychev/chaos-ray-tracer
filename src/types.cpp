#include <iostream>
#include <array>
#include <cmath>
#include "types.hpp"

bool operator==(const Color& c1, const Color& c2) {
    return c1.r == c2.r && c1.g == c2.g && c1.b == c2.b;
}

std::ostream& operator<<(std::ostream& os, const Color& c) {
    return os << "Color{r=" << c.r << ", g=" << c.g << ", b=" << c.b << "}";
}

bool operator==(const Pixel& p1, const Pixel& p2) {
    return p1.x == p2.x && p1.y == p2.y; 
}

std::ostream& operator<<(std::ostream& os, const Pixel& p) {
    return os << "Pixel{x=" << p.x << ", y=" << p.y << "}";
}

float Vector::length() {
    return sqrt(x * x + y * y + z * z);
}

void Vector::normalize() {
    float l = length();
    x /= l;
    y /= l;
    z /= l;
}

Vector Vector::cross(const Vector& rhs) {
    return Vector {
        y * rhs.z - z * rhs.y,
        z * rhs.x - x * rhs.z,
        x * rhs.y - y * rhs.x,
    };
}

float Vector::dot(const Vector& rhs) {
    return (x * rhs.x) + (y * rhs.y) + (z * rhs.z); 
}

bool operator==(const Vector& v1, const Vector& v2) {
    return v1.x == v2.x && v1.y == v2.y && v1.z == v2.z; 
}

Vector operator-(const Vector& v1, const Vector& v2) {
    float x = v1.x - v2.x;
    float y = v1.y - v2.y;
    float z = v1.z - v2.z;

    return Vector{x, y, z};
}


std::ostream& operator<<(std::ostream& os, const Vector& v) {
    return os << "Vector{x=" << v.x << ", y=" << v.y << ", z=" << v.z << "}";
}

Triangle::Triangle(Vector v0, Vector v1, Vector v2) : v0 {v0}, v1 {v1}, v2 {v2} {
    Vector v0v1 = v1 - v0;
    Vector v0v2 = v2 - v0;

    normal = v0v1.cross(v0v2);
    normal.normalize();
}



bool Triangle::intersects(Ray ray) {
    const float err = 1e-8;
    if (normal.dot(ray.direction) >= 0 - err) {
        return false;
    }

    float rayDist = abs((v0 - ray.origin).dot(normal));
    float rayProj = abs(ray.direction.dot(normal));

    float t = rayDist/rayProj;
    
    Vector p{ray.origin.x + t * ray.direction.x, ray.origin.y + t * ray.direction.y, ray.origin.z + t * ray.direction.z};

    bool leftOfE0 = leftOfEdge(p, v0, v1);
    bool leftOfE1 = leftOfEdge(p, v1, v2);
    bool leftOfE2 = leftOfEdge(p, v2, v0);

    return leftOfE0 && leftOfE1 && leftOfE2 ;
}

float Triangle::intersectionDistance(Ray ray) {
    const float err = 1e-8;
    if (normal.dot(ray.direction) >= 0 - err) {
        return -1;
    }

    float rayDist = abs((v0 - ray.origin).dot(normal));
    float rayProj = abs(ray.direction.dot(normal));

    float t = rayDist/rayProj;
    
    Vector p{ray.origin.x + t * ray.direction.x, ray.origin.y + t * ray.direction.y, ray.origin.z + t * ray.direction.z};

    bool leftOfE0 = leftOfEdge(p, v0, v1);
    bool leftOfE1 = leftOfEdge(p, v1, v2);
    bool leftOfE2 = leftOfEdge(p, v2, v0);

    if(leftOfE0 && leftOfE1 && leftOfE2) {
        return t;
    } else {
        return -1;
    }
}


bool Triangle::leftOfEdge(const Vector& p, const Vector& v0, const Vector& v1) {
    Vector e = v0 - v1;
    Vector vp = v0 - p;

    return normal.dot(e.cross(vp)) > 0;
}

Matrix::Matrix(std::array<std::array<float, 3>, 3> m) : m {m} {}

Matrix Matrix::identity() {
    std::array<std::array<float, 3>, 3> identityArr = {{
        {{1.0f, 0.0f, 0.0f}},
        {{0.0f, 1.0f, 0.0f}},
        {{0.0f, 0.0f, 1.0f}}
    }};
    return Matrix{identityArr};
}

std::array<float, 3> Matrix::operator[](int idx) {
    return m[idx];
}

Matrix operator*(const Matrix& lhs, const Matrix& rhs) {
    Matrix result;

    for (int i = 0; i < 3; ++i) {          
        for (int j = 0; j < 3; ++j) {
            result.m[i][j] = 0.0f;
            for (int k = 0; k < 3; ++k) {
                result.m[i][j] += lhs.m[i][k] * rhs.m[k][j];
            }
        }
    }

    return result;
}

Vector operator*(const Vector& lhs, const Matrix& rhs) {
    return Vector{
        .x = lhs.x * rhs.m[0][0] + lhs.y * rhs.m[1][0] + lhs.z * rhs.m[2][0],
        .y = lhs.x * rhs.m[0][1] + lhs.y * rhs.m[1][1] + lhs.z * rhs.m[2][1],
        .z = lhs.x * rhs.m[0][2] + lhs.y * rhs.m[1][2] + lhs.z * rhs.m[2][2],
    };
}

bool operator==(const Matrix& lhs, const Matrix& rhs) {
    return lhs.m == rhs.m;
}

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
    std::array<std::array<float, 3>, 3> panArr = {{
        {{std::cos(angle), 0.0f, -std::sin(angle)}},
        {{0.0f, 1.0f, 0.0f}},
        {{std::sin(angle), 0.0f, std::cos(angle)}}
    }};
    Matrix panMatrix {panArr};

    rotation = rotation * panMatrix;
}