//
//  ledGroup.cpp
//  halloween17
//
//  Created by Pat Long on 2017-10-27.
//

#include "ledGroup.h"

ledGroup::ledGroup(string name, vector<ledPixel*> pixels) {
    this->name = name;
    setPixels(pixels);
}

ledGroup::~ledGroup() {
}

void ledGroup::setPixels(vector<ledPixel*> pixels) {
    this->pixels = pixels;
}

void ledGroup::setColor(ofColor color) {
    for (int i=0; i < pixels.size(); i++) {
        pixels[i]->setColor(color);
    }
}
