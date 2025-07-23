#include <catch2/catch_test_macros.hpp>
#include <cmath>
#include <numbers>

#include "interx.hxx"
#include "enginenew.hxx"
#include "types.hxx"
#include "scene.hxx"

using namespace std;

// class MockDrawer : public Drawer {
// public:
//     MockDrawer(){}
//     ~MockDrawer(){}

//     void draw(int x, int y, const Color& c) {
//         assert(x <= 1 && x >=0 );
//         assert(y <= 1 && y >= 0);
//         canvas[x][y] = c;
//     }  

//     Color canvas[2][2];
// };


// TEST_CASE("calls pixel renderer to render a 2x2 canvas") {
//     Color wantCanvas[2][2] {
//         {{1, 0, 0}, {0, 1, 0}},
//         {{0, 0, 1}, {1, 1, 1}}};
    
    
//     auto pixelRenderer {[wantCanvas](int x, int y) {
//         return wantCanvas[x][y];
//     }};
//     EngineNew engine {pixelRenderer};

//     Scene scene {
//         .settings = {
//             .width = 2,
//             .height = 2,
//         },
//         .camera = Camera({0, 0, 0}, Matrix::identity()),
//     };
//     MockDrawer drawer{};
//     engine.render(scene, drawer);

//     std::vector<Color> want {wantCanvas[0][0], wantCanvas[0][1],
//         wantCanvas[1][0], wantCanvas[1][1]};
//     std::vector<Color> actual {drawer.canvas[0][0], drawer.canvas[0][1],
//         drawer.canvas[1][0], drawer.canvas[1][1]};
//     REQUIRE(want == actual);
// }

// TEST_CASE("renders pixel with index") {
//     Color wantColor {1, 0, 0};

//     auto shader {[wantColor]() {
//         return wantColor;
//     }};

//     Color gotColor = renderPixel(0, 0, shader);
//     REQUIRE(wantColor == gotColor);
// }

// TEST_CASE("renders pixel with another index") {
//     Scene scene {

//     }
//     Color wantColor {1, 1, 0};

//     auto shader {[wantColor]() {
//         return wantColor;
//     }};

//     Color gotColor = renderPixel(0, 1, shader);
//     REQUIRE(wantColor == gotColor);
// }

// TEST_CASE("splits rendering into two threads") {
//     // create first thread
//     // create mock drawer
//     REQUIRE(mock_drawer[0][0] == want);
//     REQUIRE(mock_drawer[0][1] == notTouched)
 
//     // create second thread
//     REQUIRE(mock_drawer[0][1] == want);
//     REQUIRE(mock_drawer[0][0] == notTouched);
// }