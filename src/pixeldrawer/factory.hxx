#include "engine.hxx"
#include "animation.hxx"

#include <string>

using namespace std;

class PPMFactory : public DrawerFactory {
public:
    PPMFactory(string directory, int width, int height);
    engine::Drawer* get();
private:
    int counter = 0;
    string directory;
    int width, height;
};;