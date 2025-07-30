#ifndef ANIMATION_HXX
#define ANIMATION_HXX

enum class MovementType {
    Dolly,
    Jib,
    Truck,

    Pan,
    Tilt,
    Roll,

    Arc,

    Zoom,
};

struct Clip {
    MovementType movement;
    float units;

    int framesCount;
};

class Engine {

};

class Animator {

private: 

};

#endif // ANIMATION_HXX