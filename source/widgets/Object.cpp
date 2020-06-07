/**
 * Rhythm Run for Nintendo 3DS
 * Lauren Kelly, 2020
 */

#include "widgets/Object.hpp"

#include <3ds.h>
#include <iostream>

namespace widgets {

Object::Object(gfxScreen_t screen, float width, float height, float center_x, float center_y, float x, float y) {
    std::cout << "Object constructor called!\n";
    this->screen = screen;
    this->width = width;
    this->height = height;
    this->center_x = center_x;
    this->center_y = center_y;
    this->x = x;
    this->y = y;
};

Object::~Object(void) {
    std::cout << "Object destructor called!";
}

float Object::getWidth(void) {
    return this->width;
}

}