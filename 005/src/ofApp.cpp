#include "ofApp.h"

string INDEX;
string MIDIPORT;
string ROT;
string FLIP;
int rot = 0;
int cNote = 3; // current note
int flip = 0;
float temp = 0;

float xx = 0;

//--------------------------------------------------------------
void ofApp::setup() {
  ofBackground(0, 0, 0);                      // default background to black / LEDs off
  ofDisableAntiAliasing();                    // we need our graphics sharp for the LEDs
  ofSetVerticalSync(false);
  ofSetFrameRate(60);
  ofSetWindowPosition(0, 0);

  // SYSTEM SETTINGS
  INDEX = ofGetEnv("INDEX");
  MIDIPORT = ofGetEnv("MIDIPORT");
  ROT = ofGetEnv("ROT");
  FLIP = ofGetEnv("FLIP");
  rot = ofToInt(ROT);
  flip = ofToInt(FLIP);
  cout << "pifull INDEX: " << INDEX << endl;
  cout << "MIDIPORT: " << MIDIPORT << endl;
  cout << "ROT: " << ROT << endl;
  cout << "FLIP: " << FLIP << endl;

  midiIn.listInPorts();
  if( MIDIPORT != "none") {
    for( int i = 0; i < midiIn.getNumInPorts(); i++ ) {
      if( ofIsStringInString(midiIn.getInPortName(i), MIDIPORT) ) {
        midiIn.openPort(i);
        midiIn.addListener(this);
      }
      // cout << midiIn.getInPortName(i) << endl;
    }
  }

  // Teensy SYSTEM SETTINGS
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
  teensy.serialConfigure("ttyACM0", 0, 0, 100, 100, flip);
  teensy.setBrightness(brightness);


  // allocate our pixels, fbo, and texture
  fbo.allocate(stripWidth, stripHeight*stripsPerPort*numPorts, GL_RGB);

}

//--------------------------------------------------------------
void ofApp::update(){
  temp++;

  teensy.update();                            // update our serial to teensy stuff
}

//--------------------------------------------------------------
void ofApp::draw(){
  fbo.begin();
  ofClear(0,0,0);                             // refreshes fbo, removes artifacts
  ofSetColor(255);

  ofPushMatrix();
  ofTranslate(8, 8);
  ofRotateZDeg(rot);
  ofTranslate(-8, -8);


  if( cNote == 0) {
    xx = ofMap(temp, 0, 960, 0, 360);
    ofPushMatrix();
    ofTranslate( stripWidth / 2.f, rowHeight / 2.f );
    ofRotateDeg(xx);
    ofTranslate( -stripWidth / 2.f, -rowHeight / 2.f );
    ofSetColor(160,20,45);
    ofDrawTriangle(0,16, 8,0, 16,16);
    ofPopMatrix();
  }
  if( cNote == 1) {
    xx = ofMap(temp, 0, 960, 1, 0);
    ofPushMatrix();
    ofTranslate( 8, 8 );
    ofSetColor(20,20,160);
    ofScale(xx,xx,1);
    ofDrawRectangle(-8,-8,16,16);
    ofTranslate(-8,-8);
    ofPopMatrix();
  }
  if( cNote == 2) {
    ofSetColor(20,160,45);
    ofDrawRectangle(0,0,16,16);

    xx = ofMap(temp, 0, 1910, 0, 1);
    ofPushMatrix();
    ofTranslate(8, 8);
    ofRotateZDeg(45);
    ofTranslate(-8, -8);
    ofSetColor(0,0,0);
    ofScale(xx,xx,1);
    ofDrawRectangle(2,2,12,12);
    ofPopMatrix();
  }
  if( cNote == 3) {
    xx = ofMap(temp, 0, 480, 0, 360);
    ofPushMatrix();
    ofTranslate(8, 8);
    ofRotateZDeg(xx);
    ofTranslate(-8, -8);

    ofSetColor(5,60,80);
    ofDrawRectangle(8,8,8,8);

    ofSetColor(80,60,5);
    ofDrawRectangle(0,0,8,8);

    ofSetColor(80,5,60);
    ofDrawRectangle(4,4,8,8);
    ofPopMatrix();

  }
  if( cNote == 4) {
    ofSetColor(100,100,45);
    ofDrawRectangle(4,4,8,8);
  }
  if( cNote == 5) {
    ofSetColor(160,20,160);
    ofDrawRectangle(4,4,8,8);
  }
  if( cNote == 6) {
    ofSetColor(160,160,45);
    ofDrawRectangle(4,4,8,8);
  }




  ofPopMatrix();
  fbo.end();

  fbo.readToPixels(teensy.pixels1);           // send fbo pixels to teensy
  // fbo.draw(0, 0);
  teensy.draw(32,32);
}

//--------------------------------------------------------------
void ofApp::newMidiMessage(ofxMidiMessage& msg) {
  if( msg.channel != 6 ) {
    return;
  }
  if( msg.status == MIDI_NOTE_ON && msg.velocity > 0 ) {
    handleNote(msg.pitch);
  }
  if( msg.status == MIDI_CONTROL_CHANGE ) {
    // cout << msg.control << endl;
    // cout << msg.value << endl;
  }
}

//--------------------------------------------------------------
void ofApp::handleNote(int note) {
  if( note == cNote ) {
    // return;
  }
  cNote = note;
  cout << temp << endl;
  cout << note << endl;
  temp = 0;
}

//--------------------------------------------------------------
void ofApp::exit() {
  if( MIDIPORT != "none") {
    midiIn.closePort();
    midiIn.removeListener(this);
  }
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){}
