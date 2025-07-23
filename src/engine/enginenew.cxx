#include <iostream>

#include "enginenew.hxx"
#include "scene.hxx"

#include "raygen.hxx"
#include "tracer.hxx"
#include "shader.hxx"

namespace engine{

EngineNew::EngineNew(RayGenFactory raygenFactory, ShaderFactory shaderFactory) 
    : raygenFactory{raygenFactory}, shaderFactory{shaderFactory} {}

void EngineNew::render(Scene& scene, Drawer& drawer) {
    auto raygen = raygenFactory(scene);
    auto shader = shaderFactory(scene);
    
    for (int i = 0; i < scene.settings.height; i++) {
        for (int j = 0; j < scene.settings.width; j++) {
            Ray ray = raygen->generate(j, i);
            drawer.draw(i, j, shader->shade(ray));
        }
    }
}

Renderer::Renderer(RayGen& raygen, Shader& shader) 
    : raygen {raygen}, shader {shader} {}

Color Renderer::render(int x, int y) {
    
}

}
