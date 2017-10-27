#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetWindowTitle("Halloween 2017");
    ofSetBackgroundColor(0);
    
    fc.setup("127.0.0.1", 7890);
    
    pentagram.setup(&fc, 0, 16);
    pentagram.setLayout(100, 100, 400);
    
    ledRender.addPixels(pentagram.getPixels());
    
    // initFrameBuffer at end of setup (after all LED pixels are added)
    fc.initFrameBuffer();
    
    scrollModes = true;
    
    setMode(MIN_MODE);
}

//--------------------------------------------------------------
void ofApp::exit(){
    ledRender.dropPixels(pentagram.getPixels());
    pentagram.reset();
    fc.disconnect();
}

//--------------------------------------------------------------
void ofApp::update(){
    if (scrollModes) {
        int cTime = ofGetElapsedTimeMillis();
        if ((cTime - modeSetTime) > 5000) {
            nextMode();
        }
    }

    sequences.update();
    
    fc.sendPixels();
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofPath border;
    border.rectangle(100, 100, 800, 400);
    border.setFilled(false);
    border.setStrokeWidth(1.0);
    border.setStrokeColor(ofColor(192, 128, 255));
    border.draw();
    
    ledRender.draw();
}

//--------------------------------------------------------------
bool ofApp::setMode(int mode) {
    if (mode == 0) {
        sequences.reset();
        
        map<string, string> config;
        config["R"] = "255";
        config["G"] = "0";
        config["B"] = "0";

        sequences.add(new LEDSeqs::Solid("1", pentagram.getPixels("1"), config));
        sequences.add(new LEDSeqs::Solid("2", pentagram.getPixels("2"), config));
        
        config["R"] = "255";
        config["G"] = "255";
        config["B"] = "0";
        sequences.add(new LEDSeqs::Solid("3", pentagram.getPixels("3"), config));
        
        config["R"] = "0";
        config["G"] = "255";
        config["B"] = "255";
        sequences.add(new LEDSeqs::Solid("4", pentagram.getPixels("4"), config));
        sequences.add(new LEDSeqs::Solid("5", pentagram.getPixels("5"), config));
        
        sequences.start();
        cMode = 0;
        modeSetTime = ofGetElapsedTimeMillis();
        
        return true;
    }
    else if (mode == 1) {
        sequences.reset();
        
        map<string, string> config;
        config["R"] = "0";
        config["G"] = "64";
        config["B"] = "192";
        
        sequences.add(new LEDSeqs::Solid("all", pentagram.getPixels("all"), config));
        
        sequences.start();
        cMode = 1;
        modeSetTime = ofGetElapsedTimeMillis();
        
        return true;
    }
    else if (mode == 2) {
        sequences.reset();
        
        map<string, string> config;
        config["R"] = "192";
        config["G"] = "64";
        config["B"] = "0";
        
        sequences.add(new LEDSeqs::Solid("all", pentagram.getPixels("all"), config));
        
        sequences.start();
        cMode = 2;
        modeSetTime = ofGetElapsedTimeMillis();
        
        return true;
    }
    else if(mode == 3) {
        sequences.reset();
        
        map<string, string> config;
        config["R"] = "192";
        config["G"] = "0";
        config["B"] = "0";
        sequences.add(new LEDSeqs::Solid("outline", pentagram.getPixels("outline"), config));
        
        config["R"] = "0";
        config["G"] = "0";
        config["B"] = "192";
        sequences.add(new LEDSeqs::Solid("center", pentagram.getPixels("center"), config));

        sequences.start();
        cMode = 3;
        modeSetTime = ofGetElapsedTimeMillis();
        
        return true;
    }
    else if(mode == 4) {
        sequences.reset();
        
        map<string, string> config;
        config["R"] = "192";
        config["G"] = "0";
        config["B"] = "0";
        sequences.add(new LEDSeqs::Solid("P1", pentagram.getPixels("P1"), config));
        
        config["R"] = "192";
        config["G"] = "0";
        config["B"] = "192";
        sequences.add(new LEDSeqs::Solid("P2", pentagram.getPixels("P2"), config));
        
        config["R"] = "0";
        config["G"] = "192";
        config["B"] = "0";
        sequences.add(new LEDSeqs::Solid("P3", pentagram.getPixels("P3"), config));
        
        config["R"] = "0";
        config["G"] = "192";
        config["B"] = "192";
        sequences.add(new LEDSeqs::Solid("P4", pentagram.getPixels("P4"), config));
        
        config["R"] = "0";
        config["G"] = "0";
        config["B"] = "192";
        sequences.add(new LEDSeqs::Solid("P5", pentagram.getPixels("P5"), config));
        
        sequences.start();
        cMode = 4;
        modeSetTime = ofGetElapsedTimeMillis();
        
        return true;
    }
    else if(mode == 5) {
        sequences.reset();
        
        map<string, string> config;
        config["R"] = "0";
        config["G"] = "0";
        config["B"] = "192";
        sequences.add(new LEDSeqs::Solid("outline", pentagram.getPixels("outline"), config));
        
        config["R"] = "192";
        config["G"] = "0";
        config["B"] = "0";
        sequences.add(new LEDSeqs::Solid("center", pentagram.getPixels("center"), config));
        
        sequences.start();
        cMode = 5;
        modeSetTime = ofGetElapsedTimeMillis();
        
        return true;
    }
    else if(mode == 6) {
        sequences.reset();
        
        map<string, string> config;
        config["R"] = "0";
        config["G"] = "0";
        config["B"] = "192";
        
        config["R2"] = "192";
        config["G2"] = "0";
        config["B2"] = "0";
        
        config["trigger"] = "25";
        sequences.add(new LEDSeqs::SoundPulse("outline", pentagram.getPixels("outline"), config));
        
        config["trigger"] = "6";
        sequences.add(new LEDSeqs::SoundPulse("center", pentagram.getPixels("center"), config));
        
        sequences.start();
        cMode = 6;
        modeSetTime = ofGetElapsedTimeMillis();
        
        return true;
    }
    return false;
}

int ofApp::nextMode() {
    if (cMode >= MAX_MODE) {
        setMode(MIN_MODE);
    }
    else {
        setMode(cMode + 1);
    }
    return cMode;
}
int ofApp::prevMode() {
    if (cMode <= MIN_MODE) {
        setMode(MAX_MODE);
    }
    else {
        setMode(cMode - 1);
    }
    return cMode;
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
