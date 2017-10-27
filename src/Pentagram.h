//
//  Pentagram.h
//  halloween17
//
//  Created by Pat Long on 2017-10-27.
//

#ifndef Pentagram_h
#define Pentagram_h

#include "Fadecandy.h"
#include "ledStrip.h"
#include "ledGroup.h"

class Pentagram {
public:
    Pentagram();
    ~Pentagram();
    
    void setup(Fadecandy* fadecandy, int startAddress=0, int ledsPerSide=16);
    void reset();
    
    vector<ledPixel*> getPixels(string named="*", bool invert=false);
    
    struct {
        float x;
        float y;
        float stripLength;
    } layout;
    void setLayout(float x=0.0, float y=0.0, float stripLength=250.0);
    
protected:
    Fadecandy* fadecandy;
    int startAddress;
    int ledsPerSide;
    vector<ledStrip*> strips;
    vector<int> stripMap;
    
    
    
};

#endif /* Pentagram_h */
