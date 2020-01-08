
#include "ofxAudioFile.h"

#define DR_WAV_IMPLEMENTATION
#include "../libs/dr_wav.h"

#define DR_MP3_IMPLEMENTATION
#include "../libs/dr_mp3.h"

#define DR_FLAC_IMPLEMENTATION
#include "../libs/dr_flac.h"

#include "../libs/stb_vorbis.h"

#include <algorithm>
#include <cstring>

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
        
        for( size_t n=0; n<buffersize; ++n ){
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
        
        for( size_t n=0; n<buffersize; ++n ){
            this->buffer[n] = other.buffer[n];
        }
    }
    return *this;
}

void ofxAudioFile::setVerbose(bool verbose){
    this->verbose = verbose;
}
        
void ofxAudioFile::free(){
    if(buffer != nullptr) delete buffer;    
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


// ------------------------- AUDIO LOADING ROUTINES ---------------------------
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
        std::cout<<"[ofxAudioFile] ERROR : "<<extension<<" is an unsupported file extension\n";
    }
    
}

void ofxAudioFile::load_wav( std::string path ){
    
    unsigned int channels;
    unsigned int sampleRate;
    drwav_uint64 totalFrameCount;
    size_t totalSampleCount;

    float* pSampleData =  drwav_open_file_and_read_pcm_frames_f32( path.c_str(), &channels, &sampleRate, &totalFrameCount);

    if ( pSampleData == NULL) {
        std::cout<<"[ofxAudioFile] ERROR loading wav file\n";
    }else{
        if(buffer!=nullptr){ delete buffer; }
        this->nchannels = channels;
        totalSampleCount = (size_t) (totalFrameCount * this->nchannels);
        buffer = new float[totalSampleCount];
        std::memcpy(buffer, pSampleData, totalSampleCount * sizeof(float));
        drwav_free(pSampleData);
        this->sampleRate = sampleRate;
        this->buffersize = totalSampleCount;
        this->slength = this->buffersize / this->nchannels;
        this->filePath = path;
        if(verbose) std::cout<<"[ofxAudioFile] loading "<<this->filePath<<" | sample rate : "<< this->sampleRate <<" | channels : "<<this->nchannels<<" | length : "<<this->slength<<"\n";
    }
    
}

void ofxAudioFile::load_mp3( std::string path ){

    drmp3_uint64 totalFrameCount;
	size_t totalSampleCount;

	drmp3_config config{ 0, 0 };
    
    float* pSampleData =  drmp3_open_file_and_read_f32( path.c_str(), &config, &totalFrameCount);
    if ( pSampleData == NULL) {
        std::cout<<"[ofxAudioFile] ERROR loading mp3 file\n";
    }else{
        if(buffer!=nullptr){ delete buffer; }
        this->nchannels = config.outputChannels;
		totalSampleCount = (size_t) (totalFrameCount * this->nchannels);
        buffer = new float[totalSampleCount];
        std::memcpy(buffer, pSampleData, totalSampleCount * sizeof(float));
        drmp3_free(pSampleData);
        this->sampleRate = config.outputSampleRate;
        this->buffersize = totalSampleCount;
        this->slength = totalFrameCount;
        this->filePath = path;
        if(verbose) std::cout<<"[ofxAudioFile] loading "<<this->filePath<<" | sample rate : "<< this->sampleRate <<" | channels : "<<this->nchannels<<" | length : "<<this->slength<<"\n";
    }
}

void ofxAudioFile::load_flac( std::string path ){
    
    unsigned int channels;
    unsigned int sampleRate;
    drflac_uint64 totalFrameCount;
    size_t totalSampleCount;

    float* pSampleData = drflac_open_file_and_read_pcm_frames_f32( path.c_str(), &channels, &sampleRate, &totalFrameCount, NULL);

    if ( pSampleData == NULL) {
        std::cout<<"[ofxAudioFile] ERROR loading FLAC file\n";
    }else{
        if(buffer!=nullptr){ delete buffer; }
        this->nchannels = channels;
        totalSampleCount = (size_t) (totalFrameCount * this->nchannels);
        buffer = new float[totalSampleCount];
        std::memcpy(buffer, pSampleData, totalSampleCount * sizeof(float));
        drflac_free(pSampleData, NULL);
        this->sampleRate = sampleRate;
        this->buffersize = totalSampleCount;
        this->slength = this->buffersize / this->nchannels;
        this->filePath = path;
        if(verbose) std::cout<<"[ofxAudioFile] loading "<<this->filePath<<" | sample rate : "<< this->sampleRate <<" | channels : "<<this->nchannels<<" | length : "<<this->slength<<"\n";
    }
}


void ofxAudioFile::load_ogg( std::string path ){

    short int *decoded;
    int channels, len, sr;
    len = stb_vorbis_decode_filename(path.c_str(), &channels, &sr, &decoded);
    
    #ifndef __ANDROID__
    len *= channels;
    #endif
    
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
        if(verbose) std::cout<<"[ofxAudioFile] loading "<<this->filePath<<" | sample rate : "<< this->sampleRate <<" | channels : "<<this->nchannels<<" | length : "<<this->slength<<"\n";

        delete decoded;
    }else{
        std::cout<<"[ofxAudioFile] ERROR loading ogg/vorbis file\n";
    }

}
