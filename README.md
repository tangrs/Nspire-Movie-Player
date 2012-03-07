# Nspire Movie Player

This is a movie player for the NSpire calculator. http://youtu.be/Yx0L7ce5hq4

## Status

At the moment, this only works for the NSpire CX. Older models are extremely easy to port to.

Only a POC at the moment. More polish to come.

## What's the difference between this and nPlayer?

I haven't had a look since the start of 2011 at nPlayer but at that time, it looked like it couldn't run for more than a few seconds. The compression they used was run length encoding.

This player uses a public domain jpeg decoder to decode each frame and relies on jpeg compression to keep file sizes low. This has a performance penalty on the calculator (it had trouble even running 15fps video natively) but significantly reduces the file size (especially for the CX where there's more pixel data).

I don't know about nPlayer, but a typical NMP movie file size is about ~2.86Mb per minute at 15fps.

## Compiling

Clone this repo to some location on your computer. Alternatively, download an archive and extract it.

Make sure you have a working Ndless SDK. Go into the source code and change the movie filename.

Change into the directory and run ```make``` and it should compile everything.

## How convert movies for this?

First you need the following installed on your host machine:
* ffmpeg
* A C compiler

Make a folder for all your frames. Then compile the helper.c program and copy the executable into your folder.

Change into your directory and run the following command on the command line:

    ffmpeg -i /path/to/movie -s qvga -an -r 15 frame-%05d.jpg

This will spit out a whole lot of files. When it finishes, run the helper program and it will produce a ```jpegmov.tns``` file. Copy this to your calculator along with the main program and run it.

Enjoy your movie.

## Licence

Licenced under GPLv3. See LICENSE.