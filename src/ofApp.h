#pragma once

#include "ofMain.h"
#include "Pentagram.h"
#include "ledLayoutRenderer.h"
#include "ledSequenceRunner.h"

#include "LEDSeqs.h"

#define MIN_MODE 2
#define MAX_MODE 2

#define DOUBLES_MIRROR 1

//#define AUDIO_DEVICE 3
//#define AUDIO_MONO 1

#define MODE_TIMEOUT 5000

#include "ofxFft.h"

class ofApp : public ofBaseApp{
    private:
        Fadecandy fc;
    
        Pentagram penta1;
        Pentagram penta2;
    
        ledLayoutRenderer ledRender;
    
        ledSequenceRunner sequences;
    
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
            
            struct {
                bool enabled = false;
                ofMutex soundMutex;
                
                int bufferSize = 256;
                int channelCount = 16;
                
                ofxFft* fft;
                vector<float> bins;
                
                /**
                ofxEasyFft fft;
                int bufferSize = 256;
                int channelCount = 16;
                 */
                
                vector<float>* channels;
                vector<float> lastBuffer;
            } FFT;
        } Audio;
    
        int cMode;
        int modeSetTime;
        bool scrollModes;
        bool setMode(int mode=0);
        int nextMode();
        int prevMode();
    
        void initAudio(int deviceID=-1);
        void initAudioFFT();
        void updateAudio();
        void normalizeAudio(vector<float>& data);
        bool toggleAudio();

	public:
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
