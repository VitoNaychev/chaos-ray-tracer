#include "factory.hxx"

#include "engine.hxx"
#include "ppm.hpp"

#include <iostream>
#include <string>
#include <format>

using namespace std;

PPMFactory::PPMFactory(string directory, int width, int height) 
    : directory {directory}, width {width}, height {height} {}

engine::Drawer* PPMFactory::get() {
    std::string filePath = format("{}/frame{:04}.ppm", directory, counter);    

    auto outFile = new ofstream(filePath); 
    if (!outFile) {
        cerr << "Failed to open file.\n";
    }
    counter ++;

    return new PPMDrawer{outFile, width, height};
}