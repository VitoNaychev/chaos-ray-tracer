#include <iostream>
#include <thread>

#include "engine.hxx"
#include "scene.hxx"
#include "workqueue.hxx"

#include "raygen.hxx"
#include "tracer.hxx"
#include "shader.hxx"

namespace engine{


RayTraceEngine::RayTraceEngine(RayGenFactory raygenFactory, ShaderFactory shaderFactory) 
    : raygenFactory{raygenFactory}, shaderFactory{shaderFactory} {}

void RayTraceEngine::render(Scene& scene, Drawer& drawer) {
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

void RayTraceEngine::renderThreaded(Scene& scene, Drawer& drawer) {
    auto raygen = raygenFactory(scene);
    auto shader = shaderFactory(scene);
    
    WorkQueue workQueue {12};

    for (int i = 0; i < 10; ++ i) {
        for (int j = 0; j < 10; ++ j) {
            Edge start = {
                .x = scene.settings.width / 10 * i,
                .y = scene.settings.height / 10 * j,
            };
            Edge end = {
                .x = scene.settings.width / 10 * (i+1),
                .y = scene.settings.height / 10 * (j+1),
            };

            workQueue.push([this, &drawer, raygen, shader, start, end]() {
                this->renderSection(drawer, raygen, shader, start, end);
            });
        }
    }

    workQueue.wait();
    drawer.flush();
    
}

void RayTraceEngine::renderSection(Drawer& drawer, RayGen* raygen, Shader* shader, Edge start, Edge end) {
    for (int i = start.y; i < end.y; i++) {
        for (int j = start.x; j < end.x; j++) {
            Ray ray = raygen->generate(j, i);
            drawer.draw(i, j, shader->shade(ray));
        }
    }
}

}
