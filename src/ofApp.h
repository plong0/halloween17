#pragma once

#include "ofMain.h"
#include "Pentagram.h"
#include "ledLayoutRenderer.h"
#include "ledSequenceRunner.h"

#define AUDIO_ENABLED 1
//#define AUDIO_FFT_ENABLED 1

#include "LEDSeqs.h"

#define MIN_MODE 7.0
#define MAX_MODE 7.0

#define DOUBLES_MIRROR 1

#define STROBE_SPEED 600.0

#define AUDIO_DEVICE 0
#define AUDIO_MONO true

#define MODE_TIMEOUT 5000

#ifdef AUDIO_FFT_ENABLED
#include "ofxFft.h"
#endif

class ofApp : public ofBaseApp{
    private:
        Fadecandy fc;
    
        Pentagram penta1;
        Pentagram penta2;
    
        ledLayoutRenderer ledRender;
    
        ledSequenceRunner sequences;
    
        float* activeSetting = NULL;
        float* activeSettingAdjust = NULL;
        float settingAdjustDec;
        float settingAdjustOne = 1.0;
        float settingAdjustTen = 10.0;
        float settingAdjustHundred = 100.0;
    
        float cModeSetting;
    
    float minMode = 0;
    float maxMode = 0;
    float modeTimeout = 1000;
    float cMode;
    bool cycleModes;
    
    int modeSetTime;
    int modeReps;
    
    float strobeSpeed = STROBE_SPEED;

        struct {
            bool enabled = false;
            
            vector <float> mono;
            vector <float> left;
            vector <float> right;
            
            bool useNormalization;
            
            int bufferSize = 256;
            int bufferCounter;
            
            float smoothedVol;
            float smoothedLeft;
            float smoothedRight;
            
            float scaledVol;
            float scaledLeft;
            float scaledRight;
            
            ofSoundStream stream;
            
            float pulseResponse = 0.0023;
            float pulseResponseAdjust = 0.0001;
            
#ifdef AUDIO_FFT_ENABLED
            struct {
                bool enabled = false;
                ofMutex soundMutex;
                
                int bufferSize = 256;
                int channelCount = 16;
                
                ofxFft* fft;
                vector<float> bins;
                
                vector<float>* channels;
                vector<float> lastBuffer;
            } FFT;
#endif
        } Audio;

        bool setMode(float mode=0);
        float nextMode();
        float prevMode();
        bool toggleModeCycling();
        float adjustMinMode(float adj=1.0);
        float adjustMaxMode(float adj=1.0);
    
#ifdef AUDIO_ENABLED
        void initAudio(int deviceID=-1);
#ifdef AUDIO_FFT_ENABLED
        void initAudioFFT();
#endif
        void updateAudio();
        void normalizeAudio(vector<float>& data);
#endif

	public:
        vector<string> arguments;
    
		void setup();
        void exit();
		void update();
		void draw();
    
        void audioIn(float * input, int bufferSize, int nChannels);

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
};
