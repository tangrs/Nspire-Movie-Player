#!/bin/sh

TEMP_FOLDER=`mktemp -d -t helper`

if [ $# != 3 ]
then
    echo "Usage:"
    echo "$0 inputfile outputfile fps"
    exit 1
fi

if [ ! -f ./moviecompile ];
then
    echo "moviecompile program not found!"
    exit 1
fi

ffmpeg -s qvga -an -r $3 $TEMP_FOLDER/frame-%05d.jpg -i $1

./moviecompile $TEMP_FOLDER $2 $3

rm -R $TEMP_FOLDER