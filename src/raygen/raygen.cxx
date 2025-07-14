#include <vector>
#include "types.hxx"
#include "raygen.hxx"


std::vector<Pixel> getPixels(int width, int height) {
    std::vector<Pixel> pixels;
    for (int i = 0; i < height; ++ i) {
        for (int j = 0; j < width; j++) {
            Pixel p = Pixel {
                .x = static_cast<float>(j),
                .y = static_cast<float>(i)
            };
            pixels.push_back(p);
        }
    }
    return pixels;
}


void getPixelCenters(std::vector<Pixel>& pixels) {
    for (auto& p : pixels) {
        p.x += 0.5;
        p.y += 0.5;
    }
}

void normalize(std::vector<Pixel>& pixels, int width, int height) {
    for (auto& p : pixels) {
        p.x /= width;
        p.y /= height;
    }
}

void getCV(std::vector<Pixel>& pixels) {
    for (auto& p : pixels) {
        p.x = (p.x * 2) - 1;
        p.y = -((p.y * 2) - 1);
    }
}

void adjustAspectRatio(std::vector<Pixel>& pixels, int width, int height) {
    for (auto& p : pixels) {
        p.x *= float(width)/height;
    }
}

std::vector<Vector> getVectors(std::vector<Pixel>& pixels) {
    std::vector<Vector> vectors;
    for (auto& p : pixels) {
        Vector v = Vector {
            .x = p.x,
            .y = p.y,
            .z = -1,
        };
        vectors.push_back(v);
    }
    return vectors;
}

void normalizeVectors(std::vector<Vector>& vectors) {
    for (auto& v : vectors) {
        v.normalize();
    }
}

std::vector<Ray> getRays(const Vector& origin, const std::vector<Vector>& vectors) {
    std::vector<Ray> rays;
    for (auto& v : vectors) {
        Ray r = {
            .origin = origin,
            .direction = v,
        };
        rays.push_back(r);
    }
    return rays;
}

std::vector<Ray> generateRays(int width, int height, Vector origin) {
    auto pixels = getPixels(width, height);
    getPixelCenters(pixels);

    normalize(pixels, width, height);
    getCV(pixels);
    adjustAspectRatio(pixels, width, height);

    auto vectors = getVectors(pixels);
    normalizeVectors(vectors);

    return getRays(origin, vectors);
}

RayGenerator::RayGenerator(int width, int height, Camera camera) : width {width}, height {height}, camera {camera} {}

Ray RayGenerator::generate(int x, int y) {
    Pixel p {float(x), float(y)};

    centerPixel(p);
    normalize(p);

    toCV(p);
    adjustAspectRatio(p);

    auto v = vectorFor(p);
    v.normalize();

    rotate(v);
    return Ray{
        .origin = camera.getPosition(),
        .direction = v,
    };
}

void RayGenerator::centerPixel(Pixel& p) {
    p.x += 0.5;
    p.y += 0.5;
}

void RayGenerator::normalize(Pixel& p) {
    p.x /= width;
    p.y /= height;
}

void RayGenerator::toCV(Pixel& p) {
    p.x = (p.x * 2) - 1;
    p.y = -((p.y * 2) - 1);
}

void RayGenerator::adjustAspectRatio(Pixel& p) {
    p.x *= float(width)/height;
}

Vector RayGenerator::vectorFor(const Pixel& p) {
    return Vector {
        .x = p.x,
        .y = p.y,
        .z = camera.getPosition().z,
    };
}

void RayGenerator::rotate(Vector& v) {
    v = v * camera.getRotation();
}