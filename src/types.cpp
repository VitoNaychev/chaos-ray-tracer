#include <iostream>
#include "types.hpp"

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


bool operator==(const Vector& v1, const Vector& v2) {
    return v1.x == v2.x && v1.y == v2.y && v1.z == v2.z; 

}

std::ostream& operator<<(std::ostream& os, const Vector& v) {
    return os << "Vector{x=" << v.x << ", y=" << v.y << ", z=" << v.z << "}";
}
