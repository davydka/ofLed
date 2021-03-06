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
  fdbk.allocate(stripWidth, 16, GL_RGB);
  fdbkX.allocate(stripWidth, stripHeight*stripsPerPort*numPorts, GL_RGB);
  fdbk.begin();
  ofClear(0,255);
  fdbk.end();
  fdbkX.begin();
  ofClear(0,255);
  fdbkX.end();

  stars.resize(80);
  for(int i=0; i < stars.size(); i++) {
    stars[i].x = ofRandom(-stripWidth, stripWidth);
    stars[i].y = ofRandom(-rowHeight, rowHeight);
    stars[i].z = ofRandom(0, stripWidth);
    stars[i].w = stars[i].z;
  }

  aa = ofRandom(0, 110);
  ss = ofRandom(0, 120);
  dd = ofRandom(110, 255);
}

//--------------------------------------------------------------
void ofApp::update(){
  if( cNote == 100) {
    for(int i=0; i < stars.size(); i++) {
      stars[i].z = stars[i].z - .2;
      stars[i].w = stars[i].z;

      if(stars[i].z < 1) {
        stars[i].x = ofRandom(-stripWidth, stripWidth);
        stars[i].y = ofRandom(-rowHeight, rowHeight);
        stars[i].z = ofRandom(0, stripWidth);
        stars[i].w = stars[i].z;
      }
    }
  }

  temp++;

  if( cNote == 2  || cNote == 3 || cNote == 5 || cNote == 6 ) {
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

  if( cNote == 4 ) {
    if(aa > 16) {
      aab = false;
    }
    if(aa < 0) {
      aab = true;
    }
    if(ss > 16) {
      ssb = false;
    }
    if(ss < 0) {
      ssb = true;
    }
    if(dd > 16) {
      ddb = false;
    }
    if(dd < 0) {
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

  /*
  if( cNote == 6 ) {
    float zz = ofMap(temp, 0, 700, 1, 0);
    fdbk.begin();
    // ofSetColor(255);
    fdbkX.draw(0,0,16,16);
    //  glBlendFuncSeparate (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
      ofPushMatrix();
        ofTranslate( 8, 8 );
        ofScale(zz,zz,1);
        ofTranslate(-8,-8);
        ofSetColor(ofRandom(255),ofRandom(255),255);
        ofDrawCircle(8,8,8);
      ofPopMatrix();
    fdbk.end();

    fdbkX.begin();
    // ofSetColor(255);
    fdbk.draw(0,0);
    fdbkX.end();
  }
  */

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
    ofPushMatrix();
      ofTranslate(8,8,0);
      ofSetColor(255, 100, 32);
      ofFill();
      ofSetPolyMode(OF_POLY_WINDING_ODD);
      ofBeginShape();
      float angleStep = TWO_PI/(10.0f + sin(temp/75.0f) * 60);
      float radiusAdder = 0.5f;
      float radius = 0;
      for (int i = 0; i < 20; i++){
        float anglef = (i) * angleStep;
        float x = radius * cos(anglef);
        float y = radius * sin(anglef);
        ofVertex(x,y);
        radius += radiusAdder;
      }
      ofEndShape(OF_CLOSE);
    ofPopMatrix();
  }

  if( cNote == 1 ) {
    float zz = ofMap(temp, 0, 700, 0, 1);
    ofSetColor(255, 100, 32);
    ofDrawTriangle(0,16, 8,0, 16,16);

    ofPushMatrix();
      // ofTranslate(8,8,0);
      ofSetColor(32, 100, 255);
      ofFill();
      ofSetPolyMode(OF_POLY_WINDING_ODD);
      ofBeginShape();

      for (int i = 0; i < 10; i++){
        ofVertex(ofRandom(0,16), ofRandom(0,16));
      }


      ofEndShape(OF_CLOSE);
      ofPushMatrix();
        ofTranslate( 8, 8 );
        ofScale(zz,zz,1);
        ofTranslate(-8,-8);

        ofSetColor(0, 0, 0);
        ofDrawTriangle(0,0, 8,16, 16,0);
      ofPopMatrix();

    ofPopMatrix();
  }

  if( cNote == 2 ) {
    ofSetColor(aa,ss,dd);
    ofDrawCircle(8,4,4);

    float zz = ofMap(temp, 0, 700, 10, 16);
    ofSetColor(255, 32, 100);
    ofSetPolyMode(OF_POLY_WINDING_ODD);	// this is the normal mode
    ofBeginShape();

      ofVertex(16,zz);
      ofVertex(0,zz);
      ofVertex(16,0);
      ofVertex(8,16);
      ofVertex(0,0);

    ofEndShape(OF_CLOSE);
  }

  if( cNote == 3 ) {
    ofSetColor(aa,ss,dd);
    ofDrawCircle(8,8,8);

    float zz = ofMap(temp, 0, 700, 0, 16);
    ofSetColor(0,0,0);
    ofDrawCircle(zz,8,8);
  }

  if( cNote == 4 ) {
    float zz = ofMap(temp, 0, 700, 0, 1);
    ofSetColor(255, 100, 32);
    ofDrawTriangle(0,16, 8,0, 16,16);

    ofPushMatrix();
      // ofTranslate(8,8,0);
      ofSetColor(32, 100, 255);
      ofFill();
      ofSetPolyMode(OF_POLY_WINDING_ODD);
      ofBeginShape();

      for (int i = 0; i < 3; i++){
        ofVertex(aa, ofRandom(0,16));
      }
      for (int i = 0; i < 3; i++){
        ofVertex(ofRandom(0,16), ss);
      }


      ofEndShape(OF_CLOSE);
      ofPushMatrix();
        ofTranslate( 8, 8 );
        ofScale(zz,zz,1);
        ofTranslate(-8,-8);

        ofSetColor(0, 32, 64);
        ofDrawTriangle(0,0, 8,16, 16,0);
      ofPopMatrix();

    ofPopMatrix();
  }

  if( cNote == 5 ) {
    ofSetColor(aa,ss,dd);
    ofDrawRectangle(0,0,16,16);

    float zz = ofMap(temp, 0, 700, -6, 12);
    // float xx = ofMap(temp, 0, 700, 8, -8);
    ofSetColor(0,0,0);
    ofDrawCircle(zz,8,6);
    ofDrawCircle(16-zz,8,6);
  }

  if( cNote == 6 ) {
    // fdbk.draw(0,0);
    float zz = ofMap(temp, 0, 1400, 1, 0);
    float xx = ofMap(temp, 0, 1400, 1, 2);
    fdbk.begin();
      // ofSetColor(255);
      ofPushMatrix();
        ofTranslate( 8, 8 );
        ofScale(zz,zz,1);
        ofTranslate(-8,-8);
        ofSetColor(aa/0.5, ofRandom(2, 32), dd);
        // ofSetColor(ofRandom(255),ofRandom(255),255);
        ofDrawCircle(8,8,4);
      ofPopMatrix();
    fdbk.end();

    ofPushMatrix();
      ofTranslate( 8, 8 );
      ofScale(xx,xx,1);
      ofTranslate(-8,-8);

      fdbk.draw(0,0,16,16);
    ofPopMatrix();
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

  if( cNote == 100 ) {
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
  ofSetColor(255,192,32);
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
  if( note != cNote ) {
    aa = ofRandom(0, 110);
    ss = ofRandom(0, 120);
    dd = ofRandom(110, 255);
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
