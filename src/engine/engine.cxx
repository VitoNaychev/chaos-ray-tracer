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
    : scene{scene}, drawer{drawer}, raygen{scene.settings.width, scene.settings.height, scene.camera} {
    initTriangles(this->scene.objects);
}

void Engine::initTriangles(const std::vector<Mesh>& objects) {
    for (auto& object: objects) {
        for (size_t i = 0; i < object.triangleIndicies.size(); i += 3) {
            const Vector& v0 = object.vertices[object.triangleIndicies[i]];
            const Vector& v1 = object.vertices[object.triangleIndicies[i+1]];
            const Vector& v2 = object.vertices[object.triangleIndicies[i+2]];

            Triangle triangle(v0, v1, v2);
            triangles.push_back(triangle);
        }
    }
}

void Engine::render() {
    Settings& settings = scene.settings;
    
    for (int i = 0; i < settings.height; i++) {
        for (int j = 0; j < settings.width; j++) {
            auto ray = raygen.generate(j, i);

            Intersection closestIntersection {
                .distance = INFINITY,
            };
            for (auto& tri : triangles) {
               Intersection i = intersect(ray, tri);
                if (i.distance < closestIntersection.distance) {
                    closestIntersection = i;
                }
            }

            if (closestIntersection.distance != INFINITY) {
                Color finalColor {0, 0, 0};
                for (auto light : scene.lights) {
                    finalColor += shade(closestIntersection, light, triangles);
                }
                drawer.draw(toPixelColor(finalColor));
            } else {
                drawer.draw(toPixelColor(settings.background));
            }
        }
    }
}