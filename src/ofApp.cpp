#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetWindowTitle("Halloween 2017");
    ofSetBackgroundColor(0);
    
    fc.setup("127.0.0.1", 7890);
    
    penta1.setup(&fc, 0, 16);
    penta1.setLayout(150, 100, 300);
    ledRender.addPixels(penta1.getPixels());
    
#ifdef DOUBLES_MIRROR
    penta2.setup(&fc, 128, 16);
    penta2.setLayout(550, 100, 300);
    ledRender.addPixels(penta2.getPixels());
#endif
    
    // initFrameBuffer at end of setup (after all LED pixels are added)
    fc.initFrameBuffer();

#ifdef AUDIO_DEVICE
    initAudio(AUDIO_DEVICE);
#else
    initAudio();
#endif
    
    initAudioFFT();
    
    scrollModes = (MIN_MODE != MAX_MODE);
    
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
        if ((cTime - modeSetTime) > MODE_TIMEOUT) {
            nextMode();
        }
    }
    
    updateAudio();

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
    
    if (deviceID >= 0) {
        Audio.stream.setDeviceID(deviceID);
    }
    
    Audio.bufferSize = 256;
    Audio.bufferCounter    = 0;
    Audio.smoothedVol     = 0.0;
    Audio.smoothedLeft     = 0.0;
    Audio.smoothedRight     = 0.0;
    Audio.scaledVol        = 0.0;
    Audio.scaledLeft        = 0.0;
    Audio.scaledRight        = 0.0;
    
    Audio.mono.assign(Audio.bufferSize, 0.0);
    Audio.left.assign(Audio.bufferSize, 0.0);
    Audio.right.assign(Audio.bufferSize, 0.0);
    
#if AUDIO_MONO
    Audio.stream.setup(this, 0, 1, 44100, Audio.bufferSize, 4);
#else
    Audio.stream.setup(this, 0, 2, 44100, Audio.bufferSize, 4);
#endif
}

void ofApp::initAudioFFT(){
    Audio.FFT.enabled = true;
    
    Audio.FFT.channels = new vector<float>;
    
    Audio.FFT.fft = ofxFft::create(Audio.FFT.bufferSize, OF_FFT_WINDOW_HAMMING, OF_FFT_BASIC);
    
    Audio.FFT.bins.resize(Audio.FFT.fft->getBinSize());
}

//--------------------------------------------------------------
void ofApp::updateAudio(){
    if (Audio.enabled) {
        Audio.scaledVol = ofMap(Audio.smoothedVol, 0.0, 0.17, 0.0, 1.0, true);
        Audio.scaledLeft = ofMap(Audio.smoothedLeft, 0.0, 0.17, 0.0, 1.0, true);
        Audio.scaledRight = ofMap(Audio.smoothedRight, 0.0, 0.17, 0.0, 1.0, true);
    }
        
    if (Audio.FFT.enabled) {
        Audio.FFT.fft->setSignal(&Audio.mono[0]);
        float* curFft = Audio.FFT.fft->getAmplitude();
        copy(curFft, curFft + Audio.FFT.fft->getBinSize(), Audio.FFT.bins.begin());
        normalizeAudio(Audio.FFT.bins);
        
        //for (int i=0; i < Audio.FFT.bins.size(); i++) {
        //    cout << "AUDIO #" << i << " @ " << Audio.FFT.bins[i] << endl;
        //}
        
        Audio.FFT.channels->clear();
        Audio.FFT.channels->assign(Audio.FFT.channelCount, 0.0);
        
        for (int i=0; i < Audio.FFT.bins.size(); i++) {
            float bufferDiff = Audio.FFT.bins[i];
            
            if (Audio.FFT.lastBuffer.size() > i) {
                //cout << "AUDIO #" << i << " @ " << buffer[i] << " - " << Audio.FFT.lastBuffer[i] << " => " << abs(buffer[i] - Audio.FFT.lastBuffer[i]) << endl;
                //bufferDiff = abs(Audio.FFT.bins[i] - Audio.FFT.lastBuffer[i]);
            }
            
            //float bufferValue = buffer[i];
            float bufferValue = bufferDiff;
            
            int channel = i / Audio.FFT.channelCount;
            if (channel < Audio.FFT.channels->size()) {
                int subChannel = i % Audio.FFT.channelCount;
                //cout << "AUDIO (" << i << " / " << buffer.size() << ") [" << channel << "," << subChannel << "] @ " << buffer[i] << endl;
                if (bufferValue > Audio.FFT.channels->at(channel)) {
                    (*Audio.FFT.channels)[channel] = bufferValue;
                 //   cout << "AUDIO BUFFER [" << channel << ", " << subChannel << "] => " << bufferValue << endl;
                }
                
                
                /**
                 if (buffer[i] > Audio.FFT.channels->at(channel)) {
                 (*Audio.FFT.channels)[channel] = buffer[i];
                 //(*Audio.FFT.channels)[channel] += buffer[i];
                 //(*Audio.FFT.channels)[channel] /= 2.0;
                 }*/
            }
        }
        
        //for (int i=0; i < Audio.FFT.channels->size(); i++) {
          //  cout << "AUDIO BUFFER [" << i << "] => " << Audio.FFT.channels->at(i) << endl;
        //}
        
        Audio.FFT.lastBuffer = Audio.FFT.bins;
        
    }
}

