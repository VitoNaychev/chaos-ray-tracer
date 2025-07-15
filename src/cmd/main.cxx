#include <iostream>
#include <fstream>
#include <cmath>
#include <algorithm>

#include "types.hxx"
#include "crtscene.hpp"
#include "raygen.hxx"
#include "interx.hxx"

#include "pixeldrawer.hpp"
#include "ppm.hpp"

using namespace std;

vector<Triangle> getTriangles(vector<Mesh> objects) {
    vector<Triangle> triangles;

    for (auto& object: objects) {
        for (size_t i = 0; i < object.triangleIndicies.size(); i += 3) {
            Vector& v0 = object.vertices[object.triangleIndicies[i]];
            Vector& v1 = object.vertices[object.triangleIndicies[i+1]];
            Vector& v2 = object.vertices[object.triangleIndicies[i+2]];

            Triangle triangle(v0, v1, v2);
            triangles.push_back(triangle);
        }
    }

    return triangles;
}

pixeldrawer::Color toPixelColor(Color c) {
    return pixeldrawer::Color {
        .r = static_cast<uint8_t>(min(1.0f, c.r) * 255),
        .g = static_cast<uint8_t>(min(1.0f, c.g) * 255),
        .b = static_cast<uint8_t>(min(1.0f, c.b) * 255),
    };
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        cerr << "Usage: renderer <crtscene-path> <ouput-image-path>";
        return 1;
    }

    std::ifstream sceneFile(argv[1]); 
    if (!sceneFile) {
        std::cerr << "Failed to open file.\n";
    }
    Scene scene = parseCRTScene(sceneFile);
    Settings& settings = scene.settings;

    std::ofstream outFile(argv[2]); 
    if (!outFile) {
        std::cerr << "Failed to open file.\n";
    }
    pixeldrawer::PPMPixelDrawer drawer(outFile, settings.width, settings.height);

    auto triangles = getTriangles(scene.objects);
    RayGenerator raygen(settings.width, settings.height, scene.camera);

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

            if (closestIntersection.distance <=10000) {
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
    return 0;
}

