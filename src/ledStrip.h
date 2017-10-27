//
//  ledStrip.h
//  halloween17
//
//  Created by Pat Long on 2017-10-27.
//

#ifndef ledStrip_h
#define ledStrip_h

#include "Fadecandy.h"
#include "ledPixel.h"

class ledStrip {
public:
    ledStrip(Fadecandy* fadecandy, int startAddress, int length);
    ~ledStrip();
    
    void setup();
    void reset();
    
    vector<ledPixel*> getPixels(string named="*");
    
    void setColor(ofColor color);
    
    struct {
        float x;
        float y;
        float width;
        float length;
        float angle;
        float spacing;
    } layout;
    void setLayout(float x=0.0, float y=0.0, float length=200.0, float angle=0.0, float spacing=0.25, float width=25.0);
    
protected:
    Fadecandy* fadecandy;
    int startAddress;
    int length;
    
   
    
    vector<ledPixel*> pixels;
};

#endif /* ledStrip_h */
