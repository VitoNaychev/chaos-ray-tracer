#ifndef TRANSFORMATIONS_HPP
#define TRANSFORMATIONS_HPP

#include <vector>
#include "types.hpp"


std::vector<Pixel> getPixels(int width, int height);
void getPixelCenters(std::vector<Pixel>&);

void normalize(std::vector<Pixel>& pixels, int width, int height);
void getCV(std::vector<Pixel>& pixels);
void adjustAspectRatio(std::vector<Pixel>& pixels, int width, int height);

std::vector<Vector> getVectors(std::vector<Pixel>& pixels);
void normalizeVectors(std::vector<Vector>& vectors);

std::vector<Ray> generateRays(int width, int height, Vector origin);

#endif