#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){

    ofBackground( 0 );

    std::string filepath = ofToDataPath("tubophone.wav"); // mono original file
    //std::string filepath = ofToDataPath("tubophone.mp3"); // at the moment mp3 isn't working on ARM
    //std::string filepath = ofToDataPath("tubophone.ogg"); // ogg/vorbis
    //std::string filepath = ofToDataPath("tubophone.flac"); // FLAC

    audiofile.setVerbose(true); 
    ofSetLogLevel(OF_LOG_VERBOSE);
    
    if( ofFile::doesFileExist( filepath ) ){
        audiofile.load( filepath );
        if (!audiofile.loaded()){
            ofLogError()<<"error loading file, double check the file path";
        }
    }else{
        ofLogError()<<"input file does not exists";
    }
    
    // audio setup for testing audio file stream 
	ofSoundStreamSettings settings;
    sampleRate = 44100.0;
    settings.setOutListener(this);
	settings.sampleRate = sampleRate;
	settings.numOutputChannels = 2;
	settings.numInputChannels = 0;
	settings.bufferSize = 512;
	ofSoundStreamSetup(settings);


    playhead = std::numeric_limits<int>::max(); // because it is converted to int for check
    playheadControl = -1.0;
    step = audiofile.samplerate() / sampleRate;
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
  
        float phx = ofMap( playhead, 0, audiofile.length(), 0, max );
        ofDrawLine( phx, 0, phx, ofGetHeight()*0.5f);
        
        ofTranslate( 0.0, ofGetHeight()*0.5 );
    }
    ofPopMatrix();
    
    ofDrawBitmapString( audiofile.path(), 10, 20 );

    ofDrawBitmapString ( "press SPACEBAR to play, press L to load a sample", 10, ofGetHeight()-20);    
}

//--------------------------------------------------------------
void ofApp::audioOut(ofSoundBuffer & buffer){
    
    // really spartan and not efficient sample playing, just for testing
    
    if( playheadControl >= 0.0 ){
        playhead = playheadControl;
        playheadControl = -1.0;
    }
    
    for (size_t i = 0; i < buffer.getNumFrames(); i++){
        
        int n = playhead;
        
        if( n < audiofile.length()-1 ){
            
            for( size_t k=0; k<buffer.getNumChannels(); ++k){
                if( k < audiofile.channels() ){
                    float fract = playhead - (double) n;
                    float s0 = audiofile.sample( n, k );
                    float s1 = audiofile.sample( n+1, k );
                    float isample = s0*(1.0-fract) + s1*fract; // linear interpolation
                    buffer[i*buffer.getNumChannels() + k] = isample;
                }else{
                    buffer[i*buffer.getNumChannels() + k] = 0.0f;
                }
            }
            
            playhead += step;
        
        }else{
            buffer[i*buffer.getNumChannels()    ] = 0.0f;
            buffer[i*buffer.getNumChannels() + 1] = 0.0f;
            playhead = std::numeric_limits<int>::max();
        }

    }
}
		
		

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 
    audiofile.load( dragInfo.files[0] );
}

//--------------------------------------------------------------
void ofApp::exit(){
    ofSoundStreamClose();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if( key == ' ') playheadControl = 0.0;
    
    if( key == 'l' || key=='L'){
       //Open the Open File Dialog
        ofFileDialogResult openFileResult= ofSystemLoadDialog("select an audio sample"); 
        //Check if the user opened a file
        if (openFileResult.bSuccess){
            string filepath = openFileResult.getPath();            
            audiofile.load ( filepath );
            step = audiofile.samplerate() / sampleRate;
            ofLogVerbose("file loaded");
        }else {
            ofLogVerbose("User hit cancel");
        }
    }
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


