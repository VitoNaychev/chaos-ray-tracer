#include <fstream>
#include <cstdint>

#include "ppm.hpp"
#include "engine.hpp"

using namespace std;

PPMColor toPPMColor(Color c) {
    return PPMColor {
        .r = static_cast<uint8_t>(std::min(1.0f, c.r) * 255),
        .g = static_cast<uint8_t>(std::min(1.0f, c.g) * 255),
        .b = static_cast<uint8_t>(std::min(1.0f, c.b) * 255),
    };
}
    
const int uint8_max = 255;

PPMDrawer::PPMDrawer(ostream& out, int width, int height) : out {out}, width {width}, height {height} {
    writeHeader(out, width, height);
}

PPMDrawer::~PPMDrawer() {
}

void PPMDrawer::draw(int x, int y, Color c) {
    out << to_string(c.r) << " " << to_string(c.g) << " " << to_string(c.b) << " ";
}

void PPMDrawer::writeHeader(ostream& out, int width, int height) {
    out << "P3" << endl;
    out << to_string(width) << " " << to_string(height) << endl;
    out << to_string(UINT8_MAX) << endl;
}
