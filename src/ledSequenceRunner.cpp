//
//  ledSequenceRunner.cpp
//  halloween17
//
//  Created by Pat Long on 2017-10-27.
//

#include "ledSequenceRunner.h"

ledSequenceRunner::ledSequenceRunner() {
    init();
}
ledSequenceRunner::~ledSequenceRunner() {
}

void ledSequenceRunner::start() {
    int startTime = ofGetElapsedTimeMillis();
    for (deque<ledSequence*>::iterator it = seqs.begin() ; it != seqs.end(); ++it) {
        if (*it) {
            (*it)->start(false, startTime);
        }
    }
}
void ledSequenceRunner::stop() {
    for (deque<ledSequence*>::iterator it = seqs.begin() ; it != seqs.end(); ++it) {
        if (*it) {
            (*it)->stop();
        }
    }
}
void ledSequenceRunner::update() {
    for (deque<ledSequence*>::iterator it = seqs.begin() ; it != seqs.end(); ++it) {
        if (*it) {
            (*it)->update();
        }
    }
}

void ledSequenceRunner::init() {
    reset();
}
void ledSequenceRunner::reset() {
    clear();
}
void ledSequenceRunner::clear() {
    stop();
    
    for (deque<ledSequence*>::iterator it = seqs.begin() ; it != seqs.end(); ++it) {
        if (*it) {
            delete (*it);
        }
    }
    
    seqs.clear();
}

int ledSequenceRunner::add(ledSequence* sequence) {
    seqs.insert(seqs.end(), sequence);
}

ledSequence* ledSequenceRunner::get(string id) {
    for (deque<ledSequence*>::iterator it = seqs.begin() ; it != seqs.end(); ++it) {
        if (*it && (*it)->getID() == id) {
            return (*it);
        }
    }
    return NULL;
}
