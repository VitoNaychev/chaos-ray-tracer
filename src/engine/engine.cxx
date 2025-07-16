#include "engine.hxx"
#include "interx.hxx"
#include "shade.hxx"
#include <algorithm>
#include <iostream>

pixeldrawer::Color toPixelColor(Color c) {
    return pixeldrawer::Color {
        .r = static_cast<uint8_t>(std::min(1.0f, c.r) * 255),
        .g = static_cast<uint8_t>(std::min(1.0f, c.g) * 255),
        .b = static_cast<uint8_t>(std::min(1.0f, c.b) * 255),
    };
}

Engine::Engine(Scene scene, pixeldrawer::PixelDrawer& drawer) 
    : scene{scene}, drawer{drawer}, raygen{scene.settings.width, scene.settings.height, scene.camera} {}


void Engine::render() {
    Settings& settings = scene.settings;
    
    for (int i = 0; i < settings.height; i++) {
        for (int j = 0; j < settings.width; j++) {
            auto ray = raygen.generate(j, i);

            Intersection closestIntersection {
                .distance = INFINITY,
            };
            for (auto& object : scene.objects) {
                for (auto& tri : object.getTriangles()) {
                    Intersection i = intersect(ray, tri);
                    if (i.distance < closestIntersection.distance) {
                        closestIntersection = i;
                    }
                }
            }
            if (closestIntersection.distance != INFINITY) {
                const Triangle& tri = *closestIntersection.triangle;

                Vector v0p = closestIntersection.point - tri.getV0();
                Vector v0v1 = tri.getV1() - tri.getV0();
                Vector v0v2 = tri.getV2() - tri.getV0();

                float u = v0v1.cross(v0p).length() / v0v1.cross(v0v2).length();
                float v = v0v2.cross(v0p).length() / v0v1.cross(v0v2).length();

                Color finalColor {u, v, 0};
                // for (auto light : scene.lights) {
                //     finalColor += shade(closestIntersection, light, scene.objects);
                // }
                drawer.draw(toPixelColor(finalColor));
            } else {
                drawer.draw(toPixelColor(settings.background));
            }
        }
    }
}