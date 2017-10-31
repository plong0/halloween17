//
//  ledStrip.cpp
//  halloween17
//
//  Created by Pat Long on 2017-10-27.
//

#include "ledStrip.h"

ledStrip::ledStrip(Fadecandy* fadecandy, int startAddress, int length) {
    this->fadecandy = fadecandy;
    this->startAddress = startAddress;
    this->length = length;
    setup();
}

ledStrip::~ledStrip() {
    reset();
}

void ledStrip::setup() {
    reset();
    for (int i=0; i < length; i++) {
        pixels.push_back(new ledPixel(fadecandy, startAddress+i));
    }
    setLayout();
}

void ledStrip::reset() {
    for (int i=0; i < pixels.size(); i++) {
        if (pixels[i]) {
            delete pixels[i];
        }
    }
    pixels.clear();
    setLayout();
}

vector<ledPixel*> ledStrip::getPixels(string named) {
    vector<ledPixel*> result = pixels;
    return result;
}

void ledStrip::setColor(ofColor color) {
    for (int i=0; i < pixels.size(); i++) {
        pixels[i]->setColor(color);
    }
}

void ledStrip::setLayout(float x, float y, float length, float angle, bool invert, float spacing, float width) {
    layout.x = x;
    layout.y = y;
    layout.width = width;
    layout.length = length;
    layout.angle = angle;
    layout.spacing = spacing;
    
    float pixelWidth = layout.width;
    float pixelPadding = 0;
    if (pixels.size()) {
        pixelWidth = layout.length / pixels.size();
        if (layout.width < pixelWidth) {
            pixelWidth = layout.width;
        }
        pixelPadding = pixelWidth * layout.spacing;
    }
    float pixelRadius = (pixelWidth - pixelPadding) / 2.0;
    
    float theta = (angle * M_PI) / 180.0;
    float cosTheta = cos(theta);
    float sinTheta = sin(theta);
    
    float cX = x + (cosTheta * pixelWidth/2.0);
    float cY = y + (sinTheta * pixelWidth/2.0);
    
    for (int i=0; i < pixels.size(); i++) {
        int index = invert ? (pixels.size()-1-i) : i;
        if (pixels[index]) {
            pixels[index]->setLayout(cX, cY, pixelRadius);
        }
        cX += cosTheta * pixelWidth;
        cY += sinTheta * pixelWidth;
    }
}
