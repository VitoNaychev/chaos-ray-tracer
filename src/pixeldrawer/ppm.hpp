#ifndef PPM_HPP
#define PPM_HPP

#include <fstream>

#include "pixeldrawer.hpp"

class PPMPixelDrawer : public PixelDrawer {
public:
    PPMPixelDrawer(std::ostream& out, int width, int height);
    ~PPMPixelDrawer();
    void draw(Color c);
private:
    std::ostream& out;
    int width, height;
private:
    void writeHeader(std::ostream& out, int width, int height);
};


#endif