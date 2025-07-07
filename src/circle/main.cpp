#include <iostream>
#include <fstream>

#include "pixeldrawer.hpp"
#include "ppm.hpp"

#include "circle.hpp"
#include "visualizer.hpp"

#include "transformations.hpp"

using namespace std;

unsigned int Factorial( unsigned int number ) {
    return number <= 1 ? number : Factorial(number-1)*number;
}

void drawCircle() {
    std::ofstream outFile("output.ppm"); 
    if (!outFile) {
        std::cerr << "Failed to open file.\n";
    }

    PixelDrawer* drawer = new PPMPixelDrawer(outFile, 50, 50);
    CircleDrawer* circleD = new CircleDrawer(50, 50, *drawer);

    Color fill = Color{.r =  200, .g = 200, .b = 0};
    Color bg = Color{.r =  0, .g = 200, .b = 100};
    circleD->draw(Pixel{25, 25}, 5, fill, bg);

    outFile.close(); // optional, done automatically on destruction
}


int main() {
    int width = 1000;
    int height = 1000;

    std::ofstream outFile("normalized.ppm"); 
    if (!outFile) {
        std::cerr << "Failed to open file.\n";
    }

    PixelDrawer* drawer = new PPMPixelDrawer(outFile, width, height);
    PixelVisualizer* pixelVis = new PixelVisualizer(drawer);

    auto pixels = getPixels(width, height);
    getPixelCenters(pixels);

    normalize(pixels, width, height);
    getCV(pixels);
    adjustAspectRatio(pixels, width, height);

    auto vectors = getVectors(pixels);
    normalizeVectors(vectors);

    auto vectorVis = new VectorVisualizer(drawer);
    vectorVis->generate(vectors, 255);

    return 0;    

}