#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground( 0 );

    audiofile.load( "data/tubophone.mp3");
    //audiofile.load( "data/tubophone.wav"); // mono wav source file
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofNoFill();
    
    ofPushMatrix();
    for( int c=0; c<audiofile.channels(); ++c ){
        
        float max = ofGetWidth();
        ofBeginShape();
        for( int x=0; x<max; ++x){
            int n = ofMap( x, 0, max, 0, audiofile.length(), true );
            float val = audiofile.sample( n, c );
            float y = ofMap( val, -1.0f, 1.0f, ofGetHeight()*0.5, 0.0f );
            ofVertex( x, y );       
        }
        ofEndShape();        
        
        ofTranslate( 0.0, ofGetHeight()*0.5 );
    }
    ofPopMatrix();
    
    ofDrawBitmapString( audiofile.path(), 10, 20 );

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
