#include "ofApp.h"

ofIcoSpherePrimitive icoSphere;

string INDEX;
string MIDIPORT;
string ROT;
string FLIP;
int indexInt = 0;
int rot = 0;
int cNote = 5; // current note
int flip = 0;
float temp = 0;

float aa = 0;
float ss = 0;
float dd = 0;

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
  //teensy.serialConfigure("cu.usbmodem2733511", 0, 0, 100, 100, 0);
  teensy.serialConfigure("ttyACM0", 0, 0, 100, 100, flip);
  teensy.setBrightness(brightness);


  // allocate our ofxTeensyOcto pixels, fbo, and texture
  fbo.allocate(stripWidth, stripHeight*stripsPerPort*numPorts, GL_RGB);

  // ANIMATION SETTINGS
  stars.resize(80);
  for(int i=0; i < stars.size(); i++) {
    stars[i].x = ofRandom(-stripWidth, stripWidth);
    stars[i].y = ofRandom(-rowHeight, rowHeight);
    stars[i].z = ofRandom(0, stripWidth);
  }
}

//--------------------------------------------------------------
void ofApp::update(){
  if( cNote == 100) {
    for(int i=0; i < stars.size(); i++) {
      stars[i].z = stars[i].z - .1;

      if(stars[i].z < 1) {
        stars[i].x = ofRandom(-stripWidth, stripWidth);
        stars[i].y = ofRandom(-rowHeight, rowHeight);
        stars[i].z = ofRandom(0, stripWidth);
      }
    }
  }

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

  ofSetColor(255,0,15);

  if( cNote == 0) {
    for(int x=4; x<12; x++) {
      for(int y=4; y<12; y++) {
        ofSetColor(
          ofRandom(1,16),
          ofRandom(1,16),
          ofRandom(1,16)
        );
        ofDrawRectangle(x,y,1,1);
      }
    }
    dd = ofMap(temp, 0, 200, 0, 8);
    ofSetRectMode(OF_RECTMODE_CENTER);
    ofSetColor(0,64,32);
    ofDrawRectangle(8,8,dd,dd);
  }

  if( cNote == 1 || cNote == 4) {
    aa = ofMap(temp, 0, 200, 4, 0);
    ss = ofMap(temp, 0, 200, 12, 16);
    for(int x=aa; x<ss; x++) {
      for(int y=aa; y<ss; y++) {
        ofSetColor(
          ofRandom(1,4),
          ofRandom(1,32),
          ofRandom(1,64)
        );
        ofDrawRectangle(x,y,1,1);
      }
    }
    dd = 0;
    if( aa < 1 ) {
      dd = ofMap(temp, 0, 600, 0, 8);
    }
    ofSetColor(255,64,32);
    ofDrawCircle(8,8,dd);

    if( cNote == 4) {
      ofSetColor(255,64,32);
      ofDrawCircle(2,2,aa);
      ofDrawCircle(14,2,aa);
      ofDrawCircle(2,14,aa);
      ofDrawCircle(14,14,aa);
    }
  }

  if( cNote == 2 ) {
    for(int x=0; x<16; x++) {
      for(int y=0; y<16; y++) {
        ofSetColor(
          ofRandom(1,4),
          ofRandom(1,64),
          ofRandom(1,96)
        );
        ofDrawRectangle(x,y,1,1);
      }
    }

    ofSetColor(255,64,32);
    ofDrawCircle(8,8,6);

    aa = ofMap(temp, 0, 200, 4, 0);
    ss = ofMap(temp, 0, 200, 12, 16);

    dd = 0;
    if( aa < 1 ) {
      dd = ofMap(temp, 0, 400, 0, 8);
    }
    ofSetColor(12,64,128);
    ofDrawCircle(8,8,dd);
  }

  if( cNote == 3 ) {
    ofSetColor(96,192,32);
    ofDrawCircle(8,8,8);

    aa = ofMap(temp, 0, 200, 0, 16);
    ss = ofMap(temp, 0, 400, 0, 16);
    dd = ofMap(temp, 0, 600, 0, 16);

    ofSetColor(192,64,128);
    ofDrawCircle(8,8,aa);
    ofSetColor(32,64,128);
    ofDrawCircle(8,8,ss);
    ofSetColor(192,128,32);
    ofDrawCircle(8,8,dd);
  }

  if( cNote == 5 ) {
    aa = ofMap(temp, 0, 400, 0, 0.5);
    ss = ofMap(temp, 0, 400, 0, 32);
    icoSphere.setRadius( ss );
    ofSetColor(0, 128, 255);
    icoSphere.setResolution(1);
    icoSphere.setPosition(8,8,0);
    icoSphere.rotateDeg(aa, 1.0, 0.0, 0.0);
    icoSphere.draw(OF_MESH_WIREFRAME);
  }

  if( cNote != 100 ) {
     if(indexInt == 3) {
      ofSetColor(0, 0, 0);

      ofDrawRectangle(8, 3, 1, 1);

      // ofDrawRectangle(8, 2, 1, 1);
      ofDrawRectangle(9, 3, 1, 1);
    }
    if(indexInt == 4) {
      ofSetColor(0, 0, 0);
      ofDrawRectangle(14, 7, 1, 1);
    }
  }

  if( cNote == 100) {
    for(int x=4; x<12; x++) {
      for(int y=4; y<12; y++) {
        ofSetColor(
          ofRandom(1,16),
          ofRandom(1,16),
          ofRandom(1,16)
        );
        ofDrawRectangle(x,y,1,1);
      }
    }

    ofPushMatrix();
    // ofTranslate( stripWidth / 2.f, rowHeight / 2.f );
    ofTranslate( 2, 16 );
    for(int i=0; i < stars.size(); i++) {
      star(stars[i].x, stars[i].y, stars[i].z);
    }
    ofPopMatrix();
  }

  ofPopMatrix();
  fbo.end();

  fbo.readToPixels(teensy.pixels1);           // send fbo pixels to teensy
  // fbo.draw(0, 0);
  teensy.draw(32,32);
}

void ofApp::star(float x, float y, float z) {
  ofSetColor(200,130,15);
  ofFill();

  float sx = ofMap(x / z, 0, 1, 0, stripWidth);
  float sy = ofMap(y / z, 0, 1, 0, rowHeight);
  float r = ofMap(z, 0, ofGetWidth(), 2, 0);
  ofDrawCircle(sx, sy, r);
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
  cout << note << endl;
  cout << temp << endl;
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
