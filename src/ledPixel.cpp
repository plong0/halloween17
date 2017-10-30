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
    if (color.a == 255) {
        this->color.r = color.r;
        this->color.g = color.g;
        this->color.b = color.b;
        this->color.a = color.a;
    }
    else {
        float a2 = (float)color.a / 255.0;
        float a1 = 1.0 - a2;
        this->color.r = (float)this->color.r*a1 + (float)color.r*a2;
        this->color.g = (float)this->color.g*a1 + (float)color.g*a2;
        this->color.b = (float)this->color.b*a1 + (float)color.b*a2;
    }
    
    if (fadecandy) {
        fadecandy->setPixel(getAddress(), this->color);
    }
}

void ledPixel::setLayout(float x, float y, float radius) {
    layout.x = x;
    layout.y = y;
    layout.radius = radius;
}
