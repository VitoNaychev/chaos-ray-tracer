#include <iostream>
#include <fstream>
#include <memory>

#include <chrono>

#include "ppm.hpp"
#include "crtscene.hpp"
#include "enginenew.hxx"
#include "raygen.hxx"
#include "tracer.hxx"
#include "shader.hxx"

using namespace std;
using namespace std::chrono;


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
    PPMDrawer drawer(outFile, settings.width, settings.height);


    shader::ShaderFactory shaderFactory {tracer::factory};
    auto shaderFactoryFn = [&shaderFactory](const Scene& scene) {
        return shaderFactory.factory(scene);
    };    

    engine::EngineNew engine(raygen::factory, shaderFactoryFn);
    engine.render(scene, drawer);

    return 0;
}

