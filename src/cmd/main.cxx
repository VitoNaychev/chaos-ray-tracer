#include <iostream>
#include <fstream>
#include <cmath>
#include <algorithm>

#include "ppm.hpp"
#include "crtscene.hpp"
#include "engine.hxx"

using namespace std;


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

    Engine engine(scene, drawer);
    engine.render();

    return 0;
}

