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
        
        void setTrigger(float* trigger) {
            this->trigger = trigger;
        }
        void setAudioData(float* pulse) {
            currentPulse = pulse;
        }
        
    protected:
        float* currentPulse = NULL;
        float* trigger = NULL;
        float lastPulse = -1;
        
        void doUpdate() {
            ofColor color;
            
            float triggerValue = *trigger;
            
            if (config.find("trigger") != config.end()) {
                triggerValue = stof(config["trigger"]);
            }
            
            if (lastPulse == -1 || abs(*currentPulse - lastPulse) >= triggerValue) {
                color.r = stof(config["R2"]);
                color.g = stof(config["G2"]);
                color.b = stof(config["B2"]);
            }
            else {
                color.r = stof(config["R"]);
                color.g = stof(config["G"]);
                color.b = stof(config["B"]);
            }
            
            lastPulse = *currentPulse;
            
            for (int i=0; i < pixels.size(); i++) {
                if (pixels[i]) {
                    pixels[i]->setColor(color);
                }
            }
        }
    };
}

#endif /* SoundPulse_h */
