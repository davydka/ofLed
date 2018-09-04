#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
  ofBackground(0, 0, 0);                      // default background to black / LEDs off
  ofDisableAntiAliasing();                    // we need our graphics sharp for the LEDs
  ofSetVerticalSync(false);
  ofSetFrameRate(60);
  ofSetWindowPosition(0, 0);
  if(ofGetEnv("INDEX") == "0") {
    // cout << "that's it" << endl;
  }

  // SYSTEM SETTINGS
  //--------------------------------------
  stripWidth = 16;                            // pixel width of strip
  stripHeight = 128;                            // pixel height of strip
  rowHeight = 16;
  stripsPerPort = 1;                          // total number of strips per port
  numPorts = 1;                               // total number of teensy ports?
  brightness = 255;                             // LED brightness

  // setup our teensys
  teensy.setup(stripWidth, stripHeight, rowHeight, stripsPerPort, numPorts);

  /* Configure our teensy boards (portName, xOffset, yOffset, width%, height%, direction) */
  //teensy.serialConfigure("cu.usbmodem2809741", 0, 0, 100, 100, 0);
  //teensy.serialConfigure("cu.usbmodem2733511", 0, 0, 100, 100, 0);
  teensy.serialConfigure("ttyACM0", 0, 0, 100, 100, 0);
  teensy.setBrightness(brightness);


  // allocate our pixels, fbo, and texture
  fbo.allocate(stripWidth, stripHeight*stripsPerPort*numPorts, GL_RGB);

  stars.resize(80);
  for(int i=0; i < stars.size(); i++) {
    stars[i].x = ofRandom(-stripWidth, stripWidth);
    stars[i].y = ofRandom(-rowHeight, rowHeight);
    stars[i].z = ofRandom(0, stripWidth);
    stars[i].w = stars[i].z;
  }
}

//--------------------------------------------------------------
void ofApp::update(){
  for(int i=0; i < stars.size(); i++) {
    stars[i].z = stars[i].z - .5;
    stars[i].w = stars[i].z;

    if(stars[i].z < 1) {
      stars[i].x = ofRandom(-stripWidth, stripWidth);
      stars[i].y = ofRandom(-rowHeight, rowHeight);
      stars[i].z = ofRandom(0, stripWidth);
      stars[i].w = stars[i].z;
    }
  }

  teensy.update();                            // update our serial to teensy stuff
}

//--------------------------------------------------------------
void ofApp::draw(){
  fbo.begin();
  ofClear(0,0,0);                             // refreshes fbo, removes artifacts
  ofSetColor(255);

  ofPushMatrix();
  ofTranslate( stripWidth / 2.f, rowHeight / 2.f );
  for(int i=0; i < stars.size(); i++) {
    star(stars[i].x, stars[i].y, stars[i].z, stars[i].w);
  }
  ofPopMatrix();
  fbo.end();

  fbo.readToPixels(teensy.pixels1);           // send fbo pixels to teensy
  // fbo.draw(0, 0);
  teensy.draw(32,32);
}

void ofApp::star(float x, float y, float z, float w) {
  ofSetColor(255,130,0);
  ofFill();

  float sx = ofMap(x / z, 0, 1, 0, stripWidth);
  float sy = ofMap(y / z, 0, 1, 0, rowHeight);
  float r = ofMap(z, 0, ofGetWidth(), 2, 0);
  ofDrawCircle(sx, sy, r);
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){}
