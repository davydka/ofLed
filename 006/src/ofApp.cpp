#include "ofApp.h"

string INDEX;
string MIDIPORT;
string ROT;
string FLIP;
int indexInt = 0;
int rot = 0;
int cNote = 3; // current note
int flip = 0;
float temp = 0;

float aa = 0;
bool aab = true;
float ss = 0;
bool ssb = true;
float dd = 0;
bool ddb = true;

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

  stars.resize(80);
  for(int i=0; i < stars.size(); i++) {
    stars[i].x = ofRandom(-stripWidth, stripWidth);
    stars[i].y = ofRandom(-rowHeight, rowHeight);
    stars[i].z = ofRandom(0, stripWidth);
    stars[i].w = stars[i].z;
  }

  stars2.resize(20);
  for(int i=0; i < stars2.size(); i++) {
    stars2[i].x = ofRandom(0, stripWidth);
    stars2[i].y = ofRandom(0, rowHeight);
  }

  aa = ofRandom(5, 250);
  ss = ofRandom(5, 150);
  dd = ofRandom(5, 250);
}

//--------------------------------------------------------------
void ofApp::update(){
  if( cNote != 100 ) {
    if(aa > 250) {
      aab = false;
    }
    if(aa < 5) {
      aab = true;
    }
    if(ss > 150) {
      ssb = false;
    }
    if(ss < 5) {
      ssb = true;
    }
    if(dd > 250) {
      ddb = false;
    }
    if(dd < 5) {
      ddb = true;
    }

    if( aab ) {
      aa++;
    }
    if( !aab ) {
      aa--;
    }
    if( ssb ) {
      ss++;
    }
    if( !ssb ) {
      ss--;
    }
    if( ddb ) {
      dd++;
    }
    if( !ddb ) {
      dd--;
    }
  }

  if( cNote == 100) {
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
  }

  if( cNote == 0 ) {
    for(int i=0; i < stars2.size(); i++) {
      stars2[i].x = stars2[i].x + .1;
      stars2[i].y = stars2[i].y + .5;

      if(stars2[i].y > 16) {
        stars2[i].x = ofRandom(0, 16);
        stars2[i].y = ofRandom(0, -4);
      }
    }
  }

  if( cNote == 1 ) {
    for(int i=0; i < stars2.size(); i++) {
      if( stars2[i].x < 8 ) {
        stars2[i].x = stars2[i].x - .2;
      }
      if( stars2[i].x >= 8 ) {
        stars2[i].x = stars2[i].x + .2;
      }
      stars2[i].y = stars2[i].y + .5;

      if( stars2[i].y > 16 || stars2[i].x < 0 || stars2[i].x > 16 ) {
        stars2[i].x = ofRandom(0, 16);
        stars2[i].y = ofRandom(0, -4);
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

  if( cNote == 0 ) {
    float qq = ofMap(sin(temp/118.0f), -1, 1, -4, 3);

    for(int i=0; i < stars2.size(); i++) {
      ofSetColor(108,108,108);
      ofDrawCircle(stars2[i].x, stars2[i].y, 1);
    }

    ofSetColor(32, 32, 168);
    ofDrawCircle(4, 4, 4);

    ofSetColor(0);
    ofDrawCircle(qq, 4, 4);
  }

  if( cNote == 1 ) {
    float qq = ofMap(sin(temp/118.0f), -1, 1, 0, 720);

    for(int i=0; i < stars2.size(); i++) {
      ofSetColor(108,108,108);
      ofDrawCircle(stars2[i].x, stars2[i].y, 1);
    }

    ofPushMatrix();
      ofTranslate(8, 10);
      ofRotateZDeg(qq);
      ofTranslate(-8, -10);

      ofSetColor(aa, ss, dd);
      ofDrawRectangle(4, 6, 8, 8);
    ofPopMatrix();
  }

  if( cNote == 2 ) {
    float qq = ofMap(sin(temp/118.0f), -1, 1, 0, 16);
    ofSetColor(aa, ss, dd);

    ofDrawLine(0,0,  8,qq);
    ofDrawLine(0,4,  8,qq);
    ofDrawLine(0,8,  8,qq);
    ofDrawLine(0,12, 8,qq);
    ofDrawLine(0,16, 8,qq);

    ofDrawLine(16,0,  8,qq);
    ofDrawLine(16,4,  8,qq);
    ofDrawLine(16,8,  8,qq);
    ofDrawLine(16,12, 8,qq);
    ofDrawLine(16,16, 8,qq);
  }

  if( cNote == 3 ) {
    float qq = ofMap(sin(temp/118.0f), -1, 1, 0, 360);

    float x0 = 0;
    float y0 = 8;

    float x1 = 4;
    float y1 = 8+8*sin(temp/28.0f);
    float x2 = 12;
    float y2 = 8+8*sin(temp/14.0f);

    /*
    float x1 = 4+2*cos(temp*1.0f);
    float y1 = 8+2*sin(temp/3.5f);
    float x2 = 12+2*cos(temp*2.0f);
    float y2 = 8+2*sin(temp);
    */

    /*
    float x1 = 4+2*cos(ofGetElapsedTimef()*1.0f);
    float y1 = 8+2*sin(ofGetElapsedTimef()/3.5f);
    float x2 = 12+2*cos(ofGetElapsedTimef()*2.0f);
    float y2 = 8+2*sin(ofGetElapsedTimef());
    */

    float x3 = 16;
    float y3 = 8;

    ofPushMatrix();
      ofTranslate(8, 8);
      ofRotateZDeg(qq);
      ofTranslate(-8, -8);

      ofFill();
      ofSetColor(255, 64, 32);
      ofBeginShape();
      ofVertex(x0,y0);
      ofBezierVertex(x1,y1,x2,y2,x3,y3);
      ofEndShape();
      ofDrawLine(0,8,19,8);
    ofPopMatrix();
  }

  if( cNote == 4 ) {
  }

  if( cNote == 5 ) {
  }

  if( cNote != 100) {
    if(indexInt == 3) {
      ofSetColor(0, 0, 0);

      ofDrawRectangle(8, 3, 1, 1);
      ofDrawRectangle(9, 3, 1, 1);
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

  ofPopMatrix();
  fbo.end();
  fbo.readToPixels(teensy.pixels1);           // send fbo pixels to teensy
  // fbo.draw(0, 0);
  teensy.draw(32,32);
}

void ofApp::star(float x, float y, float z, float w) {
  ofSetColor(255,0,0);
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
