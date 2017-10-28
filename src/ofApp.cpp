#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetWindowTitle("Halloween 2017");
    ofSetBackgroundColor(0);
    
    fc.setup("127.0.0.1", 7890);
    
    penta1.setup(&fc, 0, 16);
    penta1.setLayout(100, 100, 300);
    ledRender.addPixels(penta1.getPixels());
    
#ifdef DOUBLES_MIRROR
    penta2.setup(&fc, 128, 16);
    penta2.setLayout(500, 100, 300);
    ledRender.addPixels(penta2.getPixels());
#endif
    
    // initFrameBuffer at end of setup (after all LED pixels are added)
    fc.initFrameBuffer();

#ifdef AUDIO_DEVICE
    initAudio(AUDIO_DEVICE);
#else
    initAudio();
#endif
    
    scrollModes = true;
    
    setMode(MIN_MODE);
}

//--------------------------------------------------------------
void ofApp::exit(){
    ledRender.dropPixels(penta1.getPixels());
    penta1.reset();
    
#ifdef DOUBLES_MIRROR
    ledRender.dropPixels(penta2.getPixels());
    penta2.reset();
#endif
    
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
void ofApp::initAudio(int deviceID){
    Audio.enabled = true;
    Audio.stream.printDeviceList();
    
    //if you want to set a different device id
    //soundStream.setDeviceID(0); //bear in mind the device id corresponds to all audio devices, including  input-only and output-only devices.
    
    int bufferSize = 256;
    
    Audio.left.assign(bufferSize, 0.0);
    Audio.right.assign(bufferSize, 0.0);
    
    Audio.bufferCounter    = 0;
    Audio.smoothedVol     = 0.0;
    Audio.smoothedLeft     = 0.0;
    Audio.smoothedRight     = 0.0;
    Audio.scaledVol        = 0.0;
    Audio.scaledLeft        = 0.0;
    Audio.scaledRight        = 0.0;
    
    if (deviceID >= 0) {
        Audio.stream.setDeviceID(deviceID);
    }
#if AUDIO_MONO
    Audio.stream.setup(this, 0, 1, 44100, bufferSize, 4);
#else
    Audio.stream.setup(this, 0, 2, 44100, bufferSize, 4);
#endif
}

//--------------------------------------------------------------
void ofApp::updateAudio(){
    if (Audio.enabled) {
        Audio.scaledVol = ofMap(Audio.smoothedVol, 0.0, 0.17, 0.0, 1.0, true);
        Audio.scaledLeft = ofMap(Audio.smoothedLeft, 0.0, 0.17, 0.0, 1.0, true);
        Audio.scaledRight = ofMap(Audio.smoothedRight, 0.0, 0.17, 0.0, 1.0, true);
    }
}

//--------------------------------------------------------------
void ofApp::audioIn(float * input, int bufferSize, int nChannels){
    float curVol = 0.0;
    float curLeft = 0.0;
    float curRight = 0.0;
    
    // samples are "interleaved"
    int numCounted = 0;
    
    //lets go through each sample and calculate the root mean square which is a rough way to calculate volume
#if AUDIO_MONO
    for (int i = 0; i < bufferSize; i++){
        Audio.left[i]     = input[i]*0.5;
        Audio.right[i]    = input[i]*0.5;
        
        curLeft += Audio.left[i] * Audio.left[i];
        curRight += Audio.right[i] * Audio.right[i];
        
        curVol += Audio.left[i] * Audio.left[i];
        curVol += Audio.right[i] * Audio.right[i];
        numCounted+=2;
    }
#else
    for (int i = 0; i < bufferSize; i++){
        Audio.left[i]     = input[i*2]*0.5;
        Audio.right[i]    = input[i*2+1]*0.5;
        
        curLeft += Audio.left[i] * Audio.left[i];
        curRight += Audio.right[i] * Audio.right[i];
        
        curVol += Audio.left[i] * Audio.left[i];
        curVol += Audio.right[i] * Audio.right[i];
        numCounted+=2;
    }
#endif
    
    //this is how we get the mean of rms :)
    curVol /= (float)numCounted;
    curLeft /= (float)numCounted/2;
    curRight /= (float)numCounted/2;
    
    // this is how we get the root of rms :)
    curVol = sqrt( curVol );
    curLeft = sqrt( curLeft );
    curRight = sqrt( curRight );
    
    Audio.smoothedVol *= 0.93;
    Audio.smoothedVol += 0.07 * curVol;
    
    Audio.smoothedLeft *= 0.93;
    Audio.smoothedLeft += 0.07 * curLeft;
    
    Audio.smoothedRight *= 0.93;
    Audio.smoothedRight += 0.07 * curRight;
    
    Audio.bufferCounter++;
}

//--------------------------------------------------------------
bool ofApp::setMode(int mode) {
    if (mode == 0) {
        sequences.reset();
        
        map<string, string> config;
        config["R"] = "255";
        config["G"] = "0";
        config["B"] = "0";

        sequences.add(new LEDSeqs::Solid("1.1", penta1.getPixels("1"), config));
        sequences.add(new LEDSeqs::Solid("1.2", penta1.getPixels("2"), config));
        
#ifdef DOUBLES_MIRROR
        sequences.add(new LEDSeqs::Solid("2.1", penta2.getPixels("1"), config));
        sequences.add(new LEDSeqs::Solid("2.2", penta2.getPixels("2"), config));
#endif
        
        config["R"] = "255";
        config["G"] = "255";
        config["B"] = "0";
        sequences.add(new LEDSeqs::Solid("1.3", penta1.getPixels("3"), config));
        
#ifdef DOUBLES_MIRROR
        sequences.add(new LEDSeqs::Solid("2.3", penta2.getPixels("3"), config));
#endif
        
        config["R"] = "0";
        config["G"] = "255";
        config["B"] = "255";
        sequences.add(new LEDSeqs::Solid("1.4", penta1.getPixels("4"), config));
        sequences.add(new LEDSeqs::Solid("1.5", penta1.getPixels("5"), config));
        
#ifdef DOUBLES_MIRROR
        sequences.add(new LEDSeqs::Solid("2.4", penta2.getPixels("4"), config));
        sequences.add(new LEDSeqs::Solid("2.5", penta2.getPixels("5"), config));
#endif
        
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
        
        sequences.add(new LEDSeqs::Solid("1.*", penta1.getPixels("all"), config));
        
#ifdef DOUBLES_MIRROR
        sequences.add(new LEDSeqs::Solid("2.*", penta2.getPixels("all"), config));
#endif
        
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
        
        sequences.add(new LEDSeqs::Solid("1.*", penta1.getPixels("all"), config));
        
#ifdef DOUBLES_MIRROR
        sequences.add(new LEDSeqs::Solid("2.*", penta2.getPixels("all"), config));
#endif
        
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
        sequences.add(new LEDSeqs::Solid("1.O", penta1.getPixels("outline"), config));
        
#ifdef DOUBLES_MIRROR
        sequences.add(new LEDSeqs::Solid("2.O", penta2.getPixels("outline"), config));
#endif
        
        config["R"] = "0";
        config["G"] = "0";
        config["B"] = "192";
        sequences.add(new LEDSeqs::Solid("1.C", penta1.getPixels("center"), config));
        
#ifdef DOUBLES_MIRROR
        sequences.add(new LEDSeqs::Solid("2.C", penta2.getPixels("center"), config));
#endif

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
        sequences.add(new LEDSeqs::Solid("1.1", penta1.getPixels("P1"), config));
        
#ifdef DOUBLES_MIRROR
        sequences.add(new LEDSeqs::Solid("2.1", penta2.getPixels("P1"), config));
#endif
        
        config["R"] = "192";
        config["G"] = "0";
        config["B"] = "192";
        sequences.add(new LEDSeqs::Solid("1.2", penta1.getPixels("P2"), config));
        
#ifdef DOUBLES_MIRROR
        sequences.add(new LEDSeqs::Solid("2.2", penta2.getPixels("P2"), config));
#endif
        
        config["R"] = "0";
        config["G"] = "192";
        config["B"] = "0";
        sequences.add(new LEDSeqs::Solid("1.3", penta1.getPixels("P3"), config));
        
#ifdef DOUBLES_MIRROR
        sequences.add(new LEDSeqs::Solid("2.3", penta2.getPixels("P3"), config));
#endif
        
        config["R"] = "0";
        config["G"] = "192";
        config["B"] = "192";
        sequences.add(new LEDSeqs::Solid("1.4", penta1.getPixels("P4"), config));
        
#ifdef DOUBLES_MIRROR
        sequences.add(new LEDSeqs::Solid("2.4", penta2.getPixels("P4"), config));
#endif
        
        config["R"] = "0";
        config["G"] = "0";
        config["B"] = "192";
        sequences.add(new LEDSeqs::Solid("1.5", penta1.getPixels("P5"), config));
        
#ifdef DOUBLES_MIRROR
        sequences.add(new LEDSeqs::Solid("2.5", penta2.getPixels("P5"), config));
#endif
        
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
        sequences.add(new LEDSeqs::Solid("1.O", penta1.getPixels("outline"), config));
        
#ifdef DOUBLES_MIRROR
        sequences.add(new LEDSeqs::Solid("2.O", penta2.getPixels("outline"), config));
#endif
        
        config["R"] = "192";
        config["G"] = "0";
        config["B"] = "0";
        sequences.add(new LEDSeqs::Solid("1.C", penta1.getPixels("center"), config));
        
#ifdef DOUBLES_MIRROR
        sequences.add(new LEDSeqs::Solid("2.C", penta2.getPixels("center"), config));
#endif
        
        sequences.start();
        cMode = 5;
        modeSetTime = ofGetElapsedTimeMillis();
        
        return true;
    }
    else if(mode == 6) {
        sequences.reset();
        
        LEDSeqs::SoundPulse* newSequence;
        map<string, string> config;
        /**
        config["R"] = "0";
        config["G"] = "0";
        config["B"] = "192";
        
        config["R2"] = "192";
        config["G2"] = "0";
        config["B2"] = "0";
         */
        
        config["R"] = "0";
        config["G"] = "192";
        config["B"] = "0";
        
        config["R2"] = "128";
        config["G2"] = "0";
        config["B2"] = "192";
        
        config["trigger"] = "0.03";
        
#ifdef DOUBLES_MIRROR
        newSequence = new LEDSeqs::SoundPulse("1.O", penta1.getPixels("outline"), config);
        newSequence->setAudioData(&Audio.smoothedLeft);
        sequences.add(newSequence);
        
        newSequence = new LEDSeqs::SoundPulse("2.O", penta2.getPixels("outline"), config);
        newSequence->setAudioData(&Audio.smoothedRight);
        sequences.add(newSequence);
#else
        newSequence = new LEDSeqs::SoundPulse("1.O", penta1.getPixels("outline"), config);
        newSequence->setAudioData(&Audio.smoothedVol);
        sequences.add(newSequence);
#endif
        
        //config["trigger"] = "0.25";
        /**
        config["R"] = "192";
        config["G"] = "0";
        config["B"] = "0";
        
        config["R2"] = "0";
        config["G2"] = "0";
        config["B2"] = "192";
         */
        config["R"] = "128";
        config["G"] = "0";
        config["B"] = "192";
        
        config["R2"] = "0";
        config["G2"] = "192";
        config["B2"] = "0";
        
#ifdef DOUBLES_MIRROR
        newSequence = new LEDSeqs::SoundPulse("1.C", penta1.getPixels("center"), config);
        newSequence->setAudioData(&Audio.smoothedLeft);
        sequences.add(newSequence);
        
        newSequence = new LEDSeqs::SoundPulse("2.C", penta2.getPixels("center"), config);
        newSequence->setAudioData(&Audio.smoothedRight);
        sequences.add(newSequence);
#else
        newSequence = new LEDSeqs::SoundPulse("1.C", penta1.getPixels("center"), config);
        newSequence->setAudioData(&Audio.smoothedVol);
        sequences.add(newSequence);
#endif
        
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
