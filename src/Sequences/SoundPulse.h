//
//  SoundPulse.h
//  halloween17
//
//  Created by Pat Long on 2017-10-27.
//

#ifndef SoundPulse_h
#define SoundPulse_h

#include "ofMain.h"
#include "ledSequence.h"

namespace LEDSeqs {
    class SoundPulse : public ledSequence {
    public:
        SoundPulse(string id, vector<ledPixel*> pixels, map<string, string> config, bool autoStart=false) : ledSequence(id, pixels, config, autoStart){}
        ~SoundPulse(){}
        
        string getName() {
            return "SoundPulse";
        }
        
    protected:
        void doUpdate() {
            ofColor color;
            
            int trigger = 24;
            if (config.find("trigger") != config.end()) {
                trigger = stoi(config["trigger"]);
            }
            
            if (updateTime % trigger == 0) {
                color.r = stof(config["R2"]);
                color.g = stof(config["G2"]);
                color.b = stof(config["B2"]);
            }
            else {
                color.r = stof(config["R"]);
                color.g = stof(config["G"]);
                color.b = stof(config["B"]);
            }
            
            for (int i=0; i < pixels.size(); i++) {
                if (pixels[i]) {
                  pixels[i]->setColor(color);
                }
            }
        }
    };
}


#endif /* SoundPulse_h */
