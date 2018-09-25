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
    vector <ofVec2f> stars2;
};
