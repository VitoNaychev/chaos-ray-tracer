#ifndef SCENE_HPP
#define SCENE_HPP

#include "types.hxx"

class Vertex : public Vector {
public:
    Vector normal;
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

struct Settings {
    int width, height;
    Color background;
};

struct Light {
    Vector position;
    int intensity;
};

struct Mesh{
    Mesh(std::vector<Vertex> vertices, std::vector<int> triangleIndicies);
    
    const std::vector<Vertex>& getVertices();
    const std::vector<int>& getTriangleIndicies();

    const std::vector<Triangle>& getTriangles();
private:
    std::vector<Vertex> vertices;
    std::vector<int> triangleIndicies;

    std::vector<Triangle> triangles;
};

struct Scene {    
    Settings settings;
    Camera camera;

    std::vector<Light> lights;
    std::vector<Mesh> objects;
};
    

#endif