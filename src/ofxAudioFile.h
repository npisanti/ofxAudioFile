
#include <iostream>
#include <string>

class ofxAudioFile {

public:
    ofxAudioFile();
    ofxAudioFile(const ofxAudioFile& other);
    ofxAudioFile& operator= (const ofxAudioFile& other);
    ~ofxAudioFile();
    
    /*!
    @brief loads the audio data from a file
    @param[in] filePath absolute or relative path to audio file
    */
    void    load( std::string path );

    /*!
    @brief unload the data from the buffer and free the allocated memory
    */    
    void    free();
    
    /*!
    @brief returns true if data has been loaded into the ofxAudioFile
    */ 
    bool    loaded() const;
    
    /*!
    @brief activate logging of file loading operations for this ofxAudioFile
    @param[in] verbose
    */     
    void    setVerbose(bool verbose);

    /*!
    @brief returns the path of loaded file, or contains diagnostic strings if a file is not loaded
    */  
    std::string path() const {
        return filePath;
    }

    /*!
    @brief returns a pointer to the buffer, use with caution! the internal buffer is initializated to nullptr
    */  
    float * data(){
        return buffer;
    }

    /*!
    @brief loads the audio data from a file
    @param[in] n position of the sample to load, must be less than length()
    @param[in] channel channel of the sample to load
    */
    float sample( int n, int channel ) const{
        return buffer[ n*nchannels + channel ];
    }

    /*!
    @brief returns the sample rate of the sample
    */      
    int samplerate() const {
        return sampleRate;
    }
    
    /*!
    @brief returns the number of channels
    */  
    int channels() const {
        return nchannels;
    }

    /*!
    @brief returns the lenght of each sample channel
    */
    int length() const {
        return slength;
    }

private:
    void load_wav(  std::string path );
    void load_ogg(  std::string path );
    void load_mp3(  std::string path );
    void load_flac( std::string path );
    
    float* buffer;
    
    int             nchannels;   
    int             buffersize;
    int             slength;
    int             sampleRate;
    std::string     filePath;   
    bool            verbose;

};