void ofApp::normalizeAudio(vector<float>& data) {
    if(Audio.useNormalization) {
        float maxValue = 0;
        for(int i = 0; i < data.size(); i++) {
            if(abs(data[i]) > maxValue) {
                maxValue = abs(data[i]);
            }
        }
        for(int i = 0; i < data.size(); i++) {
            data[i] /= maxValue;
        }
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
        Audio.mono[i]     = input[i]*0.5;
        Audio.left[i]     = Audio.mono[i];
        Audio.right[i]    = Audio.mono[i];

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
        Audio.mono[i]     = (Audio.left[i] + Audio.right[i]) / 2.0;
        
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
        config["R"] = "192";
        config["G"] = "0";
        config["B"] = "0";
        sequences.add(new LEDSeqs::Solid("1.1", penta1.getPixels("P2"), config));
        
        
#ifdef DOUBLES_MIRROR
        sequences.add(new LEDSeqs::Solid("2.1", penta2.getPixels("P1"), config));
#endif
        
        config["R"] = "192";
        config["G"] = "0";
        config["B"] = "192";
        sequences.add(new LEDSeqs::Solid("1.2", penta1.getPixels("P4"), config));
        
#ifdef DOUBLES_MIRROR
        sequences.add(new LEDSeqs::Solid("2.2", penta2.getPixels("P3"), config));
#endif
        
        config["R"] = "0";
        config["G"] = "192";
        config["B"] = "0";
        sequences.add(new LEDSeqs::Solid("1.3", penta1.getPixels("P3"), config));
        
#ifdef DOUBLES_MIRROR
        sequences.add(new LEDSeqs::Solid("2.3", penta2.getPixels("P4"), config));
#endif
        
        config["R"] = "0";
        config["G"] = "192";
        config["B"] = "192";
        sequences.add(new LEDSeqs::Solid("1.4", penta1.getPixels("P1"), config));
        
#ifdef DOUBLES_MIRROR
        sequences.add(new LEDSeqs::Solid("2.4", penta2.getPixels("P2"), config));
#endif
        
        config["R"] = "0";
        config["G"] = "0";
        config["B"] = "192";
        sequences.add(new LEDSeqs::Solid("1.5", penta1.getPixels("P5"), config));
        
#ifdef DOUBLES_MIRROR
        sequences.add(new LEDSeqs::Solid("2.5", penta2.getPixels("P5"), config));
#endif
        
        sequences.start();
        cMode = 0;
        modeSetTime = ofGetElapsedTimeMillis();
        
        return true;
    }
    else if (mode == 1) {
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
        cMode = 1;
        modeSetTime = ofGetElapsedTimeMillis();
        
        return true;
    }
    else if (mode == 2) {
        map<string, string> config;
        config["R"] = "32";
        config["G"] = "0";
        config["B"] = "0";
        sequences.add(new LEDSeqs::Solid("1.*", penta1.getPixels("*"), config));
#ifdef DOUBLES_MIRROR
        sequences.add(new LEDSeqs::Solid("2.*", penta2.getPixels("*"), config));
#endif
        
        config.clear();
        
        vector<LEDSeqs::Fade::FadeStep> steps;
        steps.push_back({
            ofColor(192, 0, 0),false,
            500
        });
        steps.push_back({
            ofColor(192, 0, 168),false,
            500
        });
        
        // fade to blue
        steps.push_back({
            ofColor(0, 0, 192),false,
            500
        });
        // hold blue for 500ms
        steps.push_back({
            ofColor(0, 0, 192),false,
            100
        });
        
        // fade to black and set transparent
        steps.push_back({
            ofColor(0, 0, 0),true,
            10
        });
        // hold transparency for 250ms
        steps.push_back({
            ofColor(0, 0, 0),true,
            250
        });
        
        // turn lights on at blue and hold for 250ms
        steps.push_back({
            ofColor(0, 0, 192),false,
            0
        });
        steps.push_back({
            ofColor(0, 0, 192),false,
            250
        });
        // fade to green
        steps.push_back({
            ofColor(0, 192, 0),false,
            250
        });
        // fade to yellow
        steps.push_back({
            ofColor(192, 192, 0),false,
            250
        });
        // fade to red and hold for 250ms
        steps.push_back({
            ofColor(192, 0, 0),false,
            250
        });
        steps.push_back({
            ofColor(192, 0, 0),false,
            100
        });
        // fade to cyan and enable transparency
        steps.push_back({
            ofColor(0, 255, 255),true,
            10
        });
        // hold transparency for 500ms
        steps.push_back({
            ofColor(0, 0, 0),true,
            500
        });
        
        // turn lights on at green and hold for 500ms
        steps.push_back({
            ofColor(0, 255, 0),false,
            0
        });
        steps.push_back({
            ofColor(0, 255, 0),false,
            500
        });
        
        // blink green 60 times at 10ms strobes
        for (int i=0; i < 60; i++) {
            steps.push_back({
                ofColor(0, 255, 0),false,
                10
            });
            steps.push_back({
                ofColor(0, 255, 0),true,
                10
            });
        }
        // finish pulsing on green
        steps.push_back({
            ofColor(0, 255, 0),false,
            500
        });
        // fade out to darker green
        steps.push_back({
            ofColor(32, 128, 32),false,
            150
        });
        // fade back to step 0
        
        LEDSeqs::Fade* newSequence;
        
        newSequence = new LEDSeqs::Fade("1.CA", penta1.getPixels("center"), config);
        newSequence->setFadeSteps(steps);
        sequences.add(newSequence);
#ifdef DOUBLES_MIRROR
        //sequences.add(new LEDSeqs::Fade("2.CA", penta2.getPixels("center"), config));
        newSequence = new LEDSeqs::Fade("2.CA", penta2.getPixels("center"), config);
        newSequence->setFadeSteps(steps);
        sequences.add(newSequence);
#endif
        
        sequences.start();
        cMode = 2;
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
