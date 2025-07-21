#include "enginenew.hxx"
#include "scene.hxx"

EngineNew::EngineNew(PixelRenderer pixelRenderer) : pixelRenderer {pixelRenderer} {
}

void EngineNew::render(Scene& scene, Drawer& drawer) {
    for (int i = 0; i < scene.settings.height; i++) {
        for (int j = 0; j < scene.settings.width; j++) {
            drawer.draw(i, j, pixelRenderer(i, j));
        }
    }
}