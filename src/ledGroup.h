//
//  ledGroup.h
//  halloween17
//
//  Created by Pat Long on 2017-10-27.
//

#ifndef ledGroup_h
#define ledGroup_h

#include "ofMain.h"
#include "ledPixel.h"

class ledGroup {
public:
    ledGroup(string name, vector<ledPixel*> pixels);
    ~ledGroup();
    
    void setPixels(vector<ledPixel*> pixels);
    
    void setColor(ofColor color);
    
    string name;
    vector<ledPixel*> pixels;
};

#endif /* ledGroup_h */
