#include "ofApp.h"

string INDEX;
string MIDIPORT;
string ROT;
string FLIP;
int indexInt = 0;
int rot = 0;
int cNote = 0; // current note
int flip = 0;
float temp = 0;


ofVec3f camPos;
string translate;
string scale;
string rotx;
string roty;
string rotz;

ofPlanePrimitive plane;


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
  teensy.serialConfigure("ttyACM0", 0, 0, 100, 100, 0);
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

  // MODEL SETTINGS
  // ofDisableArbTex(); // we need GL_TEXTURE_2D for our models coords.
  animationPosition = 0;

  model.loadModel("ico.dae", false);
  model.disableMaterials();
  model.setLoopStateForAllAnimations(OF_LOOP_NORMAL);
  model.setScale(100,100,100);
  model.setRotation(0,180,0,0,1);
  model.playAllAnimations();

  cout << ofToString(model.getAssimpScene()) << endl;

  xml.loadFile("ico.dae");
  xml.pushTag("COLLADA", 0);
  xml.pushTag("library_visual_scenes", 0);
  xml.pushTag("visual_scene", 0);

  int num = xml.getNumTags("node");
  for( int i = 0; i < num; i++ ) {
    if(xml.getAttribute("node", "id", "", i) == "Camera") {
      xml.pushTag("node", i);
      translate = xml.getValue("translate", "", 0);
      scale = xml.getValue("scale", "", 0);

      int rots = xml.getNumTags("rotate");
      for( int ii = 0; ii < rots; ii++ ) {
        // cout << xml.getAttribute("rotate", "sid", "", ii) << endl;
        if(xml.getAttribute("rotate", "sid", "", ii) == "rotationX") {
          rotx = xml.getValue("rotate", "", ii);
        }
        if(xml.getAttribute("rotate", "sid", "", ii) == "rotationY") {
          roty = xml.getValue("rotate", "", ii);
        }
        if(xml.getAttribute("rotate", "sid", "", ii) == "rotationZ") {
          rotz = xml.getValue("rotate", "", ii);
        }
      }

      xml.popTag();
    }
  }

  vector<string> transPos = ofSplitString(translate, " ");
  vector<string> Xrot = ofSplitString(rotx, " ");
  vector<string> Yrot = ofSplitString(roty, " ");
  vector<string> Zrot = ofSplitString(rotz, " ");

  xml.popTag();
  xml.popTag();
  xml.popTag();

  cam.setAutoDistance(false);
  cam.setFov(35);
  /*
  cam.setPosition(
    0,
    ofToFloat(transPos[2])*100, // y value in blender is Z value in OF
    -(ofToFloat(transPos[1]))*100 // z value in blender is negative Y value in OF
  );
  */
  cam.setTarget(ofVec3f(0,0,0));
  cam.setPosition(0,0,0);
  // cam.setPosition(0,0,420);
  // cam.tiltDeg(ofToFloat(Xrot[3]) - 90); // X for OF, X - 90 from blender
  // cam.panDeg(ofToFloat(Zrot[3])); // Y for OF, Z from blender
  // cam.rollDeg(-(ofToFloat(Yrot[3]))); // Z for OF, negative Y from blender
  cam.setNearClip(.0001);
  cam.setFarClip(100000);

  plane.set(640, 640);
  plane.setPosition(0, 0, 0);
  plane.tiltDeg(90);
  plane.setResolution(64, 64);
}

//--------------------------------------------------------------
void ofApp::update(){
  temp++;

  if(cNote == 100) {
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

  if(cNote != 100) {
    model.update();
  }

  teensy.update();                            // update our serial to teensy stuff

  cout << "cam pos: " + ofToString(cam.getPosition()) << endl;
}

//--------------------------------------------------------------
void ofApp::draw(){
  fbo.begin();
  ofClear(0,0,0);                             // refreshes fbo, removes artifacts
  ofSetColor(255);

  if(cNote == 0) {
    // ofPushMatrix();
    // ofTranslate( 0, -4 );

    ofEnableDepthTest();
    ofEnableSeparateSpecularLight();

    cam.begin();
    // cam.setPosition(camPos);
    ofSetColor(0, 255, 0, 255);
    model.draw(OF_MESH_WIREFRAME);

    // ofSetColor(0, 128, 255, 255);
    // plane.draw(OF_MESH_WIREFRAME);
    cam.end();

    ofDisableDepthTest();
    ofDisableLighting();
    ofDisableSeparateSpecularLight();

    // ofSetColor(0, 255, 0, 255);
    // ofDrawRectangle(0,0,8,8);
    // ofPopMatrix();
  }

  if(cNote == 100) {
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
  fbo.draw(0, 0);
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
