//
//  Pentagram.cpp
//  halloween17
//
//  Created by Pat Long on 2017-10-27.
//

#include "Pentagram.h"

Pentagram::Pentagram() {
}

Pentagram::~Pentagram() {
    reset();
}

void Pentagram::setup(Fadecandy* fadecandy, int startAddress, int ledsPerSide) {
    this->fadecandy = fadecandy;
    this->startAddress = startAddress;
    this->ledsPerSide = ledsPerSide;
    
    reset();
    int address = startAddress;
    for (int i=0; i < 5; i++) {
        strips.push_back(new ledStrip(fadecandy, address, ledsPerSide));
        stripMap.push_back(i);
        address += ledsPerSide;
        
        strips[i]->setColor(ofColor(192, 192, 0));
    }
    setLayout();
}

void Pentagram::reset() {
    setLayout();
    for (int i=0; i < strips.size(); i++) {
        if (strips[i]) {
            delete strips[i];
        }
    }
    strips.clear();
    stripMap.clear();
}

vector<ledPixel*> Pentagram::getPixels(string named, bool invert) {
    vector<ledPixel*> result;
    int offset1 = 6;
    int offset2 = 10;
    if (named == "" || named == "*" || named == "all") {
        vector<ledPixel*> temp;
        temp = getPixels("1");
        result.insert(result.end(), temp.begin(), temp.end());
        
        temp = getPixels("2");
        result.insert(result.end(), temp.begin(), temp.end());
        
        temp = getPixels("3");
        result.insert(result.end(), temp.begin(), temp.end());
        
        temp = getPixels("4");
        result.insert(result.end(), temp.begin(), temp.end());
        
        temp = getPixels("5", true);
        result.insert(result.end(), temp.begin(), temp.end());
    }
    else if (named == "1") {
        result = strips[0]->getPixels();
    }
    else if (named == "1A") {
        vector<ledPixel*> temp = strips[0]->getPixels();
        result.insert(result.begin(), temp.begin(), temp.begin()+offset1);
    }
    else if (named == "1B") {
        vector<ledPixel*> temp = strips[0]->getPixels();
        result.insert(result.begin(), temp.begin()+offset1, temp.begin()+offset2);
    }
    else if (named == "1C") {
        vector<ledPixel*> temp = strips[0]->getPixels();
        result.insert(result.begin(), temp.begin()+offset2, temp.end());
    }
    else if (named == "2") {
        result = strips[1]->getPixels();
    }
    else if (named == "2A") {
        vector<ledPixel*> temp = strips[1]->getPixels();
        result.insert(result.begin(), temp.begin(), temp.begin()+offset1);
    }
    else if (named == "2B") {
        vector<ledPixel*> temp = strips[1]->getPixels();
        result.insert(result.begin(), temp.begin()+offset1, temp.begin()+offset2);
    }
    else if (named == "2C") {
        vector<ledPixel*> temp = strips[1]->getPixels();
        result.insert(result.begin(), temp.begin()+offset2, temp.end());
    }
    else if (named == "3") {
        result = strips[2]->getPixels();
    }
    else if (named == "3A") {
        vector<ledPixel*> temp = strips[2]->getPixels();
        result.insert(result.begin(), temp.begin(), temp.begin()+offset1);
    }
    else if (named == "3B") {
        vector<ledPixel*> temp = strips[2]->getPixels();
        result.insert(result.begin(), temp.begin()+offset1, temp.begin()+offset2);
    }
    else if (named == "3C") {
        vector<ledPixel*> temp = strips[2]->getPixels();
        result.insert(result.begin(), temp.begin()+offset2, temp.end());
    }
    else if (named == "4") {
        result = strips[3]->getPixels();
    }
    else if (named == "4A") {
        vector<ledPixel*> temp = strips[3]->getPixels();
        result.insert(result.begin(), temp.begin(), temp.begin()+offset1);
    }
    else if (named == "4B") {
        vector<ledPixel*> temp = strips[3]->getPixels();
        result.insert(result.begin(), temp.begin()+offset1, temp.begin()+offset2);
    }
    else if (named == "4C") {
        vector<ledPixel*> temp = strips[3]->getPixels();
        result.insert(result.begin(), temp.begin()+offset2, temp.end());
    }
    else if (named == "5") {
        result = strips[4]->getPixels();
    }
    else if (named == "5A") {
        vector<ledPixel*> temp = strips[4]->getPixels();
        result.insert(result.begin(), temp.begin(), temp.begin()+offset1);
    }
    else if (named == "5B") {
        vector<ledPixel*> temp = strips[4]->getPixels();
        result.insert(result.begin(), temp.begin()+offset1, temp.begin()+offset2);
    }
    else if (named == "5C") {
        vector<ledPixel*> temp = strips[4]->getPixels();
        result.insert(result.begin(), temp.begin()+offset2, temp.end());
    }
    else if (named == "P1") {
        vector<ledPixel*> temp;
        temp = getPixels("4C");
        result.insert(result.end(), temp.begin(), temp.end());
        
        temp = getPixels("5C", true);
        result.insert(result.end(), temp.begin(), temp.end());
        
        temp = getPixels("2B", true);
        result.insert(result.end(), temp.begin(), temp.end());
    }
    else if (named == "P2") {
        vector<ledPixel*> temp;
        temp = getPixels("1C");
        result.insert(result.end(), temp.begin(), temp.end());
        
        temp = getPixels("2A");
        result.insert(result.end(), temp.begin(), temp.end());
        
        temp = getPixels("4B", true);
        result.insert(result.end(), temp.begin(), temp.end());
    }
    else if (named == "P3") {
        vector<ledPixel*> temp;
        temp = getPixels("3A", true);
        result.insert(result.end(), temp.begin(), temp.end());
        
        temp = getPixels("2C", true);
        result.insert(result.end(), temp.begin(), temp.end());
        
        temp = getPixels("5B", true);
        result.insert(result.end(), temp.begin(), temp.end());
    }
    else if (named == "P4") {
        vector<ledPixel*> temp;
        temp = getPixels("3C");
        result.insert(result.end(), temp.begin(), temp.end());
        
        temp = getPixels("4A");
        result.insert(result.end(), temp.begin(), temp.end());
        
        temp = getPixels("1B", true);
        result.insert(result.end(), temp.begin(), temp.end());
    }
    else if (named == "P5") {
        vector<ledPixel*> temp;
        temp = getPixels("1A", true);
        result.insert(result.end(), temp.begin(), temp.end());
        
        temp = getPixels("5A");
        result.insert(result.end(), temp.begin(), temp.end());
        
        temp = getPixels("3B");
        result.insert(result.end(), temp.begin(), temp.end());
    }
    else if (named == "C" || named == "center") {
        vector<ledPixel*> temp;
        temp = getPixels("2B");
        result.insert(result.end(), temp.begin(), temp.end());
        
        temp = getPixels("5B", true);
        result.insert(result.end(), temp.begin(), temp.end());
        
        temp = getPixels("3B");
        result.insert(result.end(), temp.begin(), temp.end());
        
        temp = getPixels("1B");
        result.insert(result.end(), temp.begin(), temp.end());
        
        temp = getPixels("4B");
        result.insert(result.end(), temp.begin(), temp.end());
    }
    else if (named == "O" || named == "outline") {
        vector<ledPixel*> temp;
        temp = getPixels("4A");
        result.insert(result.end(), temp.begin(), temp.end());
        
        temp = getPixels("1C");
        result.insert(result.end(), temp.begin(), temp.end());
        
        temp = getPixels("2A");
        result.insert(result.end(), temp.begin(), temp.end());
        
        temp = getPixels("4C");
        result.insert(result.end(), temp.begin(), temp.end());
        
        temp = getPixels("5C", true);
        result.insert(result.end(), temp.begin(), temp.end());
        
        temp = getPixels("2C");
        result.insert(result.end(), temp.begin(), temp.end());
        
        temp = getPixels("3A");
        result.insert(result.end(), temp.begin(), temp.end());
        
        temp = getPixels("5A", true);
        result.insert(result.end(), temp.begin(), temp.end());
        
        temp = getPixels("1A");
        result.insert(result.end(), temp.begin(), temp.end());
        
        temp = getPixels("3C");
        result.insert(result.end(), temp.begin(), temp.end());
    }
    
    if (invert) {
        reverse(result.begin(), result.end());
    }
    return result;
}

void Pentagram::setLayout(float x, float y, float stripLength) {
    layout.x = x;
    layout.y = y;
    layout.stripLength = stripLength;
    
    if (strips.size()) {
        for (int i=0; i < 5; i++) {
            strips[i]->setLayout(layout.x, layout.y + 50*(i + 0.5), layout.stripLength);
        }
        /**
        float stripWidth = layout.height / 5;
         */
    }
}

