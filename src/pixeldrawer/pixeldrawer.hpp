#ifndef PIXEL_DRAWER_HPP
#define PIXEL_DRAWER_HPP

#include <fstream>

namespace pixeldrawer {

class PixelDrawer{
public:
    virtual ~PixelDrawer() {};             
    virtual void draw(Color c) = 0;
};

}


#endif