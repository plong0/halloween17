//
//  ledPixel.h
//  halloween17
//
//  Created by Pat Long on 2017-10-27.
//

#ifndef ledPixel_h
#define ledPixel_h

#include "Fadecandy.h"

class ledPixel {
public:
    ledPixel(Fadecandy* fadecandy, int address);
    ~ledPixel();
    
    void setup();
    void reset();
    
    int getAddress();
    ofColor& getColor();
    
    void setColor(ofColor color);

    struct {
        float x;
        float y;
        float radius;
    } layout;
    void setLayout(float x=0.0, float y=0.0, float radius=10.0);
    
protected:
    Fadecandy* fadecandy;
    int address;
    ofColor color;
};

#endif /* ledPixel_h */
