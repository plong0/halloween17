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
        
        typedef struct ColorStep {
            ofColor color;
            bool blank;
            float percent;
        } ColorStep;
        
        Tracer(string id, vector<ledPixel*> pixels, map<string, string> config, bool autoStart=false) : ledSequence(id, pixels, config, autoStart){}
        ~Tracer(){}
        
        string getName() {
            return "Tracer";
        }
        
        bool start(bool forceRestart=false, int startTime=-1) {
            ledSequence::start(forceRestart, startTime);
            pixelStartTime.clear();
            head = 0;
            lastIncTime = (float)startTime;
            pixelStartTime[0] = lastIncTime;
            return isRunning();
        }
        
        void setColors(vector<ColorStep> colors) {
            this->colors = colors;
        }
        void setLength(float* length=NULL) {
            this->length = length;
        }
        void setSpeed(float* speed=NULL) {
            this->speed = speed;
        }
        
    protected:
        vector<ColorStep> colors;
        float* length=NULL;
        float* speed=NULL;
        
        int head = -1;
        float lastIncTime = -1.0;
        int loopMode = LOOP_MODE_HEAD;
        
        map<int,int> pixelStartTime;
        
        ofColor getColor(vector<ColorStep> colors, float percent) {
            ofColor result(0,0,0,0);
            if (percent >= 0.0 && percent <= 1.0) {
                float cPercent = 0.0;
                int stepIndex = 0;
                float stepPercent = 0;
                for (int i=0; i < colors.size(); i++) {
                    stepPercent = cPercent;
                    cPercent += colors[i].percent;
                    if (cPercent >= percent) {
                        stepIndex = i;
                        break;
                    }
                }
                ofColor color1 = colors[stepIndex].color;
                if ((stepIndex+1) < colors.size()) {
                    ofColor color2 = colors[stepIndex+1].color;
                    result.r = color1.r + (float)(color2.r - color1.r)*((percent-stepPercent)/colors[stepIndex].percent);
                    result.g = color1.g + (float)(color2.g - color1.g)*((percent-stepPercent)/colors[stepIndex].percent);
                    result.b = color1.b + (float)(color2.b - color1.b)*((percent-stepPercent)/colors[stepIndex].percent);
                    result.a = 255;
                }
                else {
                    result = color1;
                }
            }
            return result;
        }
        
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
                    
                    int index = head;
                    if (index >= pixels.size() && loopModeValue == LOOP_MODE_HEAD) {
                        index %= pixels.size();
                    }
                    pixelStartTime[index] = lastIncTime;
                }
                
                ofColor color(0, 192, 64);
                int cTime = currentTime();
                for (int i=0; i < pixels.size(); i++) {
                    int index = i;
                    if (index >= pixels.size() && loopModeValue == LOOP_MODE_HEAD) {
                        index %= pixels.size();
                    }
                    if (pixelStartTime.find(index) != pixelStartTime.end()) {
                        float pixelAge = (float)(cTime - pixelStartTime[index]) / (incDelay*lengthValue);
                        if (pixelAge <= 1.0) {
                            color = getColor(colors, pixelAge);
                            if (index < pixels.size() && pixels[index] && color.a > 0) {
                                pixels[index]->setColor(color);
                            }
                        }
                    }
                }
            }
        }
    };
}

#endif /* Tracer_h */
