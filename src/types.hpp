#ifndef TYPES_HPP
#define TYPES_HPP
 
#include <ostream>
#include <array>

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

std::ostream& operator<<(std::ostream& os, const Vector& v);

struct Ray {
    Vector origin, direction;
};

class Triangle {
public: 
    Triangle(Vector v0, Vector v1, Vector v2);

    bool intersects(Ray ray);
private:
   Vector v0, v1, v2;
   Vector normal;

   bool leftOfEdge(const Vector& p, const Vector& v0, const Vector& v1);
};

class Matrix {
public:
    Matrix() = default;
    Matrix(std::array<std::array<float, 3>, 3> m);

    friend Matrix operator*(const Matrix& lhs, const Matrix& rhs);
    friend Vector operator*(const Vector& lhs, const Matrix& rhs);
    
    friend bool operator==(const Matrix& lhs, const Matrix& rhs);
private:
    std::array<std::array<float, 3>, 3> m  = {};;
};



class Camera {
public:
    Camera(Vector position, Matrix rotation);

    Matrix getRotation();
    Vector getPosition();

    void dolly(const float z);
    void jib(const float y);
    void truck(const float x);


    void pan(const float degs);
    // void tilt(const float degs);
private:
    Matrix rotation;
    Vector position;
};

#endif