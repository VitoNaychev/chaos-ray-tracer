#ifndef SCENE_HPP
#define SCENE_HPP

#include "types.hxx"

struct Vertex : public Vector {
    using Vector::Vector;
    
    Vector normal;
};


enum class MovementType {
    Dolly,
    Jib,
    Truck,

    Pan,
    Tilt,
    Roll,

    Arc,

    Zoom,
};

class Camera {
public:
    Camera(Vector position, Matrix rotation);

    Matrix getRotation();
    Vector getPosition();

    void move(MovementType movement, float units);

    void dolly(const float z);
    void jib(const float y);
    void truck(const float x);

    void pan(const float degs);
    // void tilt(const float degs);

    void setFOV(const float degs);
    float getFOV() const;
private:
    Matrix rotation;
    Vector position;
    float fov;
};

struct Settings {
    int width, height;
    Color background;
};

struct Light {
    Vector position;
    int intensity;
};

enum MaterialType {
    Diffuse,
    Reflective,
    Refractive,
    Constant,
};

struct Material {
    MaterialType type;
    Color albedo;
    float ior;
    bool smooth; 
};

struct Mesh{
    Mesh(std::vector<Vertex> vertices, std::vector<int> triangleIndicies, int materialIndex);

    const std::vector<Vertex>& getVertices();
    const std::vector<int>& getTriangleIndicies();
    const int getMaterialIndex() const;

    const std::vector<Triangle>& getTriangles() const;
private:
    std::vector<Vertex> vertices;
    std::vector<int> triangleIndicies;
    int materialIndex;

    std::vector<Triangle> triangles;
};

struct Scene {
    Settings settings;
    Camera camera;

    std::vector<Light> lights;
    std::vector<Material> materials;
    std::vector<Mesh> objects;
};
    

#endif