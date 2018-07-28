#pragma once

#include "ofxTeensyOcto.h"
#include "ofMain.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
		void gotMessage(ofMessage msg);

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
		int rectWidth;
		float ballpos;
		int dir;
};
