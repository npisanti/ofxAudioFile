#!/bin/bash

directory=${PWD##*/}

if [ "$directory" = "ofxAudioFile" ]
then

    echo "ofxAudioFile libraries update"

    git clone https://github.com/nothings/stb.git
    git clone https://github.com/mackron/dr_libs.git

    echo "moving files..."
    mv dr_libs/dr_flac.h libs/dr_flac.h
    mv dr_libs/dr_wav.h libs/dr_wav.h
    mv dr_libs/dr_mp3.h libs/dr_mp3.h
    mv stb/stb_vorbis.c libs/stb_vorbis.h

    echo "deleting repos..."
    rm -rf stb/
    rm -rf dr_libs/

    echo "...done"

else
    echo "use this script from the ofxAudioFile directory"
fi


exit
