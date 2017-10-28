//
//  ledSequence.cpp
//  halloween17
//
//  Created by Pat Long on 2017-10-27.
//

#include "ledSequence.h"

ledSequence::ledSequence(string id, vector<ledPixel*> pixels, map<string, string> config, bool autoStart) {
    this->id = id;
    this->pixels = pixels;
    this->config = config;
    if (autoStart) {
        start();
    }
}

ledSequence::~ledSequence() {
    stop();
}

string ledSequence::getID() {
    return id;
}

int ledSequence::currentTime() {
    return ofGetElapsedTimeMillis();
}

int ledSequence::timeElapsed() {
    if (isRunning()) {
        return (currentTime() - startTime);
    }
    return 0;
}

bool ledSequence::isRunning() {
    return (startTime != -1);
}

void ledSequence::start(bool forceRestart, int startTime) {
    if (isRunning() && forceRestart) {
        stop();
    }
    if (!isRunning()) {
        if (startTime < 0) {
            startTime = currentTime();
        }
        this->startTime = startTime;
        //cout << "LEDSeq:" << getName() << " -> " << getID() << " Started @ " << startTime << endl;
    }
}

void ledSequence::stop() {
    startTime = -1;
    //cout << "LEDSeq:" << getName() << " -> " << getID() << " Stopped @ " << currentTime() << endl;
}

void ledSequence::restart() {
    start(true);
}

void ledSequence::update() {
    if (isRunning()) {
        updateTime = currentTime();
        doUpdate();
    }
}
