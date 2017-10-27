//
//  ledPixel.cpp
//  halloween17
//
//  Created by Pat Long on 2017-10-27.
//

#include "ledPixel.h"

ledPixel::ledPixel(Fadecandy* fadecandy, int address) {
    this->fadecandy = fadecandy;
    this->address = address;
    this->setup();
}

ledPixel::~ledPixel() {
    reset();
}

void ledPixel::setup() {
    reset();
    setLayout();
}

void ledPixel::reset() {
    setColor(ofColor(0, 0, 0));
}

int ledPixel::getAddress() {
    return address;
}

ofColor& ledPixel::getColor() {
    return color;
}

void ledPixel::setColor(ofColor color) {
    this->color.r = color.r;
    this->color.g = color.g;
    this->color.b = color.b;
    
    if (fadecandy) {
        fadecandy->setPixel(getAddress(), this->color);
    }
}

void ledPixel::setLayout(float x, float y, float radius) {
    layout.x = x;
    layout.y = y;
    layout.radius = radius;
}
