#ifndef TYPES_HPP
#define TYPES_HPP
 
struct Pixel {
    float x;
    float y;
};

bool operator==(const Pixel& p1, const Pixel& p2);

std::ostream& operator<<(std::ostream& os, const Pixel& p);

struct Vector {
    float x, y, z;
    float length();
    void normalize();
};

bool operator==(const Vector& v1, const Vector& v2);

std::ostream& operator<<(std::ostream& os, const Vector& v);

class Ray {
private:
    Vector origin, direction;
};

#endif