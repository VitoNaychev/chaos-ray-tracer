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
            drawer.draw(toPixelColor(traceRay(ray)));
        }
    }
}

Color Engine::traceRay(const Ray& ray, int depth) {
    const std::vector<Mesh>& objects = scene.objects;

    Intersection closestIntersection {
        .distance = INFINITY,
    };
    Material objectMaterial;
    for (auto& object : scene.objects) {
        for (auto& tri : object.getTriangles()) {
            Intersection i = intersect(ray, tri);
            if (i.distance < closestIntersection.distance) {
                closestIntersection = i;
                objectMaterial = scene.materials[object.getMaterialIndex()];
            }
        }
    }

    if (closestIntersection.distance == INFINITY) {
        return scene.settings.background;
    }

    if (objectMaterial.type == MaterialType::Diffuse) {
        Color finalColor {0, 0, 0};
        for (auto light : scene.lights) {
            finalColor += shade(closestIntersection, objectMaterial, light, scene.objects);
        }
        return finalColor;
    } else if (objectMaterial.type == MaterialType::Reflective) {
        if (depth == 5) {
            return scene.settings.background;
        }
        
        const Vector& hitNormal = closestIntersection.triangle->getNormal();
        Ray reflectedRay {
            .origin = closestIntersection.point,
            .direction = ray.direction - 2 * ray.direction.dot(hitNormal) * hitNormal, 
        };
        return objectMaterial.albedo * traceRay(reflectedRay, depth + 1);
    } 
    return Color{0, 0, 1};
}
