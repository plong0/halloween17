//
//  Fadecandy.h
//  halloween17
//
//  Created by Pat Long on 2017-10-27.
//

#ifndef Fadecandy_h
#define Fadecandy_h

#include "ofMain.h"
#include "opc_client.h"

typedef struct OPCPacket_Header
{
    uint8_t channel;
    uint8_t command;
    uint16_t data_length;
    
} OPCPacket_Header_t;

typedef struct OPCPacket
{
    OPCPacket_Header_t header;
    unsigned char data[];
} *OPCPacket_t;

class Fadecandy {
public:
    Fadecandy();
    ~Fadecandy();
    
    bool setup(string address, int port);
    
    void initFrameBuffer(int npixels);
    void initFrameBuffer();
    
    bool connect();
    void disconnect();
    bool isConnected();

    void setPixel(int address, ofColor color);
    void sendPixels();
    
protected:
    string address;
    int port;
    
    OPCClient opc;
    std::vector<uint8_t> frameBuffer;
    vector <string> error;
    
    vector<ofColor> pixels;
    bool hasChanges;
};


#endif /* Fadecandy_h */
