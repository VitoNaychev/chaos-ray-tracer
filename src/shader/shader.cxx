#include "shader.hxx"
#include "interx.hxx"
#include <numbers>
#include <iostream>

namespace shader {

Shader::Shader(Tracer* tracer, const vector<Mesh>& objects, const vector<Material>& materials, const vector<Light>& lights, const Color background) 
    : tracer{tracer}, objects{objects}, materials(materials), lights(lights), background(background) {
}

Color Shader::shade(const Ray& ray) {
    Intersection interx = tracer->trace(ray);
    
    if (interx.distance == INFINITY) {
        return background;
    }
    
    Material material = materials[interx.materialIndex];

    if (material.type == MaterialType::Diffuse) {
        return shadeDiffuse(interx);
    } else {
        return shadeReflective(interx);
    }
}


Ray Shader::generateReflectionRay(const Intersection& interx) {
    const Vector& hitNormal = interx.normal;
    return Ray{
        .origin = interx.point,
        .direction = interx.direction - 2 * interx.direction.dot(hitNormal) * hitNormal, 
    };
}

Ray Shader::generateShadowRay(const Intersection& interx, const Vector& hitNormal, const Vector& lightDir) {
    return Ray {
        .origin = interx.point + hitNormal * 1e-2f,
        .direction = lightDir,
    };
}

Color Shader::shadeReflective(const Intersection& interx) {
    Material material = materials[interx.materialIndex];

    Color albedo = material.albedo;
    Ray currentRay = generateReflectionRay(interx); 
    
    for (int i = 0; i < 5; i++) {
        Intersection reflectionInterx = tracer->trace(currentRay);
        
        if (reflectionInterx.distance == INFINITY) {
            return albedo * background;
        }
        
        material = materials[reflectionInterx.materialIndex];
        if (material.type == MaterialType::Reflective) {
            albedo = albedo * material.albedo;

            currentRay = generateReflectionRay(reflectionInterx);
            continue;
        } else if (material.type == MaterialType::Diffuse) {
            return albedo * shadeDiffuse(reflectionInterx);
        }
    }
    return background;
}

Color Shader::shadeDiffuse(const Intersection& interx) {
    Color finalColor{0, 0, 0};
    for (auto& light : lights) {
        finalColor += shadeDiffuseSingleLight(interx, light);
    }
    return finalColor;
}

Color Shader::shadeDiffuseSingleLight(const Intersection& interx, const Light& light) {
    Vector lightDir = light.position - interx.point;
    float sphereRadius = lightDir.length();
    
    Vector hitNormal = interx.normal;
    Material material = materials[interx.materialIndex];

    lightDir.normalize();
    float cosLaw = lightDir.dot(hitNormal);
    if (cosLaw <= 0) {
        return Color{0, 0, 0};
    }
    

    Intersection shadowInterx = tracer->trace(generateShadowRay(interx, hitNormal, lightDir));
    if (shadowInterx.distance < sphereRadius) {
        return Color{0, 0, 0};
    }
    
    float sphereArea = 4 * std::numbers::pi * sphereRadius * sphereRadius;
    return material.albedo * (light.intensity / sphereArea * cosLaw);
}

ShaderFactory::ShaderFactory(TracerFactory tracerFactory) : tracerFactory{tracerFactory} {}

Shader* ShaderFactory::factory(const Scene& scene) {
    return new Shader {tracerFactory(scene), scene.objects, scene.materials, scene.lights, scene.settings.background};
}

}