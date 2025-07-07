#ifndef CIRCLE_HPP
#define CIRCLE_HPP

#include <fstream>
#include "types.hpp"
#include "pixeldrawer.hpp"

using namespace std;

class CircleDrawer {
public:
    CircleDrawer(int canvasWidth, int canvasHeight, PixelDrawer& pixelDrawer);
    void draw(Pixel origin, int r, Color fill, Color bg);
private:
    int cWidth, cHeight;
    PixelDrawer& pixelDrawer;
};
    
#endif