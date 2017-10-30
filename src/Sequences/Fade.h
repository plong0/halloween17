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
        typedef struct FadeStep {
            ofColor color;
            bool blank;
            int timelength;
        } FadeStep;
        
        Fade(string id, vector<ledPixel*> pixels, map<string, string> config, bool autoStart=false) : ledSequence(id, pixels, config, autoStart){}
        ~Fade(){}
        
        string getName() {
            return "Fade";
        }
        
        bool start(bool forceRestart=false, int startTime=-1) {
            cout << "Fade:Start" << endl;
            ledSequence::start(forceRestart, startTime);
            this->cStep = 0;
            this->cStepStartTime = this->startTime;
            return isRunning();
        }
        
        void setFadeSteps(vector<FadeStep> steps) {
            this->steps = steps;
            this->cStep = 0;
            this->cStepStartTime = -1;
        }
        
    protected:
        vector<FadeStep> steps;
        int cStep;
        int cStepStartTime;
        
        FadeStep nextStep() {
            FadeStep& lastStep = steps[cStep];
            if (cStep >= steps.size() - 1) {
                cStep = 0;
            }
            else {
                cStep++;
            }
            cStepStartTime += lastStep.timelength;
            return steps[cStep];
        }
        
        int currentStepIndex() {
            int stepIndex = cStep;
            if (config.find("startStep") != config.end() && steps.size() > 0) {
                stepIndex = (cStep + stoi(config["startStep"])) % steps.size();
            }
            return stepIndex;
        }
        FadeStep getCurrentStep() {
            return steps[currentStepIndex()];
        }
        FadeStep getNextStep() {
            if (currentStepIndex() >= steps.size()-1) {
                return steps[0];
            }
            return steps[currentStepIndex() + 1];
        }
        
        void doUpdate() {
            bool setColor = false;
            ofColor color(0, 0, 0);
            
            if (cStep < steps.size()) {
                FadeStep step = getCurrentStep();
                FadeStep step2 = getNextStep();
                int stepTimeElapsed = currentTime() - cStepStartTime;
                int sanity = 1000;
                while (stepTimeElapsed >= step.timelength && sanity >= 0) {
                    nextStep();
                    step = getCurrentStep();
                    step2 = getNextStep();
                    stepTimeElapsed = currentTime() - cStepStartTime;
                    sanity--;
                }
                if (stepTimeElapsed < step.timelength) {
                    if (!step.blank) {
                        float stepPercent = (float)stepTimeElapsed / (float)step.timelength;
                        color.r = step.color.r + (float)(step2.color.r - step.color.r) * stepPercent;
                        color.g = step.color.g + (float)(step2.color.g - step.color.g) * stepPercent;
                        color.b = step.color.b + (float)(step2.color.b - step.color.b) * stepPercent;
                        setColor = true;
                    }
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
