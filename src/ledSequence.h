//
//  ledSequence.h
//  halloween17
//
//  Created by Pat Long on 2017-10-27.
//

#ifndef ledSequence_h
#define ledSequence_h

#include "ofMain.h"
#include "ledPixel.h"

class ledSequence {
public:
    ledSequence(string id, vector<ledPixel*> pixels, map<string, string> config, bool autoStart=false);
    virtual ~ledSequence();
    
    virtual string getName()=0;
    string getID();
    
    virtual bool start(bool forceRestart=false, int startTime=-1);
    virtual bool stop();
    virtual bool restart();
    void update();
    
    bool isRunning();
    
protected:
    string id;
    vector<ledPixel*> pixels;
    map<string, string> config;
    int startTime;
    int updateTime;
    int currentTime();
    int timeElapsed();
    
    virtual void doUpdate()=0;
};

#endif /* ledSequence_h */
