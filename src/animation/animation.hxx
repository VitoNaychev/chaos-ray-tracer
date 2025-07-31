#ifndef ANIMATION_HXX
#define ANIMATION_HXX

#include "engine.hxx"

#include <vector>

using namespace std;

struct Clip {
    MovementType movement;
    float units;

    int framesCount;
};


class DrawerFactory {
public:
    virtual ~DrawerFactory() = default;
    virtual engine::Drawer* get() = 0;
};

class Animator {
public:
    Animator(engine::Engine* engine, DrawerFactory* drawerFactory);

    void animate(Scene scene, vector<Clip> animation);
private: 
    engine::Engine* engine;
    DrawerFactory* drawerFactory;
};

#endif // ANIMATION_HXX