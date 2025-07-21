#ifndef ENGINENEW_HXX
#define ENGINENEW_HXX

#include <vector>
#include <functional>
#include "interx.hxx"
#include "scene.hxx"


class Drawer {
public:
    virtual ~Drawer() = default;
    virtual void draw(int x, int y, const Color& c) = 0;
};

typedef std::function<Color(int, int)> PixelRenderer;

class EngineNew {
public:
    EngineNew(PixelRenderer pixelRenderer);

    void render(Scene& scene, Drawer& drawer);
private:
    PixelRenderer pixelRenderer;
};

#endif // ENGINENEW_HXX