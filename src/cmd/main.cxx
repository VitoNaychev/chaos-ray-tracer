#include <iostream>
#include <fstream>
#include <cmath>

#include "types.hxx"
#include "crtscene.hpp"
#include "raygen.hxx"

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

            float minDistance = INFINITY;
            for (auto& tri : triangles) {
               float dist = tri.intersectionDistance(ray);
                if (dist != -1 && dist < minDistance) {
                    minDistance = dist;
                }
            }

            if (minDistance != INFINITY) {
                drawer.draw(pixeldrawer::Color{255, 255, 255});
            } else {
                drawer.draw(pixeldrawer::Color{255, 0, 0});
            }

        }
        
    }
}

