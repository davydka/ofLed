#pragma once

#include "ofxTeensyOcto.h"
#include "ofMain.h"
#include "ofxMidi.h"

typedef struct {

  float 	x;
  float 	y;
  bool 	bBeingDragged;
  bool 	bOver;
  float 	radius;

}draggableVertex;

class ofApp : public ofBaseApp, public ofxMidiListener{

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
    ofFbo fdbk;
    ofFbo fdbkX;

    // Graphic functions
    //-----------------------------
    void star(float x, float y, float z, float w);
    vector <ofVec4f> stars;

    int nCurveVertices;
    draggableVertex curveVertices[7];
    draggableVertex bezierVertices[4];
};
