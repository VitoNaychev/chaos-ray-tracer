#ifndef TRANSFORMATIONS_HPP
#define TRANSFORMATIONS_HPP

#include <vector>
#include "scene.hxx"
#include "types.hxx"

class RayGenerator {
public:
    RayGenerator(int width, int height, Camera camera);
    Ray generate(int x, int y);
private:
    void centerPixel(Pixel& p);
    void normalize(Pixel& p);

    void toCV(Pixel& p);
    void adjustAspectRatio(Pixel& p);

    Vector vectorFor(const Pixel& p);

    void rotate(Vector& v);

    Camera camera;
    int width, height;
};
    

std::vector<Pixel> getPixels(int width, int height);
void getPixelCenters(std::vector<Pixel>&);

void normalize(std::vector<Pixel>& pixels, int width, int height);
void getCV(std::vector<Pixel>& pixels);
void adjustAspectRatio(std::vector<Pixel>& pixels, int width, int height);

std::vector<Vector> getVectors(std::vector<Pixel>& pixels);
void normalizeVectors(std::vector<Vector>& vectors);

std::vector<Ray> generateRays(int width, int height, Vector origin);

#endif