//
//  ledLayoutRenderer.cpp
//  halloween17
//
//  Created by Pat Long on 2017-10-27.
//

#include "ledLayoutRenderer.h"

ledLayoutRenderer::ledLayoutRenderer() {
    setup();
}
ledLayoutRenderer::~ledLayoutRenderer() {
    reset();
}

void ledLayoutRenderer::setup() {
    reset();
}
void ledLayoutRenderer::reset() {
    pixels.clear();
}

void ledLayoutRenderer::addPixels(vector<ledPixel*> pixels) {
    for (int i=0; i < pixels.size(); i++) {
        addPixel(pixels[i]);
    }
}
void ledLayoutRenderer::addPixel(ledPixel* pixel) {
    if (pixel) {
        pixels.insert(pixel);
    }
}

void ledLayoutRenderer::dropPixels(vector<ledPixel*> pixels) {
    for (int i=0; i < pixels.size(); i++) {
        dropPixel(pixels[i]);
    }
}
void ledLayoutRenderer::dropPixel(ledPixel* pixel) {
    if (pixel) {
        pixels.erase(pixel);
    }
}

void ledLayoutRenderer::draw() {
    std::set<ledPixel*>::iterator it;
    for (it = pixels.begin(); it != pixels.end(); ++it) {
        ledPixel* pixel = *it;
        
        if (pixel) {
            ofPath path;
            path.setFillColor(pixel->getColor());
            path.circle(pixel->layout.x, pixel->layout.y, pixel->layout.radius);
            path.draw();
        }
    }
}
