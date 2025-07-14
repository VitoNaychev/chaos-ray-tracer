#ifndef SCENE_HPP
#define SCENE_HPP

#include "types.hxx"

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

struct Mesh{
    std::vector<Vector> vertices;
    std::vector<int> triangleIndicies;
};

struct Settings {
    int width, height;
    Color background;
};

struct Scene {
    Settings settings;
    Camera camera;
    std::vector<Mesh> objects;
};
    

#endif