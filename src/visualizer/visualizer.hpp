#ifndef PIXEL_VISUALIZER_HPP
#define PIXEL_VISUALIZER_HPP

#include "types.hxx"
#include "pixeldrawer.hpp"


class PixelVisualizer{
public:
    PixelVisualizer(PixelDrawer* drawer);
    void generate(std::vector<Pixel> pixels, float scaling_factor);
private:
    PixelDrawer* drawer;
};

class VectorVisualizer{
public:
    VectorVisualizer(PixelDrawer* drawer);
    void generate(std::vector<Vector> pixels, float scaling_factor);
private:
    PixelDrawer* drawer;
};

#endif