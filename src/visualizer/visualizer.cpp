#include <vector>
#include <fstream>

#include "visualizer.hpp"

#include "types.hxx"
#include "pixeldrawer.hpp"


PixelVisualizer::PixelVisualizer(PixelDrawer* drawer) : drawer {drawer} {}

void PixelVisualizer::generate(std::vector<Pixel> pixels, float scaling_factor) {
    for (auto& p : pixels) {
        Color c = Color {
            .r = uint8_t(abs(int(p.x * scaling_factor)) % 255),
            .g = uint8_t(abs(int(p.y * scaling_factor)) % 255),
            .b = 0,
        };
        drawer->draw(c);
    }
}

VectorVisualizer::VectorVisualizer(PixelDrawer *drawer) : drawer {drawer} {}

void VectorVisualizer::generate(std::vector<Vector> vectors, float scaling_factor) {
    for (auto& v : vectors) {
        Color c = Color {
            .r = uint8_t(abs(int(v.x * scaling_factor)) % 256),
            .g = uint8_t(abs(int(v.y * scaling_factor)) % 256),
            .b = uint8_t(abs(int(v.z * scaling_factor)) % 256),
        };
        drawer->draw(c);
    }
}