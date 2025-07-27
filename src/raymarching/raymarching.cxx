#include "raymarching.hxx"
#include <cmath>
#include <algorithm>

namespace raymarching {

RayMarchingTracer::RayMarchingTracer(const vector<Material>& materials, FractalType fractalType, Vector juliaC) 
    : materials(materials), fractalType(fractalType), juliaC(juliaC) {}

float RayMarchingTracer::mandelbulbSDF(const Vector& pos) {
    Vector z = pos;
    float dr = 1.0f;
    float r = 0.0f;
    
    const float power = 8.0f;
    const int maxIterations = 100;
    const float bailout = 2.0f;
    
    for (int i = 0; i < maxIterations; i++) {
        r = z.length();
        if (r > bailout) break;
        
        // Convert to spherical coordinates
        float theta = acos(z.z / r);
        float phi = atan2(z.y, z.x);
        
        // Update derivative for distance estimation
        dr = pow(r, power - 1.0f) * power * dr + 1.0f;
        
        // Apply power and convert back to cartesian
        float zr = pow(r, power);
        theta = theta * power;
        phi = phi * power;
        
        z = Vector(
            sin(theta) * cos(phi),
            sin(phi) * sin(theta), 
            cos(theta)
        ) * zr + pos;
    }
    
    // Distance estimator formula
    return 0.5f * log(r) * r / dr;
}

float RayMarchingTracer::juliaSetSDF(const Vector& pos) {
    Vector z = pos;
    Vector dz = Vector(1.0f, 0.0f, 0.0f);
    
    const int maxIterations = 100;
    const float bailout = 4.0f;
    
    for (int i = 0; i < maxIterations; i++) {
        float r2 = z.dot(z);
        if (r2 > bailout) break;
        
        // Quaternion derivative: dz = 2*z*dz
        dz = Vector(
            2.0f * (z.x * dz.x - z.y * dz.y - z.z * dz.z),
            2.0f * (z.x * dz.y + z.y * dz.x),
            2.0f * (z.x * dz.z + z.z * dz.x)
        );
        
        // Quaternion iteration: z = z² + c
        z = Vector(
            z.x * z.x - z.y * z.y - z.z * z.z + juliaC.x,
            2.0f * z.x * z.y + juliaC.y,
            2.0f * z.x * z.z + juliaC.z
        );
    }
    
    float r = z.length();
    float dr = dz.length();
    return 0.5f * r * log(r) / dr;
}

float RayMarchingTracer::evaluateSDF(const Vector& pos) {
    switch (fractalType) {
        case FractalType::Mandelbulb:
            return mandelbulbSDF(pos);
        case FractalType::JuliaSet:
            return juliaSetSDF(pos);
        default:
            return mandelbulbSDF(pos);
    }
}

Vector RayMarchingTracer::calculateSDFNormal(const Vector& point) {
    const float epsilon = 0.001f;
    
    Vector gradient(
        evaluateSDF(point + Vector(epsilon, 0, 0)) - 
        evaluateSDF(point - Vector(epsilon, 0, 0)),
        
        evaluateSDF(point + Vector(0, epsilon, 0)) - 
        evaluateSDF(point - Vector(0, epsilon, 0)),
        
        evaluateSDF(point + Vector(0, 0, epsilon)) - 
        evaluateSDF(point - Vector(0, 0, epsilon))
    );
    
    gradient.normalize();
    return gradient;
}

shader::Intersection RayMarchingTracer::trace(const Ray& ray) {
    Vector pos = ray.origin;
    float totalDist = 0.0f;
    const int maxSteps = 100;
    const float precision = 0.001f;
    const float maxDist = 100.0f;
    
    for (int step = 0; step < maxSteps; step++) {
        float dist = evaluateSDF(pos);
        
        if (dist < precision) {
            // HIT! Return intersection with computed normal
            return shader::Intersection {
                .point = pos,
                .distance = totalDist,
                .direction = ray.direction,
                .normal = calculateSDFNormal(pos),
                .materialIndex = 0  // Use first material
            };
        }
        
        if (totalDist > maxDist) break;
        
        pos = pos + ray.direction * dist;
        totalDist += dist;
    }
    
    return shader::Intersection {
        .distance = INFINITY,
    };
}

RayMarchingTracer* factory(const Scene& scene) {
    return new RayMarchingTracer(scene.materials, scene.fractalType, scene.juliaC);
}

}