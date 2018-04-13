
#include "ofxAudioFile.h"

#define DR_WAV_IMPLEMENTATION
#include "../libs/dr_wav.h"

#define DR_MP3_IMPLEMENTATION
#include "../libs/dr_mp3.h"

#define DR_FLAC_IMPLEMENTATION
#include "../libs/dr_flac.h"

#include "../libs/stb_vorbis.h"

#include <algorithm>

ofxAudioFile::ofxAudioFile()  {
    filePath = "file not loaded";
    buffer = nullptr;
    nchannels = 0;
    buffersize = 0;
    sampleRate = 11050;
    verbose = false;
}

ofxAudioFile::~ofxAudioFile(){
    if(buffer!=nullptr){
        free();
    }
}

ofxAudioFile::ofxAudioFile(const ofxAudioFile & other) {
    
    free();
    
    if( other.loaded() ){        
        this->filePath = other.filePath;
        this->nchannels = other.nchannels;
        this->buffersize = other.buffersize;
        this->sampleRate = other.sampleRate;
        this->verbose = other.verbose;
        
        // TODO: catch memory exception here
        buffer = new float[other.buffersize];
        
        for( int n=0; n<buffersize; ++n ){
            this->buffer[n] = other.buffer[n];
        }
    }
}

ofxAudioFile & ofxAudioFile::operator= (const ofxAudioFile & other) {
    
    free();
    
    if( other.loaded() ){        
        this->filePath = other.filePath;
        this->nchannels = other.nchannels;
        this->buffersize = other.buffersize;
        this->sampleRate = other.sampleRate;
        this->verbose = other.verbose;
        
        // TODO: catch memory exception here
        buffer = new float[other.buffersize];
        
        for( int n=0; n<buffersize; ++n ){
            this->buffer[n] = other.buffer[n];
        }
    }
    return *this;
}

void ofxAudioFile::setVerbose(bool verbose){
    this->verbose = verbose;
}
        
void ofxAudioFile::free(){
    delete buffer;    
    filePath = "file not loaded";
    buffer = nullptr;
    nchannels = 0;
    buffersize = 0;
    sampleRate = 11050;
}

bool ofxAudioFile::loaded() const{
	if (buffer != nullptr) {
		return true;
	} else {
		return false;
	}
}


// ------------------------- AUDIO LOADING ROTUINES ---------------------------
void ofxAudioFile::load( std::string path ){
    std::string extension = path.substr(path.find_last_of(".") + 1 );
    std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);   

    if( extension == "wav" ){
        load_wav( path );
    }else if ( extension == "flac" ){
        load_flac( path );
    }else if ( extension == "mp3" ){
        load_mp3( path );
    }else if ( extension == "ogg" ){
        load_ogg( path );
    }else{
        std::cout<<"[ERROR] ofxAudioFile : "<<extension<<" is an unsupported file extension\n";
    }
    
}

void ofxAudioFile::load_wav( std::string path ){
    
    unsigned int channels;
    unsigned int sampleRate;
    drwav_uint64 totalSampleCount;

    float* pSampleData =  drwav_open_and_read_file_f32( path.c_str(), &channels, &sampleRate, &totalSampleCount);

    if ( pSampleData == NULL) {
        std::cout<<"[ERROR] ofxAudioFile error loading wav file\n";
    }else{
        if(buffer!=nullptr){ delete buffer; }
        buffer = pSampleData;
        this->sampleRate = sampleRate;
        this->buffersize = totalSampleCount;
        this->nchannels = channels;
        this->slength = this->buffersize / this->nchannels;
        this->filePath = path;
    }
    
}

void ofxAudioFile::load_mp3( std::string path ){

    drmp3_uint64 totalSampleCount;
    drmp3_config config;
    
    float* pSampleData =  drmp3_open_and_decode_file_f32( path.c_str(), &config, &totalSampleCount);

    if ( pSampleData == NULL) {
        std::cout<<"[ERROR] ofxAudioFile error loading mp3 file\n";
    }else{
        if(buffer!=nullptr){ delete buffer; }
        buffer = pSampleData;
        this->sampleRate = config.outputSampleRate;
        this->nchannels = config.outputChannels;
        this->buffersize = totalSampleCount*this->nchannels;
        this->slength = totalSampleCount;
        this->filePath = path;
    }
}

void ofxAudioFile::load_flac( std::string path ){
    
    unsigned int channels;
    unsigned int sampleRate;
    drflac_uint64 totalSampleCount;

    float* pSampleData = drflac_open_and_decode_file_f32( path.c_str(), &channels, &sampleRate, &totalSampleCount);

    if ( pSampleData == NULL) {
        std::cout<<"[ERROR] ofxAudioFile error loading FLAC file\n";
    }else{
        if(buffer!=nullptr){ delete buffer; }
        buffer = pSampleData;
        this->sampleRate = sampleRate;
        this->buffersize = totalSampleCount;
        this->nchannels = channels;
        this->slength = this->buffersize / this->nchannels;
        this->filePath = path;
    }
}


void ofxAudioFile::load_ogg( std::string path ){

    short int *decoded;
    int channels, len, sr;
    len = stb_vorbis_decode_filename(path.c_str(), &channels, &sr, &decoded);

    if(len>0){
        buffer = new float[len];
        
        for( int n=0; n<len; ++n ){
            buffer[n] = static_cast<float>( decoded[n]) / 32768.0f;
        }
        
        this->sampleRate = sr;
        this->buffersize = len;
        this->nchannels = channels;
        this->slength = this->buffersize / this->nchannels;
        this->filePath = path;
    }else{
        std::cout<<"[ERROR] ofxAudioFile error loading ogg/vorbis file\n";
    }

    delete decoded;
}
