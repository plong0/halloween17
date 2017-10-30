//
//  Solid.h
//  halloween17
//
//  Created by Pat Long on 2017-10-27.
//

#ifndef Solid_h
#define Solid_h

#include "ofMain.h"
#include "ledSequence.h"

namespace LEDSeqs {
    class Solid : public ledSequence {
    public:
        Solid(string id, vector<ledPixel*> pixels, map<string, string> config, bool autoStart=false) : ledSequence(id, pixels, config, autoStart){}
        ~Solid(){}
        
        string getName() {
            return "Solid";
        }
        
    protected:
        void doUpdate() {
            ofColor color;
            color.r = stof(config["R"]);
            color.g = stof(config["G"]);
            color.b = stof(config["B"]);
            if (config.find("A") != config.end()) {
                color.a = stof(config["A"]);
            }
            for (int i=0; i < pixels.size(); i++) {
                if (pixels[i]) {
                    pixels[i]->setColor(color);
                }
            }
            
        }
    };
}

#endif /* Solid_h */
