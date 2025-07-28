#include "shader.hxx"
#include "interx.hxx"
#include "scene.hxx"
#include "types.hxx"
#include <numbers>
#include <iostream>
#include <cmath>

namespace shader {

Shader::Shader(Tracer* tracer, const vector<Mesh>& objects, const vector<Material>& materials, const vector<Light>& lights, const Color background) 
    : tracer{tracer}, objects{objects}, materials(materials), lights(lights), background(background) {
}

Color Shader::shade(const Ray& ray, int depth) {
    Intersection interx = tracer->trace(ray);
    
    if (interx.distance == INFINITY || depth == 5) {
        return background;
    }
    
    Material material = materials[interx.materialIndex];
    if (material.type == MaterialType::Diffuse) {
        return shadeDiffuse(interx);
    } else if (material.type == MaterialType::Reflective) {
        return shadeReflective(interx, depth);
    } else if (material.type == MaterialType::Refractive) {
        return shadeRefractive(interx, depth);
    } else {
        return background;
    }
}


Ray Shader::generateReflectionRay(const Intersection& interx, Vector bias) {
    const Vector& hitNormal = interx.normal;
    return Ray{
        .origin = interx.point + bias,
        .direction = interx.direction - 2 * interx.direction.dot(hitNormal) * hitNormal, 
    };
}

Ray Shader::generateRefractionRay(const Intersection& interx, float refractIndex) {
    Vector adjustedNormal = interx.normal;
    float snellCoeff = 1 / refractIndex;
    if (interx.normal.dot(interx.direction ) > 0) {
        adjustedNormal = -adjustedNormal;
        snellCoeff = 1/snellCoeff;
    }

    auto angleCos = -adjustedNormal.dot(interx.direction);
    auto angleSin = std::sqrt(1 - angleCos * angleCos);
    // if (angleSin > 1 / snellCoeff) {
    //     return Ray{};
    // }

    auto refractionAngleSin = angleSin * snellCoeff;
    auto refractionAngleCos = std::sqrt(1 - refractionAngleSin * refractionAngleSin);

    Vector A = refractionAngleCos * -adjustedNormal;

    Vector E = angleCos * adjustedNormal;
    Vector D = interx.direction + E;
    Vector C = D; C.normalize();

    Vector B = refractionAngleSin * C;

    Vector refractionBias = -adjustedNormal * 1e-4;
    return Ray{
        .origin = interx.point + refractionBias ,
        .direction = A + B, 
    };
}

Ray Shader::generateShadowRay(const Intersection& interx, const Vector& hitNormal, const Vector& lightDir) {
    return Ray {
        .origin = interx.point + hitNormal * 1e-4f,
        .direction = lightDir,
    };
}

Color Shader::shadeRefractive(const Intersection& interx, int depth) {
    Color finalColor = {0, 0, 0};

    Material material = materials[interx.materialIndex];

    Vector adjustedNormal = interx.normal;
    if (interx.normal.dot(interx.direction ) > 0) {
        adjustedNormal = -adjustedNormal;
    }

    float angleCos = interx.normal.dot(interx.direction);
    if (angleCos > 0) {
        angleCos = -angleCos;
    }

    float fresnelCoeff = 0.5 * pow(1.0 + angleCos, 5);
    
    auto reflectionRay = generateReflectionRay(interx, adjustedNormal * 1e-4);
    auto refractionRay = generateRefractionRay(interx, material.ior);
    // if (refractionRay == Ray{}) {
    //     return shade(reflectionRay, depth + 1) ;
    // }

    return fresnelCoeff * shade(reflectionRay, depth + 1) + (1 - fresnelCoeff) * shade(refractionRay, depth + 1);
}  


Color Shader::shadeReflective(const Intersection& interx, int depth) {
    Material material = materials[interx.materialIndex];

    Color albedo = material.albedo;
    Ray reflectedRay = generateReflectionRay(interx); 

    return albedo * shade(reflectedRay, depth + 1);
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

    Intersection shadowInterx = tracer->traceShadow(generateShadowRay(interx, hitNormal, lightDir), sphereRadius);
    if (shadowInterx.distance != INFINITY) {
        return Color{};
    }
    
    float sphereArea = 4 * std::numbers::pi * sphereRadius * sphereRadius;
    return material.albedo * (light.intensity / sphereArea * cosLaw);
}

ShaderFactory::ShaderFactory(TracerFactory tracerFactory) : tracerFactory{tracerFactory} {}

Shader* ShaderFactory::factory(const Scene& scene) {
    return new Shader {tracerFactory(scene), scene.objects, scene.materials, scene.lights, scene.settings.background};
}

}