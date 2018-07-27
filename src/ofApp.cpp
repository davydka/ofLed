#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	ofBackground(0, 0, 0);                      // default background to black / LEDs off
	ofDisableAntiAliasing();                    // we need our graphics sharp for the LEDs
	ofSetVerticalSync(false);
	ofSetFrameRate(60);

	// SYSTEM SETTINGS
	//--------------------------------------
	stripWidth = 16;                            // pixel width of strip
	stripHeight = 128;                            // pixel height of strip
	rowHeight = 16;
	stripsPerPort = 1;                          // total number of strips per port
	numPorts = 1;                               // total number of teensy ports?
	brightness = 255;                             // LED brightness

	drawModes = 0;                              // default mode

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
	ballpos+=dir*1.0f;

	updateFbo();                                // update our Fbo functions
	teensy.update();                            // update our serial to teensy stuff
}

//--------------------------------------------------------------
void ofApp::updateFbo(){
	fbo.begin();                                // begins the fbo
	ofClear(0,0,0);                             // refreshes fbo, removes artifacts

	//ofSetColor(brightness);
	ofSetColor(255);

	ofPushStyle();
	drawPong();
	ofPopStyle();

	fbo.end();                                  // closes the fbo

	fbo.readToPixels(teensy.pixels1);           // send fbo pixels to teensy
}

//--------------------------------------------------------------
void ofApp::draw(){
	teensy.draw(0,0);
	// fbo.draw(0, 0);

	ofSetColor(255);
}

void ofApp::drawPong(){
	if(ballpos > stripWidth-rectWidth) {
		ballpos = stripWidth-rectWidth;
		dir = -1;
	}
	else if(ballpos < 0) {
		ballpos = 0;
		dir = 1;
	}
	ofSetColor(255, 0, 255);
	ofDrawRectangle(ballpos,0,rectWidth,stripHeight*stripsPerPort*numPorts);
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){}
