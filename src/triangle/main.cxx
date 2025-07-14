#include <iostream>
#include <fstream>
#include <numbers>

#include "types.hpp"
#include "raygen.hxx"

#include "pixeldrawer.hpp"
#include "ppm.hpp"

int main() {
    int width = 1920;
    int height = 1080;

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
    pixeldrawer::PixelDrawer* drawer = new pixeldrawer::PPMPixelDrawer(outFile, width, height);

    Vector position {0, 0, -1};
    Matrix rotation = Matrix::identity();
    Camera camera(position, rotation);

    RayGenerator raygen(width, height, camera);


    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            auto ray = raygen.generate(j, i);

            bool hit = false;
            for (auto& tri : triangles) {
                if (tri.intersects(ray)) {
                    hit = true;
                    break;
                }
            }
            if (hit) {
                drawer->draw(pixeldrawer::Color{255, 255, 255});
            } else {
                drawer->draw(pixeldrawer::Color{255, 0, 0});
            }

        }
        
    }

    outFile.close();
}