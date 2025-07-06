#include <vector>
#include "types.hpp"
#include "transformations.hpp"

std::vector<Pixel> getPixels(int width, int height) {
    std::vector<Pixel> pixels;
    for (int i = 0; i < width; ++ i) {
        for (int j = 0; j < height; j++) {
            Pixel p = Pixel {
                .x = static_cast<float>(i),
                .y = static_cast<float>(j)
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