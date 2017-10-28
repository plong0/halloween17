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
    this->startTime = -1;
    this->updateTime = -1;
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

bool ledSequence::start(bool forceRestart, int startTime) {
    if (isRunning() && forceRestart) {
        stop();
    }
    if (!isRunning()) {
        if (startTime < 0) {
            startTime = currentTime();
        }
        this->startTime = startTime;
        return true;
    }
    return false;
}

bool ledSequence::stop() {
    startTime = -1;
    //cout << "LEDSeq:" << getName() << " -> " << getID() << " Stopped @ " << currentTime() << endl;
}

bool ledSequence::restart() {
    return start(true);
}

void ledSequence::update() {
    if (isRunning()) {
        updateTime = currentTime();
        doUpdate();
    }
}
