#ifndef PPM_HPP
#define PPM_HPP

#include <fstream>

#include "engine.hxx"

struct PPMColor {
    uint8_t r;
    uint8_t g;
    uint8_t b;
};

class PPMDrawer : public engine::Drawer {
public:
    PPMDrawer(std::ostream* out, int width, int height);
    ~PPMDrawer();
    
    void draw(int x, int y, const Color& c);
    void flush();
private:
    std::ostream*  out;
    int width, height;
    std::vector<PPMColor> pixels;

    void writeHeader(std::ostream* out, int width, int height);
};

#endif