//
//  SoundFreq.h
//  halloween17
//
//  Created by Pat Long on 2017-10-27.
//

#ifndef SoundFreq_h
#define SoundFreq_h

#include "ofMain.h"
#include "ledSequence.h"

namespace LEDSeqs {
    class SoundFreq : public ledSequence {
    public:
        SoundFreq(string id, vector<ledPixel*> pixels, map<string, string> config, bool autoStart=false) : ledSequence(id, pixels, config, autoStart){}
        ~SoundFreq(){}
        
        string getName() {
            return "SoundFreq";
        }
        
        void setAudioData(vector<float>* fftData, int channel=0){
            this->fftData = fftData;
            this->channel = channel;
        }
        
    protected:
        vector<float>* fftData;
        vector<float> lastFftData;
        int channel = -1;
        
        void doUpdate() {
            if (fftData == NULL || channel < 0) {
                return;
            }
            ofColor color;
            
            float trigger = 0.03;
            
            if (config.find("trigger") != config.end()) {
                trigger = stof(config["trigger"]);
            }
            
            color.r = stof(config["R"]);
            color.g = stof(config["G"]);
            color.b = stof(config["B"]);
            
            
            
            if (channel < fftData->size()) {
                float currentPulse = fftData->at(channel);
                float lastPulse = -1;
            
                if (channel < lastFftData.size()) {
                    lastPulse = lastFftData[channel];
                }
                
                if (lastPulse == -1 || abs(currentPulse - lastPulse) >= trigger) {
                    color.r = stof(config["R2"]);
                    color.g = stof(config["G2"]);
                    color.b = stof(config["B2"]);
                }
                
                /**
                if (currentPulse >= trigger) {
                    color.r = stof(config["R2"]);
                    color.g = stof(config["G2"]);
                    color.b = stof(config["B2"]);
                }
                 */
            }
            
            lastFftData = *fftData;
            
            for (int i=0; i < pixels.size(); i++) {
                if (pixels[i]) {
                    pixels[i]->setColor(color);
                }
            }
        }
    };
}

#endif /* SoundFreq_h */
