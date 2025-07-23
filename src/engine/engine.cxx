#include <iostream>
#include <thread>

#include "engine.hxx"
#include "scene.hxx"

#include "raygen.hxx"
#include "tracer.hxx"
#include "shader.hxx"

namespace engine{

Engine::Engine(RayGenFactory raygenFactory, ShaderFactory shaderFactory) 
    : raygenFactory{raygenFactory}, shaderFactory{shaderFactory} {}

void Engine::render(Scene& scene, Drawer& drawer) {
    auto raygen = raygenFactory(scene);
    auto shader = shaderFactory(scene);

    for (int i = 0; i < scene.settings.height; i++) {
        for (int j = 0; j < scene.settings.width; j++) {
            Ray ray = raygen->generate(j, i);
            drawer.draw(i, j, shader->shade(ray));
        }
    }
    drawer.flush();
}

void Engine::renderThreaded(Scene& scene, Drawer& drawer) {
    auto raygen = raygenFactory(scene);
    auto shader = shaderFactory(scene);
    
    std::vector<std::thread> threads;

    for (int i = 0; i < 8; ++ i) {
        int startX = scene.settings.width / 8 * i;
        int endX = scene.settings.width / 8 * (i+1);
        threads.push_back(std::thread(renderPart, &drawer, raygen, shader, startX, 0, endX, scene.settings.height));
    }

    for (auto& t : threads) {
        t.join();
    }
    drawer.flush();
}

void renderPart(Drawer* drawer, RayGen* raygen, Shader* shader, int startX, int startY, int endX, int endY) {
    for (int i = startY; i < endY; i++) {
        for (int j = startX; j < endX; j++) {
            Ray ray = raygen->generate(j, i);
            drawer->draw(i, j, shader->shade(ray));
        }
    }
}

}
