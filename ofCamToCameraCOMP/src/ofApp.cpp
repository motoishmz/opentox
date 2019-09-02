#include "ofMain.h"
#include "ofxOsc.h"

class ScopedGraphic {
public:
  ScopedGraphic() {
    ofPushStyle();
    ofPushMatrix();
  }
  ~ScopedGraphic() {
    ofPopMatrix();
    ofPopStyle();
  }
};

class ofApp : public ofBaseApp {

  ofxOscSender mOscOut;
  ofEasyCam mCam;

public:
  void setup() {
    ofSetFrameRate(60);
    ofSetVerticalSync(true);
    ofBackground(0);
    mOscOut.setup("localhost", 10000);
  }
  void update() {

    if (ofGetKeyPressed(OF_KEY_SHIFT)) {
      const float fov = ofMap(ofGetMouseX(), 0, ofGetWidth(), 20., 80.);
      mCam.setFov(fov);
    }

    const ofMatrix4x4 proj = mCam.getProjectionMatrix();
    const ofMatrix4x4 model = mCam.getModelViewMatrix();

    //if (mCam.isVFlipped()) {
    //  proj.preMultScale(ofVec3f(1, -1, 1));
    //}

    ofxOscMessage m, p;
    m.setAddress("/modelview");
    p.setAddress("/projection");

    for (int i = 0; i < 16; i++) {
      m.addFloatArg(model.getPtr()[i]);
      p.addFloatArg(proj.getPtr()[i]);
    }

    mOscOut.sendMessage(m);
    mOscOut.sendMessage(p);
  }
  void draw() {
    mCam.begin(); {
      ScopedGraphic g;
      ofNoFill();
      ofSetColor(255);
      ofDrawAxis(100);
      ofDrawBox(300);
    } mCam.end();
  }
  void keyPressed(int key) {

  }
};


int main() {
  ofSetupOpenGL(1280, 720, OF_WINDOW);
  ofRunApp(new ofApp());

}
