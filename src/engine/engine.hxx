#ifndef ENGINENEW_HXX
#define ENGINENEW_HXX

#include <vector>
#include <functional>
#include <memory>
#include "interx.hxx"
#include "scene.hxx"

namespace engine {

class Drawer {
public:
    virtual ~Drawer() = default;
    virtual void draw(int x, int y, const Color& c) = 0;
    virtual void flush() = 0;
};

class RayGen {
public:
    virtual ~RayGen() = default;
    virtual Ray generate(int x, int y) = 0;
};

class Shader {
public:
    virtual ~Shader() = default;
    virtual Color shade(const Ray& ray) = 0;
};

typedef std::function<RayGen*(const Scene&)> RayGenFactory;
typedef std::function<Shader*(const Scene&)> ShaderFactory;

class Engine {
public:
    Engine(RayGenFactory raygenFactory, ShaderFactory shaderFactory);

    void render(Scene& scene, Drawer& drawer);
    void renderThreaded(Scene& scene, Drawer& drawer);
private:
    RayGenFactory raygenFactory;
    ShaderFactory shaderFactory;
};

void renderPart(Drawer* drawer, RayGen* raygen, Shader* shader, int startX, int startY, int endX, int endY);


class Renderer {
public:
    Renderer(RayGen& raygen, Shader& shader);
    Color render(int x, int y);
private:
    RayGen& raygen;
    Shader& shader;
};

}

#endif // ENGINENEW_HXX