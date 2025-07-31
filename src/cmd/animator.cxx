#include <iostream>
#include <fstream>

#include <stdexcept>

#include "animparser.hxx"
#include "animation.hxx"
#include "crtscene.hpp"
#include "engine.hxx"
#include "raygen.hxx"
#include "tracer.hxx"
#include "shader.hxx"
#include "factory.hxx"

using namespace std;


int main(int argc, char *argv[]) {
    if (argc != 4) {
        cerr << "Usage: renderer <crtscene-path> <ouput-image-path>";
        return 1;
    }

    ifstream sceneFile(argv[1]); 
    if (!sceneFile) {
        cerr << "Failed to open scene file.\n";
    }
    auto scene = parseCRTScene(sceneFile);
    Settings& settings = scene.settings;

    ifstream animationFile(argv[2]); 
    if (!animationFile) {
        throw runtime_error( "Failed to open animation file.\n");
    }
    auto animation = parseAnimation(animationFile);

    auto directory = argv[3]; 
    if (!std::filesystem::exists(directory)) {
        throw std::runtime_error("Output directory does not exist");
    }

    shader::ShaderFactory shaderFactory {tracer::factory};
    auto shaderFactoryFn = [&shaderFactory](const Scene& scene) {
        return shaderFactory.factory(scene);
    };

    engine::RayTraceEngine engine(raygen::factory, shaderFactoryFn);
    PPMFactory ppmFactory {directory, settings.width, settings.height};

    Animator animator {&engine, &ppmFactory};
    animator.animate(scene, animation);
    
    return 0;
}

