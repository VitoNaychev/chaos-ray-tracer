#include <fstream>
#include <cstdint>

#include "ppm.hpp"
#include "pixeldrawer.hpp"

using namespace std;

namespace pixeldrawer {
    
const int uint8_max = 255;

PPMPixelDrawer::PPMPixelDrawer(ostream& out, int width, int height) : out {out}, width {width}, height {height} {
    writeHeader(out, width, height);
}

PPMPixelDrawer::~PPMPixelDrawer() {
}

void PPMPixelDrawer::draw(Color c) {
    out << to_string(c.r) << " " << to_string(c.g) << " " << to_string(c.b) << " ";
}

void PPMPixelDrawer::writeHeader(ostream& out, int width, int height) {
    out << "P3" << endl;
    out << to_string(width) << " " << to_string(height) << endl;
    out << to_string(UINT8_MAX) << endl;
}

}