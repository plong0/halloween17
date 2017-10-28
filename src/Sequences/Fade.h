//
//  Fade.h
//  halloween17
//
//  Created by Pat Long on 2017-10-28.
//

#ifndef Fade_h
#define Fade_h

namespace LEDSeqs {
    class Fade : public ledSequence {
    public:
        Fade(string id, vector<ledPixel*> pixels, map<string, string> config, bool autoStart=false) : ledSequence(id, pixels, config, autoStart){}
        ~Fade(){}
        
        string getName() {
            return "Fade";
        }
        
    protected:
        void doUpdate() {
            int startDelay = 0;
            if (config.find("startDelay") != config.end()) {
                startDelay = stoi(config["startDelay"]);
            }
            int fadeTime = 1;
            if (config.find("fadeTime") != config.end()) {
                fadeTime = stoi(config["fadeTime"]);
            }
            int resetDelay = 0;
            if (config.find("resetDelay") != config.end()) {
                resetDelay = stoi(config["resetDelay"]);
            }
            
            int R1 = stof(config["R1"]);
            int G1 = stof(config["G1"]);
            int B1 = stof(config["B1"]);
            
            int R2 = stof(config["R2"]);
            int G2 = stof(config["G2"]);
            int B2 = stof(config["B2"]);
            
            int frameTime = timeElapsed();
            
            ofColor color;
            bool setColor = false;
            int fadeOffset = frameTime - startDelay;
            if (fadeOffset >= 0 && fadeOffset <= fadeTime) {
                float fadePercent = (float)fadeOffset / (float)fadeTime;
                color.r = R1 + (R2 - R1) * fadePercent;
                color.g = G1 + (G2 - G1) * fadePercent;
                color.b = B1 + (B2 - B1) * fadePercent;
                setColor = true;
            }
            else if (fadeOffset > fadeTime) {
                if ((fadeOffset - fadeTime) >= resetDelay) {
                    restart();
                }
                else {
                    color.r = R2;
                    color.g = G2;
                    color.b = B2;
                    setColor = true;
                }
            }
            
            if (setColor) {
                for (int i=0; i < pixels.size(); i++) {
                    if (pixels[i]) {
                        pixels[i]->setColor(color);
                    }
                }
            }
            
        }
    };
}

#endif /* Fade_h */
