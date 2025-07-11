#include <iostream>
#include <fstream>

#include "types.hpp"
#include "raygen.hxx"

#include "pixeldrawer.hpp"
#include "ppm.hpp"

int main() {
    int width = 5;
    int height = 5;

    std::vector<Triangle> triangles = {
        Triangle({-1.75, -1.75, -3}, {1.75, -1.75, -3}, {0, 1.75, -3}), // base triangle
    //     Triangle({-1.75, -1.75, -3}, {0, -3.0, -3}, {1.75, -1.75, -3}), // bottom spike
    //     Triangle({1.75, -1.75, -3}, {3.0, 0, -3}, {0, 1.75, -3}),       // right spike
    //     Triangle({0, 1.75, -3}, {-3.0, 0, -3}, {-1.75, -1.75, -3})      // left spike
    };

    std::ofstream outFile("triangle.ppm"); 
    if (!outFile) {
        std::cerr << "Failed to open file.\n";
    }

    PixelDrawer* drawer = new PPMPixelDrawer(outFile, width, height);

    auto rays = generateRays(width, height, Vector {0, 0, 0});
    for (auto& r : rays) {
        bool hit = false;
        for (auto& tri : triangles) {
            if (tri.intersects(r)) {
                hit = true;
                break;
            }
        }
        if (hit) {
            drawer->draw(Color{255, 255, 255});
        } else {
            drawer->draw(Color{255, 0, 0});
        }
    }

    outFile.close();
}