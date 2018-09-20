#include "ofApp.h"

string INDEX;
string MIDIPORT;
string ROT;
string FLIP;
int indexInt = 0;
int rot = 0;
int cNote = 100; // current note
int flip = 0;
float temp = 0;

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
  indexInt = ofToInt(INDEX);
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
  fdbk.allocate(stripWidth, stripHeight*stripsPerPort*numPorts, GL_RGB);

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
  temp++;

  if( cNote == 100) {
    for(int i=0; i < stars.size(); i++) {
      stars[i].z = stars[i].z - .22;
      stars[i].w = stars[i].z;

      if(stars[i].z < 1) {
        stars[i].x = ofRandom(-stripWidth, stripWidth);
        stars[i].y = ofRandom(-rowHeight, rowHeight);
        stars[i].z = ofRandom(0, stripWidth);
        stars[i].w = stars[i].z;
      }
    }
  }

  teensy.update();                            // update our serial to teensy stuff
}

//--------------------------------------------------------------
void ofApp::draw(){
  fbo.begin();
  ofClear(0,0,0);                             // refreshes fbo, removes artifacts
  ofSetColor(255);

  if( cNote == 0 ) {
    ofSetColor(0,60,255);
    ofDrawRectangle(0,0,16,16);

    if(indexInt == 3) {
      ofSetColor(0, 0, 0);
      ofDrawRectangle(8, 3, 1, 1);
    }
    if(indexInt == 4) {
      ofSetColor(0, 0, 0);
      ofDrawRectangle(14, 7, 1, 1);
    }
  }

  if( cNote == 100) {
    ofPushMatrix();
    ofTranslate( stripWidth / 2.f, rowHeight / 2.f );
    for(int i=0; i < stars.size(); i++) {
      star(stars[i].x, stars[i].y, stars[i].z, stars[i].w);
    }
    ofPopMatrix();
  }

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

void ofApp::star(float x, float y, float z, float w) {
  ofSetColor(0,255,255);
  ofFill();

  float sx = ofMap(x / z, 0, 1, 0, stripWidth);
  float sy = ofMap(y / z, 0, 1, 0, rowHeight);
  float r = ofMap(z, 0, ofGetWidth(), 2, 0);
  ofDrawCircle(sx, sy, r);
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){}
