//
//  Fadecandy.cpp
//  halloween17
//
//  Created by Pat Long on 2017-10-27.
//

#include "Fadecandy.h"

Fadecandy::Fadecandy() {
    
}

Fadecandy::~Fadecandy() {
    
}

bool Fadecandy::setup(string address, int port) {
    this->address = address;
    this->port = port;
    
    this->hasChanges = false;
    
    try {
        // resolve server
        opc.resolve(address.c_str(), port);
        
        // connect to server
        this->connect();
    }
    catch (const std::exception& e) {
        string errMsg = "Error initializing OPC client.\n" + ofToString(e.what());
        ofLog(OF_LOG_ERROR) << errMsg;
        error.push_back(errMsg);
        ofSystemAlertDialog(errMsg);
        return false;
    }
    return true;
}

// initialize framebuffer to accomodate data for specified number of LED pixels
void Fadecandy::initFrameBuffer(int npixels) {
    // number of bytes needed for RGB color data
    const int frameBytes = npixels*3;
    
    // resize capacity
    frameBuffer.resize(sizeof(OPCClient::Header) + frameBytes);
    
    // construct header
    OPCClient::Header::view(frameBuffer).init(0, opc.SET_PIXEL_COLORS, frameBytes);
}

// count number of LEDs and initialize frame buffer
void Fadecandy::initFrameBuffer() {
    initFrameBuffer(pixels.size());
}

bool Fadecandy::connect() {
    opc.tryConnect();
    return isConnected();
}
void Fadecandy::disconnect() {
}
bool Fadecandy::isConnected() {
    return opc.isConnected();
}

void Fadecandy::setPixel(int address, ofColor color) {
    if (address >= 0) {
        if (pixels.size() <= address) {
            pixels.resize(address+1, ofColor(0,0,0));
        }
        
        //cout << "Fadecandy::setPixel @ " << address << " => (" << (int)color.r << "," << (int)color.g << "," << (int)color.b << ")" << endl;
        
        pixels[address] = color;
        hasChanges = true;
    }
}

void Fadecandy::sendPixels() {
    if (hasChanges && opc.tryConnect()) {
        int N = pixels.size();
        
        // use pointer arithmetic to populate frameBuffer data using
        //     Header struct in opc_client.
        //    (Same method as used by EffectRunner in fadecandy cpp example code)
        uint8_t *dest = OPCClient::Header::view(frameBuffer).data();
        
        // add rgb color data for each pixel, sequentially
        for (int p=0; p<N; p++)
        {
            int r = std::min<int>(255, std::max<int>(0, pixels[p].r));
            int g = std::min<int>(255, std::max<int>(0, pixels[p].g));
            int b = std::min<int>(255, std::max<int>(0, pixels[p].b));
            *(dest++) = r;
            *(dest++) = g;
            *(dest++) = b;
            
            //cout << "PIX[" << p << "] @ (" << r << "," << g << "," << b << ")" << endl;
        }
        
        // write data to device
        opc.write(frameBuffer);
        
        hasChanges = false;
    }
}
