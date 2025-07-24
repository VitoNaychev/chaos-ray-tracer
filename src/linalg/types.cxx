#include <iostream>
#include <array>
#include <cmath>
#include "types.hxx"

bool operator==(const Color& c1, const Color& c2) {
    return c1.r == c2.r && c1.g == c2.g && c1.b == c2.b;
}

Color operator*(const Color& c, float scalar) {
    return Color{c.r * scalar, c.g * scalar, c.b * scalar};
}

Color operator*(float scalar, const Color& c) {
    return Color{c.r * scalar, c.g * scalar, c.b * scalar};
}

Color operator*(const Color& c1, const Color& c2) {
    return Color{c1.r * c2.r, c1.g * c2.g, c1.b * c2.b};
}

Color& operator+=(Color& c1, const Color& c2) {
    c1.r += c2.r;
    c1.g += c2.g;
    c1.b += c2.b;
    return c1;
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
    return Vector(
        y * rhs.z - z * rhs.y,
        z * rhs.x - x * rhs.z,
        x * rhs.y - y * rhs.x
    );
}

float Vector::dot(const Vector& rhs) const {
    return (x * rhs.x) + (y * rhs.y) + (z * rhs.z); 
}

Vector operator-(const Vector& v1, const Vector& v2) {
    float x = v1.x - v2.x;
    float y = v1.y - v2.y;
    float z = v1.z - v2.z;

    return Vector(x, y, z);
}

Vector operator+(const Vector& v1, const Vector& v2) {
    float x = v1.x + v2.x;
    float y = v1.y + v2.y;
    float z = v1.z + v2.z;

    return Vector(x, y, z);
}

Vector operator*(const Vector& v, float scalar) {
    return Vector(v.x * scalar, v.y * scalar, v.z * scalar);
}

Vector operator*(float scalar, const Vector& v) {
    return Vector(v.x * scalar, v.y * scalar, v.z * scalar);
}

Vector& operator+=(Vector& v1, const Vector& v2) {
    v1.x += v2.x;
    v1.y += v2.y;
    v1.z += v2.z;
    return v1;
}

std::ostream& operator<<(std::ostream& os, const Vector& v) {
    return os << "Vector{x=" << v.x << ", y=" << v.y << ", z=" << v.z << "}";
}

Triangle::Triangle(std::initializer_list<Vector*> v) {
    std::copy(v.begin(), v.end(), vertices);
    Vector v0v1 = *vertices[1] - *vertices[0];
    Vector v0v2 = *vertices[2] - *vertices[0];

    normal = v0v1.cross(v0v2);
    normal.normalize();
}


const Vector& Triangle::getNormal() const {
    return normal;
}

const Vector& Triangle::operator[](size_t index) const {
    if (index > 3) throw std::out_of_range("index out of range");
    return  *vertices[index];
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
    if (idx > 3) throw std::out_of_range("index out of range");
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
    return Vector(
        lhs.x * rhs.m[0][0] + lhs.y * rhs.m[1][0] + lhs.z * rhs.m[2][0],
        lhs.x * rhs.m[0][1] + lhs.y * rhs.m[1][1] + lhs.z * rhs.m[2][1],
        lhs.x * rhs.m[0][2] + lhs.y * rhs.m[1][2] + lhs.z * rhs.m[2][2]
    );
}

bool operator==(const Matrix& lhs, const Matrix& rhs) {
    return lhs.m == rhs.m;
}