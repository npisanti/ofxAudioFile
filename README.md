ofxAudioFile
=====================================
[![Build status](https://travis-ci.org/npisanti/ofxAudioFile.svg?branch=master)](https://travis-ci.org/npisanti/ofxAudioFile) [![Build status](https://ci.appveyor.com/api/projects/status/e5pnayuh8g8vb04r?svg=true)](https://ci.appveyor.com/project/npisanti/ofxaudiofile)


cross platform audio file loading addon for openFrameworks, wrapping [dr_wav](https://github.com/mackron/dr_libs/blob/master/dr_wav.h), [dr_mp3](https://github.com/mackron/dr_libs/blob/master/dr_mp3.h), [dr_flac](https://github.com/mackron/dr_libs/blob/master/dr_flac.h) and [stb_vorbis](https://github.com/nothings/stb/blob/master/stb_vorbis.c), so you can read Wav, mp3, FLAC and Ogg Vorbis. Works on Linux (desktop and raspberry pi), Windows, OSX, Android and iOS. This addon does not play back samples, it just let you access the float values of the audio file waveform. For sample playing you could use the oF included classes or for more advanced sample playback you could try [ofxPDSP](https://github.com/npisanti/ofxPDSP).   
   
As just standard c++ headers are needed for compiling this addon, it should be also possible to use it without openFrameworks.   
   
Example app loads a mp3 file, or you can drag and drop files in the window if your platform let you do that.   
   
Donate
------------ 
I support you, you support me!    
If you find this code useful, consider [fueling me with coffee](https://ko-fi.com/npisanti)!
   
License 
------------   
As all those libraries are public domain, also this addon have an [unlicense](http://unlicense.org/).   
