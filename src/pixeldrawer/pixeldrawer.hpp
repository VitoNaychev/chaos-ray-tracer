#ifndef PIXEL_DRAWER_HPP
#define PIXEL_DRAWER_HPP

#include <fstream>

namespace pixeldrawer {

struct Color {
    uint8_t r;
    uint8_t g;
    uint8_t b;
};

class PixelDrawer{
public:
    virtual ~PixelDrawer() {};             
    virtual void draw(Color c) = 0;
};

}


#endif