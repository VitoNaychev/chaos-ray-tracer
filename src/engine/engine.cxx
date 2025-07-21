#include "engine.hxx"
#include "interx.hxx"
#include <algorithm>
#include <numbers>
#include <iostream>


Vector calculateSmoothHitNormal(const Intersection& intersection) {
    const Triangle& tri = *intersection.triangle;

    Vector v0p = intersection.point - tri[0];
    Vector v0v1 = tri[1]- tri[0];
    Vector v0v2 = tri[2] - tri[0];

    float u = v0v1.cross(v0p).length() / v0v1.cross(v0v2).length();
    float v = v0v2.cross(v0p).length() / v0v1.cross(v0v2).length();

    const Vertex& v0N = dynamic_cast<const Vertex&>(tri[0]);
    const Vertex& v1N = dynamic_cast<const Vertex&>(tri[1]);
    const Vertex& v2N = dynamic_cast<const Vertex&>(tri[2]);

    Vector averagedNormal =  v * v1N.normal + u * v2N.normal + (1 - u - v) * v0N.normal;
    averagedNormal.normalize();

    return averagedNormal;
}

Engine::Engine(Scene scene, Drawer& drawer) 
    : scene{scene}, drawer{drawer}, raygen{scene.settings.width, scene.settings.height, scene.camera} {}


void Engine::render() {
    Settings& settings = scene.settings;
    
    for (int i = 0; i < settings.height; i++) {
        for (int j = 0; j < settings.width; j++) {
            auto ray = raygen.generate(j, i);
            drawer.draw(i, j, trace(ray));
        }
    }
}

Color Engine::trace(const Ray& ray, int depth) {
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
        for (auto& light : scene.lights) {
            finalColor += shade(closestIntersection, objectMaterial, light);
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
        return objectMaterial.albedo * trace(reflectedRay, depth + 1);
    } 
    return Color{0, 0, 1};
}

Color Engine::shade(const Intersection& intersection, const Material& material, const Light& light) {
    Vector lightDir = light.position - intersection.point;
    float sphereRadius = lightDir.length();

    Vector hitNormal = intersection.triangle->getNormal();
    if (material.smooth) {
        hitNormal = calculateSmoothHitNormal(intersection);
    }  

    lightDir.normalize();
    float cosLaw = lightDir.dot(hitNormal);
    if (cosLaw <= 0) {
        return Color{0, 0, 0};
    }

    for (auto& object : scene.objects) {
        for (auto& tri : object.getTriangles()) {
            Ray shadowRay = {
                .origin = intersection.point + hitNormal * 1e-2,
                .direction = lightDir,
            };
            Intersection shadowIntersection = intersect(shadowRay, tri);
            if (shadowIntersection.distance < sphereRadius) {
                return Color{0, 0, 0};
            }
        }
    }

    float sphereArea = 4 * std::numbers::pi * sphereRadius * sphereRadius;
    return material.albedo * (light.intensity / sphereArea * cosLaw);
}