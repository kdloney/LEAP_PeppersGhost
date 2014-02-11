#include "testApp.h"

const int width = 800;
const int height = 600;


//--------------------------------------------------------------
void testApp::setup(){
	counter = 0;
	ofSetCircleResolution(50);
	
    //ofBackground(0,0,0);
    
	
    bSmooth = false;
	ofSetWindowTitle("ofxSyphon Example");
    
	mainOutputSyphonServer.setName("CMU_100_SYPHON");
    
	mClient.setup();
    
    //using Syphon app Simple Server, found at http://syphon.v002.info/
    mClient.set("","Simple Server");
	
    tex.allocate(200, 100, GL_RGBA);
    
	ofSetFrameRate(60); // if vertical sync is off, we can go a bit fast... this caps the framerate at 60fps.
    
    ofSetVerticalSync(true);
	ofSetLogLevel(OF_LOG_VERBOSE);
    
    leap.open();
    
	l1.setPosition(-500, 0, 50+sin(10));
	l2.setPosition(500, 0, -500);
    
	cam.setOrientation(ofPoint(-20, 0, 0));
    

    /////////////////////// we need GL_TEXTURE_2D for our models coords.    
    
    if(model.loadModel("CMU_100.dae",true)){
        model.setScale(-3, -3, 1);
    	//model.setPosition(ofGetWidth()/2, (float)ofGetHeight() , 0);
        model.setPosition( model.getPosition().x, model.getPosition().y-500, 0);
        //ofTranslate(-model.getPosition().x+(-1*simpleHands[i].handPos.x), -model.getPosition().y+(-1*simpleHands[i].handPos.y), -model.getPosition().z+200+(-1*simpleHands[i].handPos.z));
        //(-model.getPosition().x+(-1*simpleHands[i].handPos.x), -model.getPosition().y+(-1*simpleHands[i].handPos.y), -model.getPosition().z+200+(-1*simpleHands[i].handPos.z));

        
    	mesh = model.getMesh(0);
    	position = model.getPosition();
    	normScale = model.getNormalizedScale();
    	scale = model.getScale();
    	sceneCenter = model.getSceneCenter();
    	material = model.getMaterialForMesh(0);
        tex = model.getTextureForMesh(0);

    }
    
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    
	glEnable(GL_DEPTH_TEST);
    
    //some model / light stuff
    glShadeModel(GL_SMOOTH);
    //light.enable();
    ofEnableSeparateSpecularLight();
}



//--------------------------------------------------------------
void testApp::update(){
	counter = counter + 0.033f;    
    
    
    simpleHands = leap.getSimpleHands();
    
    if( leap.isFrameNew() && simpleHands.size() ){
        
        leap.setMappingX(-230, 230, -ofGetWidth()/2, ofGetWidth()/2);
		leap.setMappingY(90, 490, -ofGetHeight()/2, ofGetHeight()/2);
        leap.setMappingZ(-150, 150, -200, 200);
        
        for(int i = 0; i < simpleHands.size(); i++){
            
            for(int j = 0; j < simpleHands[i].fingers.size(); j++){
                int id = simpleHands[i].fingers[j].id;
                
                ofPolyline & polyline = fingerTrails[id];
                ofPoint pt = simpleHands[i].fingers[j].pos;
                
                //if the distance between the last point and the current point is too big - lets clear the line
                //this stops us connecting to an old drawing
                if( polyline.size() && (pt-polyline[polyline.size()-1] ).length() > 50 ){
                    polyline.clear();
                }
                
                //add our point to our trail
                polyline.addVertex(pt);
                
                //store fingers seen this frame for drawing
                fingersFound.push_back(id);
            }
        }
    

    }
    
	//IMPORTANT! - tell ofxLeapMotion that the frame is no longer new. 
	leap.markFrameAsOld();	

    
    
}

//--------------------------------------------------------------
void testApp::draw(){
    
    
    float radius;
	
    //Clear with alpha, so we can capture via syphon and composite elsewhere should we want.
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   



    
    //---------------------------- LEAP
    
    //ofDisableLighting();
    
    //ofSetColor(200);
	//ofDrawBitmapString("ofxLeapMotion - Example App\nLeap Connected? " + ofToString(leap.isConnected()), 20, 20);
    
    cam.begin();
   // model.drawFaces();

    
	//ofPushMatrix();
    //ofRotate(90, 0, 0, 1);
    //ofSetColor(20);
    //ofDrawGridPlane(800, 20, false);
	//ofPopMatrix();
	
	ofEnableLighting();
	l1.enable();
	l2.enable();
	
	m1.begin();
	m1.setShininess(0.6);
	
	for(int i = 0; i < fingersFound.size(); i++){
		int id = fingersFound[i];
		
		//ofPolyline & polyline = fingerTrails[id];
		
		//ofSetColor(255 - id * 15, 0, id * 25);
	}
	
    
    for(int i = 0; i < simpleHands.size(); i++){
        //simpleHands[i].debugDraw();
        
        //ofPushMatrix();
        //ofSetColor(255,130,0);
        float radius = (-simpleHands[i].handNormal.z*100);
        //ofFill();		// draw "filled shapes"
        //ofCircle(10, 10,radius);
        //ofPopMatrix();
        
        ofPushMatrix();
        //ofTranslate(model.getPosition().x+100, model.getPosition().y, 0);
        //ofScale(-3, -3);
        //ofTranslate(-model.getPosition().x+250+(-1*simpleHands[i].handPos.x), -model.getPosition().y+200+(-1*simpleHands[i].handPos.y), -model.getPosition().z+200+(-1*simpleHands[i].handPos.z));
        ofTranslate(-model.getPosition().x+(-1*simpleHands[i].handPos.x-100), -model.getPosition().y+(simpleHands[i].handPos.y), -model.getPosition().z+200+(-1*simpleHands[i].handPos.z));
       // ofRotate(), 1, 0, 0);

       ofRotate(radius, 0, 0, 1);
        
        model.drawFaces();
        
        ofPopMatrix();
        
        
        
        glPushAttrib(GL_ALL_ATTRIB_BITS);
        glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS);
        glEnable(GL_NORMALIZE);
        //glEnable(GL_CULL_FACE);
        ofPushMatrix();
        
		tex.bind();
		material.begin();
		mesh.drawWireframe();
		material.end();
		tex.unbind();
        
        ofPopMatrix();
        ofPopMatrix();
        
        glPopAttrib();
        
        l2.disable();


    }
    

    
	m1.end();
	cam.end();
	
	    
	// Syphon Stuff
    
    mClient.draw(50, 50);
    
	mainOutputSyphonServer.publishScreen();
        
   // ofDrawBitmapString("Note this text is not captured by Syphon since it is drawn after publishing.\nYou can use this to hide your GUI for example.", 150,500);
    
    
    
}


//--------------------------------------------------------------
void testApp::keyPressed  (int key){
	if (key == 's'){
		bSmooth = !bSmooth;
	}
}

//--------------------------------------------------------------

void testApp::exit(){
    // let's close down Leap and kill the controller
    leap.close();
}
