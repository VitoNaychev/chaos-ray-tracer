#include <iostream>
#include <fstream>
#include <memory>

#include <chrono>

#include "ppm.hpp"
#include "crtscene.hpp"
#include "engine.hxx"
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

    ifstream sceneFile(argv[1]); 
    if (!sceneFile) {
        cerr << "Failed to open file.\n";
    }
    Scene scene = parseCRTScene(sceneFile);
    Settings& settings = scene.settings;

    auto outFile = new ofstream(argv[2]); 
    if (!outFile) {
        cerr << "Failed to open file.\n";
    }
    PPMDrawer drawer(outFile, settings.width, settings.height);


    shader::ShaderFactory shaderFactory {tracer::factory};
    auto shaderFactoryFn = [&shaderFactory](const Scene& scene) {
        return shaderFactory.factory(scene);
    };    

    engine::RayTraceEngine engine(raygen::factory, shaderFactoryFn);
    engine.renderThreaded(scene, drawer);

    return 0;
}

