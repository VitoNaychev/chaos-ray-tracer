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

Vector::Vector(float x, float y, float z) {
    c[AxisEnum::X] = x;
    c[AxisEnum::Y] = y;
    c[AxisEnum::Z] = z;
}

Vector::Vector(std::initializer_list<float> coords) {
    std::copy(coords.begin(), coords.end(), this->c);
}

float Vector::length() {
    return sqrt(c[AxisEnum::X] * c[AxisEnum::X] + c[AxisEnum::Y] * c[AxisEnum::Y] + c[AxisEnum::Z] * c[AxisEnum::Z]);
}

void Vector::normalize() {
    float l = length();
    c[AxisEnum::X] /= l;
    c[AxisEnum::Y] /= l;
    c[AxisEnum::Z] /= l;
}

Vector Vector::cross(const Vector& rhs) {
    return Vector(
        c[AxisEnum::Y] * rhs.c[AxisEnum::Z] - c[AxisEnum::Z] * rhs.c[AxisEnum::Y],
        c[AxisEnum::Z] * rhs.c[AxisEnum::X] - c[AxisEnum::X] * rhs.c[AxisEnum::Z],
        c[AxisEnum::X] * rhs.c[AxisEnum::Y] - c[AxisEnum::Y] * rhs.c[AxisEnum::X]
    );
}

float Vector::dot(const Vector& rhs) const {
    return (c[AxisEnum::X] * rhs.c[AxisEnum::X]) + (c[AxisEnum::Y] * rhs.c[AxisEnum::Y]) + (c[AxisEnum::Z] * rhs.c[AxisEnum::Z]); 
}

Vector operator-(const Vector& v1, const Vector& v2) {
    return Vector(
        v1.c[AxisEnum::X] - v2.c[AxisEnum::X],
        v1.c[AxisEnum::Y] - v2.c[AxisEnum::Y],
        v1.c[AxisEnum::Z] - v2.c[AxisEnum::Z]
    );
}

Vector operator+(const Vector& v1, const Vector& v2) {
    return Vector(
        v1.c[AxisEnum::X] + v2.c[AxisEnum::X],
        v1.c[AxisEnum::Y] + v2.c[AxisEnum::Y],
        v1.c[AxisEnum::Z] + v2.c[AxisEnum::Z]
    );
}

Vector operator*(const Vector& v, float scalar) {
    return Vector(v.c[AxisEnum::X] * scalar, v.c[AxisEnum::Y] * scalar, v.c[AxisEnum::Z] * scalar);
}

Vector operator*(float scalar, const Vector& v) {
    return Vector(v.c[AxisEnum::X] * scalar, v.c[AxisEnum::Y] * scalar, v.c[AxisEnum::Z] * scalar);
}

Vector& operator+=(Vector& v1, const Vector& v2) {
    v1.c[AxisEnum::X] += v2.c[AxisEnum::X];
    v1.c[AxisEnum::Y] += v2.c[AxisEnum::Y];
    v1.c[AxisEnum::Z] += v2.c[AxisEnum::Z];
    return v1;
}

std::ostream& operator<<(std::ostream& os, const Vector& v) {
    return os << "Vector{x=" << v.c[AxisEnum::X] << ", y=" << v.c[AxisEnum::Y] << ", z=" << v.c[AxisEnum::Z] << "}";
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
        lhs.c[AxisEnum::X] * rhs.m[0][0] + lhs.c[AxisEnum::Y] * rhs.m[1][0] + lhs.c[AxisEnum::Z] * rhs.m[2][0],
        lhs.c[AxisEnum::X] * rhs.m[0][1] + lhs.c[AxisEnum::Y] * rhs.m[1][1] + lhs.c[AxisEnum::Z] * rhs.m[2][1],
        lhs.c[AxisEnum::X] * rhs.m[0][2] + lhs.c[AxisEnum::Y] * rhs.m[1][2] + lhs.c[AxisEnum::Z] * rhs.m[2][2]
    );
}

float& Vector::operator[](AxisEnum axis) {
    return c[axis];
}

const float& Vector::operator[](AxisEnum axis) const {
    return c[axis];
}

bool operator==(const Matrix& lhs, const Matrix& rhs) {
    return lhs.m == rhs.m;
}