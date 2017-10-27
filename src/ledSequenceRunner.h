//
//  ledSequenceRunner.h
//  halloween17
//
//  Created by Pat Long on 2017-10-27.
//

#ifndef ledSequenceRunner_h
#define ledSequenceRunner_h

#include "ofMain.h"
#include "ledSequence.h"

class ledSequenceRunner {
public:
    ledSequenceRunner();
    ~ledSequenceRunner();
    
    void start();
    void stop();
    void update();
    
    void init();
    void clear();
    void reset();
    
    int add(ledSequence* sequence);
    ledSequence* get(string id);
    
protected:
    set<ledSequence*> seqs;

};


#endif /* ledSequenceRunner_h */
