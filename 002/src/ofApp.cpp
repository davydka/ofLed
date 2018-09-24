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
float tempX = 0;

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
  fdbk.allocate(stripWidth, stripHeight*stripsPerPort*numPorts, GL_RGB);

  stars.resize(80);
  for(int i=0; i < stars.size(); i++) {
    stars[i].x = ofRandom(-stripWidth, stripWidth);
    stars[i].y = ofRandom(-rowHeight, rowHeight);
    stars[i].z = ofRandom(0, stripWidth);
    stars[i].w = stars[i].z;
  }

  aa = ofRandom(0, 40);
  ss = ofRandom(0, 120);
  dd = ofRandom(110, 255);
}

//--------------------------------------------------------------
void ofApp::update(){
  if( cNote == 100) {
    for(int i=0; i < stars.size(); i++) {
      stars[i].z = stars[i].z - .18;
      stars[i].w = stars[i].z;

      if(stars[i].z < 1) {
        stars[i].x = ofRandom(-stripWidth, stripWidth);
        stars[i].y = ofRandom(-rowHeight, rowHeight);
        stars[i].z = ofRandom(0, stripWidth);
        stars[i].w = stars[i].z;
      }
    }
  }

  if( cNote == 2 || cNote == 3 ) {
    if(aa > 250) {
      aab = false;
    }
    if(aa < 5) {
      aab = true;
    }
    if(ss > 250) {
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

  temp++;
  tempX++;
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
    ofFill();
    ofSetPolyMode(OF_POLY_WINDING_ODD);
    
    float qq = ofMap(sin(temp/75.0f), -1, 1, 0, 16);

    ofSetColor(24, 84, 224);
    ofBeginShape();
      ofVertex(0, 0);
      ofVertex(0, qq);
      ofVertex(16, 16);
    ofEndShape(OF_CLOSE);

    ofSetColor(224, 84, 24);
    ofBeginShape();
      ofVertex(0, 0);
      ofVertex(qq, 0);
      ofVertex(16, 16);
    ofEndShape(OF_CLOSE);
    ofDrawLine(0,0,16,16);
  }

  if( cNote == 1 ) {
    ofFill();
    ofSetPolyMode(OF_POLY_WINDING_ODD);
    
    float qq = ofMap(sin(tempX/69.0f), -1, 1, 0, 16);
    float ww = ofMap(sin(tempX/138.0f), -1, 1, 1, 8);

    ofSetColor(24, 184, 224);
    ofBeginShape();
      ofVertex(0, 0);
      ofVertex(0, qq);
      ofVertex(16, 16);
    ofEndShape(OF_CLOSE);

    ofSetColor(224, 84, 24);
    ofBeginShape();
      ofVertex(0, 0);
      ofVertex(qq, 0);
      ofVertex(16, 16);
    ofEndShape(OF_CLOSE);
    ofDrawLine(0,0,16,16);

    ofSetColor(4, 4, 32);
    ofDrawCircle(8,8,ww);
  }

  if( cNote == 2) {
    ofFill();
    ofSetPolyMode(OF_POLY_WINDING_ODD);
    
    float qq = ofMap(sin(tempX/69.0f), -1, 1, 0, 16);
    float ww = ofMap(sin(tempX/138.0f), -1, 1, 1, 8);
    float ee = ofMap(temp, 0, 2211, -16, 16);
    float rr = ofMap(temp, 0, 2211, 16, -16);
    float tt = ofMap(temp, 0, 2211, 0, 360);

    ofPushMatrix();
    ofTranslate( stripWidth / 2.f, rowHeight / 2.f );
    ofRotateDeg(tt);
    ofTranslate( -stripWidth / 2.f, -rowHeight / 2.f );

    ofSetColor(24, 68, 208);
    ofBeginShape();
      ofVertex(0, 0);
      ofVertex(0, qq);
      ofVertex(16, 16);
    ofEndShape(OF_CLOSE);

    ofSetColor(208, 68, 24);
    ofBeginShape();
      ofVertex(0, 0);
      ofVertex(qq, 0);
      ofVertex(16, 16);
    ofEndShape(OF_CLOSE);
    ofDrawLine(0,0,16,16);

    ofPopMatrix();

    ofSetColor(16, 8, 48);
    ofDrawCircle(8,8,ww);

    ofSetColor(aa,ss,dd);
    ofDrawTriangle(
      0,16-ee,
      8,8-ee,
      16,16-ee
    );

    ofSetColor(dd,ss,aa);
    ofDrawTriangle(
      0,0-rr,
      8,8-rr,
      16,0-rr
    );
  }

  if( cNote == 3 ) {
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);

    float qq = ofMap(temp, 0, 2211, 0, 1440);
    float ww = ofMap(sin(tempX/138.0f), -1, 1, 1, 8);
    float ee = ofMap(temp, 0, 2211, 0, 360);
    float rr = ofMap(sin(tempX/69.0f), -1, 1, 0, 192);

    /*
    for(int x=0; x<16; x++) {
      for(int y=0; y<16; y++) {
        ofSetColor(
          ofRandom(1,8),
          ofRandom(4,12),
          ofRandom(1,32)
        );
        ofDrawRectangle(x,y,1,1);
      }
    }
    */

    ofSetColor(48, 8, 16);
    ofDrawCircle(8,8,ww);

    ofPushMatrix();
    ofTranslate( stripWidth / 2.f, rowHeight / 2.f );
    ofRotateDeg(qq);
    ofTranslate( -stripWidth / 2.f, -rowHeight / 2.f );
    for(int x=0; x<16; x=x+4) {
      for(int y=0; y<16; y=y+4) {
        ofSetColor(ss,ofRandom(16, 96),dd, rr);
        ofDrawLine(x,0,16-y,16);
        // ofDrawLine(x,0,0,16);
      }
    }
    ofPopMatrix();
  }


  if( cNote != 100 ) {
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
  ofSetColor(255,130,0);
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
  cout << temp << endl;
  cout << note << endl;
  temp = 0;
  tempX = 1105;
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
