#include <iostream>
#include <fstream>

#include "pixeldrawer.hpp"
#include "ppm.hpp"
#include "circle.hpp"

using namespace std;

unsigned int Factorial( unsigned int number ) {
    return number <= 1 ? number : Factorial(number-1)*number;
}

int main() {
    std::ofstream outFile("output.ppm"); 
    if (!outFile) {
        std::cerr << "Failed to open file.\n";
        return 1;
    }

    PixelDrawer* drawer = new PPMPixelDrawer(outFile, 50, 50);
    CircleDrawer* circleD = new CircleDrawer(50, 50, *drawer);

    Color fill = Color{.r =  200, .g = 200, .b = 0};
    Color bg = Color{.r =  0, .g = 200, .b = 100};
    circleD->draw(Pixel{25, 25}, 5, fill, bg);

    outFile.close(); // optional, done automatically on destruction
    return 0;
}