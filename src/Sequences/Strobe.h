//
//  Strobe.h
//  halloween17
//
//  Created by Pat Long on 2017-10-28.
//

#ifndef Strobe_h
#define Strobe_h

#include "ofMain.h"
#include "ledSequence.h"

namespace LEDSeqs {
    class Strobe : public ledSequence {
    public:
        Strobe(string id, vector<ledPixel*> pixels, map<string, string> config, bool autoStart=false) : ledSequence(id, pixels, config, autoStart){}
        ~Strobe(){}
        
        string getName() {
            return "Strobe";
        }
        
        void setSpeed(float* speed) {
            this->speed = speed;
        }
        
    protected:
        float* speed;
        int lastPulse;
        
        void doUpdate() {
            ofColor color;
            color.r = stof(config["R"]);
            color.g = stof(config["G"]);
            color.b = stof(config["B"]);
            if (config.find("A") != config.end()) {
                color.a = stof(config["A"]);
            }
            
            float BPMs = 60.0;
            if (config.find("speed") != config.end()) {
                BPMs = stof(config["speed"]);
            }
            if (speed != NULL) {
                BPMs = *speed;
            }
            
            if (lastPulse == -1 || BPMs == 0 || abs(currentTime() - lastPulse) >= (60.0*1000.0/BPMs)) {
                color.r = stof(config["R2"]);
                color.g = stof(config["G2"]);
                color.b = stof(config["B2"]);
                if (config.find("A2") != config.end()) {
                    color.a = stof(config["A2"]);
                }
                lastPulse = currentTime();
            }
            else {
                color.r = stof(config["R"]);
                color.g = stof(config["G"]);
                color.b = stof(config["B"]);
                if (config.find("A") != config.end()) {
                    color.a = stof(config["A"]);
                }
            }
            
            for (int i=0; i < pixels.size(); i++) {
                if (pixels[i]) {
                    pixels[i]->setColor(color);
                }
            }
            
        }
    };
}


#endif /* Strobe_h */
