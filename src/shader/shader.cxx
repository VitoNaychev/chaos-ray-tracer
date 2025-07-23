#include "shader.hxx"
#include "interx.hxx"
#include <numbers>
#include <iostream>

namespace shader {

Vector calculateSmoothHitNormal(const Intersection& intersection) {
    const Triangle& tri = *intersection.triangle;

    Vector v0p = intersection.point - tri[0];
    Vector v0v1 = tri[1]- tri[0];
    Vector v0v2 = tri[2] - tri[0];

    float u = v0v1.cross(v0p).length() / v0v1.cross(v0v2).length();
    float v = v0v2.cross(v0p).length() / v0v1.cross(v0v2).length();

    const Vertex& v0N = dynamic_cast<const Vertex&>(tri[0]);
    const Vertex& v1N = dynamic_cast<const Vertex&>(tri[1]);
    const Vertex& v2N = dynamic_cast<const Vertex&>(tri[2]);

    Vector averagedNormal =  v * v1N.normal + u * v2N.normal + (1 - u - v) * v0N.normal;
    averagedNormal.normalize();

    return averagedNormal;
}

Shader::Shader(Tracer* tracer, const vector<Mesh>& objects, const vector<Material>& materials, const vector<Light>& lights, const Color background) 
    : tracer{tracer}, objects{objects}, materials(materials), lights(lights), background(background) {
}

Color Shader::shade(const Ray& ray) {
    Intersection interx = tracer->trace(ray);
    
    if (interx.distance == INFINITY) {
        return background;
    }
    
    Material material = materials[interx.object->getMaterialIndex()];

    if (material.type == MaterialType::Diffuse) {
        return shadeDiffuse(interx);
    } else {
        return shadeReflective(interx);
    }
}


Ray Shader::generateReflectionRay(const Intersection& interx) {
    const Vector& hitNormal = interx.triangle->getNormal();
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
    Material material = materials[interx.object->getMaterialIndex()];

    Color albedo = material.albedo;
    Ray currentRay = generateReflectionRay(interx); 
    
    for (int i = 0; i < 5; i++) {
        Intersection reflectionInterx = tracer->trace(currentRay);
        
        if (reflectionInterx.distance == INFINITY) {
            return albedo * background;
        }
        
        material = materials[reflectionInterx.object->getMaterialIndex()];
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
    
    Vector hitNormal = interx.triangle->getNormal();
    Material material = materials[interx.object->getMaterialIndex()];
    
    if (material.smooth) {
        hitNormal = calculateSmoothHitNormal(interx);
    }  
    
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