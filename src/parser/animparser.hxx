#ifndef ANIMPARSER_HXX
#define ANIMPARSER_HXX

#include <iostream>
#include <vector>

#include "animation.hxx"

std::vector<Clip> parseAnimation(std::istream& input);

#endif // ANIMATION_HXX