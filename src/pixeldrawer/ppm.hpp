#ifndef PPM_HPP
#define PPM_HPP

#include <fstream>

#include "enginenew.hxx"

struct PPMColor {
    uint8_t r;
    uint8_t g;
    uint8_t b;
};

class PPMDrawer : public engine::Drawer {
public:
    PPMDrawer(std::ostream& out, int width, int height);
    ~PPMDrawer();
    void draw(int x, int y, const Color& c);
private:
    std::ostream& out;
    int width, height;

    void writeHeader(std::ostream& out, int width, int height);
};

#endif