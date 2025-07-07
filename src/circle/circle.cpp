#include <fstream>
#include "circle.hpp"

using namespace std;


CircleDrawer::CircleDrawer(int canvasWidth, int canvasHeight, PixelDrawer& pixelDrawer): 
        cWidth {canvasWidth}, cHeight {canvasHeight}, pixelDrawer {pixelDrawer}{};

void CircleDrawer::draw(Pixel origin, int r, Color fill, Color bg) {
    for (int i = 0; i < cWidth; ++ i) {
        for (int j = 0; j < cHeight; ++ j) {
            int vDist = i - origin.x;
            int hDist = j - origin.y;
            int distance = vDist*vDist + hDist*hDist;

            if(distance <= r*r) {
                pixelDrawer.draw(fill);
            } else {
                pixelDrawer.draw(bg);
            }
        }
    }
}


