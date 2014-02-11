#pragma once


#include "ofMain.h"
#include "ofxSyphon.h"
#include "ofxLeapMotion.h"
#include "ofxAssimpModelLoader.h"
#include "ofVboMesh.h"



class testApp : public ofBaseApp{
	
public:
	
	void setup();
	void update();
	void draw();
    
    void beginProjection();
    void endProjection();
    
    void drawModel();

	void keyPressed(int key);
	
	float 	counter;
	bool	bSmooth;
	
    ofTexture tex;
    
	ofxSyphonServer mainOutputSyphonServer;
	ofxSyphonServer individualTextureSyphonServer;
	
	ofxSyphonClient mClient;
    
    
    void exit();

    
	ofxLeapMotion leap;
	vector <ofxLeapMotionSimpleHand> simpleHands;
    
	vector <int> fingersFound;
	ofEasyCam cam;
	ofLight l1;
	ofLight l2;
	ofMaterial m1;
	
	map <int, ofPolyline> fingerTrails;
    
    ofxAssimpModelLoader model;
    
    ofVboMesh mesh;
    ofPoint position;
    float normScale;
    ofPoint scale;
    ofPoint sceneCenter;
    ofMaterial material;
    //ofTexture tex;
    ofLight	light;
};




