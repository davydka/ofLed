#pragma once

#include "ofxTeensyOcto.h"
#include "ofMain.h"
#include "ofxMidi.h"

class ofApp : public ofBaseApp, public ofxMidiListener {

  public:
    void setup();
    void update();
    void draw();
    void exit();
    void gotMessage(ofMessage msg);

    // MIDI stuff
    ofxMidiIn midiIn;
    void newMidiMessage(ofxMidiMessage& eventArgs);
    void handleNote(int note);
    std::vector<ofxMidiMessage> midiMessages;
    std::size_t maxMessages = 10; // messages to keep track of

    // LED - Teensy stuff
    ofxTeensyOcto teensy;
    int stripWidth;
    int stripHeight;
    int rowHeight;
    int stripsPerPort;
    int numPorts;
    int brightness;

    // FBO stuff
    //-----------------------------
    ofFbo fbo;

    // Graphic functions
    //-----------------------------
    void star(float x, float y, float z, float w);
    vector <ofVec4f> stars;

    // VIDEO Stuff
    void handleOpen();
    ofVideoPlayer videoPlayer;
    ofPixels pix;
    ofImage img;

    // LUT Stuff
    void loadLUT(string path);
    void applyLUT(ofPixelsRef pix);

    bool doLUT;
    int dirLoadIndex;
    ofDirectory dir;
    ofDirectory dirV;
    ofPoint lutPos;
    ofPoint thumbPos;

    bool LUTloaded;
    ofVec3f lut[32][32][32];

    ofImage lutImg;
};
