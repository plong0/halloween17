//
//  ledLayoutRenderer.h
//  halloween17
//
//  Created by Pat Long on 2017-10-27.
//

#ifndef ledLayoutRenderer_h
#define ledLayoutRenderer_h

#include "ofMain.h"
#include "ledPixel.h"

class ledLayoutRenderer {
public:
    ledLayoutRenderer();
    ~ledLayoutRenderer();
    
    void setup();
    void reset();
    
    void addPixels(vector<ledPixel*> pixels);
    void addPixel(ledPixel* pixel);
    void dropPixels(vector<ledPixel*> pixels);
    void dropPixel(ledPixel* pixel);
    
    void draw();
    
protected:
    set<ledPixel*> pixels;
    
};

#endif /* ledLayoutRenderer_h */
