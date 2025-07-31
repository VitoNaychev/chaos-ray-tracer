#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_exception.hpp>

#include "animation.hxx"

#include <vector>

#include "engine.hxx"
#include "scene.hxx"

using namespace std;

class MockEngine : public engine::Engine {
public:
    vector<Vector> cameraPositions;
    void renderThreaded(Scene& scene, engine::Drawer& drawer) {
        cameraPositions.push_back(scene.camera.getPosition());
    }
};

class DummyDrawer : public engine::Drawer {
public:
    void draw(int x, int y, const Color& c) {};
    void flush(){}
};

class MockDrawerFactory : public DrawerFactory {
public:
    engine::Drawer* get() {
        return new DummyDrawer;
    }
};

TEST_CASE("splits motion between frames") {
    MockEngine engine{};
    MockDrawerFactory drawerFactory{};
    Vector initialPosition {0, 0, 0};

    Camera camera {initialPosition, Matrix::identity()};
    Scene scene {
        .camera = camera,
    };

    vector<Clip> animation;
    animation.push_back(Clip {
        .movement = MovementType::Dolly,
        .units = 2,

        .framesCount = 3,
    });

    Animator animator {&engine,&drawerFactory};
    animator.animate(scene, animation);

    REQUIRE(engine.cameraPositions.size() == 3);

    Vector wantFirstPosition {0, 0, 0};
    REQUIRE(engine.cameraPositions[0] == wantFirstPosition);


    Vector wantSecondPosition {0, 0, 1};
    REQUIRE(engine.cameraPositions[1] == wantSecondPosition);

    Vector wantThirdPosition {0, 0, 2};
    REQUIRE(engine.cameraPositions[2] == wantThirdPosition);
}