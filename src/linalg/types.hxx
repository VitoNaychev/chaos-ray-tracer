#ifndef TYPES_HPP
#define TYPES_HPP
 
#include <ostream>
#include <vector>
#include <array>

struct Color {
    float r;
    float g;
    float b;
};

bool operator==(const Color& c1, const Color& c2);

Color operator*(const Color& c, float scalar);
Color operator*(float scalar, const Color& c);
Color& operator+=(Color& c1, const Color& c2);

std::ostream& operator<<(std::ostream& os, const Color& c);

struct Pixel {
    float x;
    float y;
};

bool operator==(const Pixel& p1, const Pixel& p2);

std::ostream& operator<<(std::ostream& os, const Pixel& p);

struct Vector {
    float length();
    void normalize();

    Vector cross(const Vector& rhs);
    float dot(const Vector& rhs);

    float x, y, z;
};

bool operator==(const Vector& v1, const Vector& v2);

Vector operator-(const Vector& v1, const Vector& v2);
Vector operator+(const Vector& v1, const Vector& v2);
Vector operator*(const Vector& v, float scalar);
Vector operator*(float scalar, const Vector& v);
Vector& operator+=(Vector& v1, const Vector& v2);

std::ostream& operator<<(std::ostream& os, const Vector& v);

struct Ray {
    Vector origin, direction;
};

class Triangle {
public: 
    Triangle();
    Triangle(const Vector& v0, const Vector& v1, const Vector& v2);

    bool intersects(Ray ray);
    float intersectionDistance(Ray ray);
    Vector getNormal() const;
    Vector getV0() const;
    Vector getV1() const;
    Vector getV2() const;
private:
   const Vector& v0, v1, v2;
   Vector normal;
};

class Matrix {
public:
    Matrix() = default;
    Matrix(std::array<std::array<float, 3>, 3> m);

    static Matrix identity();

    std::array<float, 3> operator[](int idx);

    friend Matrix operator*(const Matrix& lhs, const Matrix& rhs);
    friend Vector operator*(const Vector& lhs, const Matrix& rhs);
    
    friend bool operator==(const Matrix& lhs, const Matrix& rhs);
private:
    std::array<std::array<float, 3>, 3> m  = {};;
};

#endif