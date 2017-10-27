#pragma once

#include "ofMain.h"
#include "Pentagram.h"
#include "ledLayoutRenderer.h"
#include "ledSequenceRunner.h"

#include "LEDSeqs.h"

#define MIN_MODE 1
#define MAX_MODE 6

class ofApp : public ofBaseApp{
    private:
        Fadecandy fc;
    
        Pentagram pentagram;
    
        ledLayoutRenderer ledRender;
    
        ledSequenceRunner sequences;
    
        int cMode;
        int modeSetTime;
        bool scrollModes;
        bool setMode(int mode=0);
        int nextMode();
        int prevMode();

	public:
		void setup();
        void exit();
		void update();
		void draw();

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
