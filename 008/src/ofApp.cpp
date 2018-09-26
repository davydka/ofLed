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

vector <string> vids;

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

  /* LUT */
  videoPlayer.load("vids/00.mp4");
  handleOpen();

  dir.allowExt("cube");
  dir.listDir("LUTs/");
  dir.sort();
  if (dir.size()>0) {
    dirLoadIndex=0;
    loadLUT(dir.getPath(dirLoadIndex));
    doLUT = true;
  }else{
    doLUT = false;
  }

  lutImg.allocate(16, 16, OF_IMAGE_COLOR);

  dirV.allowExt("mp4");
  dirV.listDir("vids/");
  vids.resize(dirV.size());

  //go through and print out all the paths
  for(int i = 0; i < dirV.size(); i++){
    vids[i] = dirV.getPath(i);
    ofLogNotice(dirV.getPath(i));
  }
  cout << vids[ofRandom(0,vids.size())] << endl;
}

//--------------------------------------------------------------
void ofApp::update(){
  if( cNote == 100 ) {
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

  if( cNote != 100 ) {
    videoPlayer.update();
    if (videoPlayer.isFrameNew()){
      if (doLUT) {
        pix = videoPlayer.getPixels();
        img.setFromPixels(pix);
        img.resize(16,16);
        applyLUT(img.getPixels());

        
        pix = lutImg.getPixels();
        img.setFromPixels(pix);
        img.resize(16,16);
        applyLUT(img.getPixels());
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

  if( cNote != 100 ) {
    if(videoPlayer.isLoaded()){
      /* LUT */
      if(doLUT)
        lutImg.draw(0,0,16,16);
    }
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
  if( note != cNote && note == 0 ) {
    videoPlayer.load("vids/00.mp4");
    handleOpen();
  }

  cNote = note;
  // cout << temp << endl;
  cout << note << endl;
  temp = 0;

  if( cNote != 100 ) {
    videoPlayer.load(vids[ofRandom(0,vids.size())]);
    handleOpen();
    // videoPlayer.setFrame(0);
  }
}

//--------------------------------------------------------------
void ofApp::exit() {
  if( MIDIPORT != "none") {
    midiIn.closePort();
    midiIn.removeListener(this);
  }
}

void ofApp::star(float x, float y, float z, float w) {
  ofSetColor(255,0,32);
  ofFill();

  float sx = ofMap(x / z, 0, 1, 0, stripWidth);
  float sy = ofMap(y / z, 0, 1, 0, rowHeight);
  float r = ofMap(z, 0, ofGetWidth(), 2, 0);
  ofDrawCircle(sx, sy, r);
}

void ofApp::loadLUT(string path){
  LUTloaded=false;

  ofFile file(path);
  string line;
  for(int i = 0; i < 5; i++) {
    getline(file, line);
    ofLog() << "Skipped line: " << line;
  }
  for(int z=0; z<32; z++){
    for(int y=0; y<32; y++){
      for(int x=0; x<32; x++){
        ofVec3f cur;
        file >> cur.x >> cur.y >> cur.z;
        lut[x][y][z] = cur;
      }
    }
  }

  LUTloaded = true;
}

void ofApp::applyLUT(ofPixelsRef pix){
  if (LUTloaded) {

    for(int y = 0; y < pix.getHeight(); y++){
      for(int x = 0; x < pix.getWidth(); x++){

        ofColor color = pix.getColor(x, y);

        int lutPos [3];
        for (int m=0; m<3; m++) {
          lutPos[m] = color[m] / 8;
          if (lutPos[m]==31) {
            lutPos[m]=30;
          }
        }

        ofVec3f start = lut[lutPos[0]][lutPos[1]][lutPos[2]];
        ofVec3f end = lut[lutPos[0]+1][lutPos[1]+1][lutPos[2]+1];

        for (int k=0; k<3; k++) {
          float amount = (color[k] % 8) / 8.0f;
          color[k]= (start[k] + amount * (end[k] - start[k])) * 64;
        }

        lutImg.setColor(x, y, color);

      }
    }

    lutImg.update();
  }
}

void ofApp::handleOpen(){
  videoPlayer.setVolume(0);
  videoPlayer.setLoopState(OF_LOOP_NORMAL);
  videoPlayer.play();
}


//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){}
