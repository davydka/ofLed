#include "ofApp.h"

string INDEX;
string MIDIPORT;
string ROT;
string FLIP;
int indexInt = 0;
int rot = 0;
int cNote = 6; // current note
int flip = 0;
float temp = 0;

float xx = 0;
float dd = 0;
float ee = 0;
float tt = 0;
float rr = 0;

float cc = 0;
bool ccb = true;
float cx = 0;
bool cxb = true;
float dc = 0;
bool dcb = true;
float dx = 0;
bool dxb = true;
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

  cc = ofRandom(0, 110);
  cx = ofRandom(0, 120);
  dc = ofRandom(0, 100);
  dx = ofRandom(0, 80);
}

//--------------------------------------------------------------
void ofApp::update(){
  temp++;

  if( cNote == 6) {
    if(ccb) {
      cc++;
    }
    if(!ccb) {
      cc--;
    }

    if(cxb) {
      cx++;
    }
    if(!cxb) {
      cx--;
    }

    if(dcb) {
      dc++;
    }
    if(!dcb) {
      dc--;
    }

    if(dxb) {
      dx++;
    }
    if(!dxb) {
      dx--;
    }

    if(cc > 110) {
      ccb = !ccb;
    }
    if(cc < 0) {
      ccb = !ccb;
    }

    if(cx > 120) {
      cxb = !cxb;
    }
    if(cx < 0) {
      cxb = !cxb;
    }

    if(dc > 100) {
      dcb = !dcb;
    }
    if(dc < 0) {
      dcb = !dcb;
    }

    if(dx > 80) {
      dxb = !dxb;
    }
    if(dx < 0) {
      dxb = !dxb;
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

    if(indexInt == 3) {
      ofSetColor(0, 0, 0);
      ofDrawRectangle(8, 3, 1, 1);
    }
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

    if(indexInt == 3) {
      ofSetColor(0, 0, 0);
      ofDrawRectangle(8, 3, 1, 1);
    }
    if(indexInt == 4) {
      ofSetColor(0, 0, 0);
      ofDrawRectangle(14, 7, 1, 1);
    }
  }
  if( cNote == 4) {
    ofSetColor(100,100,45);
    ofDrawRectangle(4,4,8,8);

    xx = ofMap(temp, 0, 1910, 0, 255);
    dd = ofMap(temp, 0, 960, 0, 8);

    ofSetRectMode(OF_RECTMODE_CENTER);
    ofSetColor(255,0,0);
    ofDrawRectangle(8,8,dd,dd);
    // ofPushMatrix();
      if(temp > 900) {
        // ofRotateZDeg(45);
        ofSetColor(0,160,100);
        // ofDrawRectangle(11,0,dd-8,dd-8);
        ofDrawRectangle(8,8,dd-8,dd-8);
      }
    // ofPopMatrix();

    ofSetRectMode(OF_RECTMODE_CORNER);

    if(indexInt == 3) {
      ofSetColor(0, 0, 0);
      ofDrawRectangle(8, 3, 1, 1);
    }
    if(indexInt == 4) {
      // ofSetColor(0, 0, 0);
      // ofDrawRectangle(14, 7, 1, 1);
    }
  }
  if( cNote == 5) {
    ofSetColor(160,20,160);
    ofDrawRectangle(4,4,8,8);

    xx = ofMap(temp, 0, 240, -4, 16);
    dd = ofMap(temp, 0, 240, 16, 0);
    ee = ofMap(temp, 0, 240, 20, -4);

    tt = ofMap(temp, 0, 240, 0, 360);
    rr = ofMap(temp, 0, 500, -4, 20);

    ofSetColor(20,20,160);
    ofDrawRectangle(xx,0,4,16);

    ofSetColor(20,160,160);
    ofDrawRectangle(0,dd,16,4);

    ofPushMatrix();
    ofTranslate(8, 8);
    ofRotateZDeg(tt);
    ofTranslate(-8, -8);
    ofSetColor(160,20,20);
    ofDrawRectangle(ee,0,4,16);
    ofPopMatrix();

    ofSetColor(160,160,20);
    ofDrawRectangle(0,rr,16,4);
  }
  if( cNote == 6) {
    // cc++;
    // xx = ofMap(temp, 0, 130, 0, 255);
    ofSetColor(dc,cx,dx);
    ofDrawRectangle(0,0,16,16);

    ofSetColor(cc,5,5);
    ofDrawRectangle(4,4,8,8);
    if(indexInt == 3) {
      ofSetColor(0, 0, 0);
      ofDrawRectangle(8, 3, 1, 1);
    }
    if(indexInt == 4) {
      ofSetColor(0, 0, 0);
      ofDrawRectangle(14, 7, 1, 1);
    }
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
