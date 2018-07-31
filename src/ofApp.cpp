#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	ofBackground(0, 0, 0);                      // default background to black / LEDs off
	ofDisableAntiAliasing();                    // we need our graphics sharp for the LEDs
	ofSetVerticalSync(true);
	ofSetFrameRate(60);

	// SYSTEM SETTINGS
	//--------------------------------------
	stripWidth = 16;                            // pixel width of strip
	stripHeight = 128;                            // pixel height of strip
	rowHeight = 16;
	stripsPerPort = 1;                          // total number of strips per port
	numPorts = 1;                               // total number of teensy ports?
	brightness = 255;                             // LED brightness

	rectWidth = 1;
	dir = 1;

	// setup our teensys
	teensy.setup(stripWidth, stripHeight, rowHeight, stripsPerPort, numPorts);

	/* Configure our teensy boards (portName, xOffset, yOffset, width%, height%, direction) */
	//teensy.serialConfigure("cu.usbmodem2809741", 0, 0, 100, 100, 0);
	//teensy.serialConfigure("cu.usbmodem2733511", 0, 0, 100, 100, 0);
	teensy.serialConfigure("ttyACM0", 0, 0, 100, 100, 0);
	teensy.setBrightness(brightness);


	// allocate our pixels, fbo, and texture
	fbo.allocate(stripWidth, stripHeight*stripsPerPort*numPorts, GL_RGB);
}

//--------------------------------------------------------------
void ofApp::update(){
	//ms= ofMap(sin(ofGetSystemTime())/1000, -1, 1, 0, stripWidth-rectWidth);
	// cout << ms << endl;

	//float h = ofToFloat(ofGetTimestampString("%H")) * 60 * 60;
	// float m = ofToFloat(ofGetTimestampString("%M")) * 60;
	float s = ofToFloat(ofGetTimestampString("%S"));
	float i = ofToFloat(ofGetTimestampString("%i"));
	float time = s + i;
	ballpos = ofMap(time, 0, 1000, 0, stripWidth-rectWidth);

	teensy.update();                            // update our serial to teensy stuff
}

//--------------------------------------------------------------
void ofApp::draw(){
	fbo.begin();
	ofClear(0,0,0);                             // refreshes fbo, removes artifacts
	ofSetColor(255, 0, 255);
	ofDrawRectangle(ballpos,0,rectWidth,stripHeight*stripsPerPort*numPorts);

	fbo.end();

	fbo.readToPixels(teensy.pixels1);           // send fbo pixels to teensy
	//fbo.draw(0, 0);
	teensy.draw(32,32);

	ofSetColor(255);
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){}
