//
//  Tracer.h
//  halloween17
//
//  Created by Pat Long on 2017-10-29.
//

#ifndef Tracer_h
#define Tracer_h

#include "ofMain.h"
#include "ledSequence.h"

namespace LEDSeqs {
    class Tracer : public ledSequence {
    public:
        static const int LOOP_MODE_HEAD = 0;
        static const int LOOP_MODE_TAIL = 1;
        
        Tracer(string id, vector<ledPixel*> pixels, map<string, string> config, bool autoStart=false) : ledSequence(id, pixels, config, autoStart){}
        ~Tracer(){}
        
        string getName() {
            return "Tracer";
        }
        
        bool start(bool forceRestart=false, int startTime=-1) {
            ledSequence::start(forceRestart, startTime);
            head = 0;
            lastIncTime = (float)startTime;
            return isRunning();
        }
        
        void setColors(vector<ofColor> colors) {
            this->colors = colors;
        }
        void setLength(float* length=NULL) {
            this->length = length;
        }
        void setSpeed(float* speed=NULL) {
            this->speed = speed;
        }
        
    protected:
        vector<ofColor> colors;
        float* length=NULL;
        float* speed=NULL;
        
        int head = -1;
        float lastIncTime = -1.0;
        int loopMode = LOOP_MODE_HEAD;
        
        void doUpdate() {
            float lengthValue = 0.0;
            if (config.find("length") != config.end()) {
                lengthValue = stof(config["length"]);
            }
            if (length != NULL) {
                lengthValue = *length;
            }
            
            float speedValue = 1000.0;
            if (config.find("speed") != config.end()) {
                speedValue = stof(config["speed"]);
            }
            if (speed != NULL) {
                speedValue = *speed;
            }
            
            int loopModeValue = loopMode;
            if (config.find("loopMode") != config.end()) {
                if (config["loopMode"] == "HEAD") {
                    loopMode = LOOP_MODE_HEAD;
                }
                else if (config["loopMode"] == "TAIL") {
                    loopMode = LOOP_MODE_TAIL;
                }
            }
            
            if (pixels.size()) {
                float incDelay = speedValue / pixels.size();
                
                float timeSinceInc = (float)currentTime() - lastIncTime;
                if (timeSinceInc >= incDelay) {
                    head += int(timeSinceInc / incDelay);
                    lastIncTime += incDelay;
                    if ((head-lengthValue) >= pixels.size()) {
                        head -= lengthValue;
                        head %= pixels.size();
                        if (loopModeValue == LOOP_MODE_HEAD) {
                            head += lengthValue;
                        }
                    }
                }
                
                ofColor color(0, 192, 64);
                for (int i=head; i > head-lengthValue && i >= 0; i--) {
                    int index = i;
                    if (index >= pixels.size() && loopModeValue == LOOP_MODE_HEAD) {
                        index %= pixels.size();
                    }
                    if (i == head) {
                        color.g = 255;
                    }
                    else {
                        color.g = 192;
                    }
                    if (index < pixels.size() && pixels[index]) {
                        pixels[index]->setColor(color);
                    }
                }
                
                /**
                ofColor color(0, 255, 192);
                color.r = stof(config["R"]);
                color.g = stof(config["G"]);
                color.b = stof(config["B"]);
                for (int i=0; i < pixels.size(); i++) {
                    if (pixels[i]) {
                        pixels[i]->setColor(color);
                    }
                }
                 */
            }
        }
    };
}

#endif /* Tracer_h */