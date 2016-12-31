# Port-Audio-System
Simple audio system based on Port Audio.
Utilizes libsamplerate for sample rate conversion.
Includes support for libsndfile.
Can support for other audio files, see IAudioSource.h and AudioSource_Libsnd.h/cpp to see how to support other audio libraries.

Libraries Required:
[Port Audio](http://www.portaudio.com/)
[Libsamplerate](http://www.mega-nerd.com/SRC/)

Libraries Recomended:
[Libsndfile](http://www.mega-nerd.com/libsndfile/)

# How to compile
The code is set up to be, hopefully, platform independent.  All you have to do for your project is to add in port audio system headers and source files to your code and include Port Audio and Libsamplerate libraries and includes.  You can optionally include Libsndfile library and AudioSource_Libsnd header and source to use Libsndfile in your program.

# How to use
Port Audio System is generally simple to use.

To create an audio source you can either create the audio source itself or use the audio source factory.
The audio source factory itself is also quite simple to use as you supply what audio source you want to use, "libsndfile" for example if you are using AudioSource_Libsnd (audio sources in the factory are usually library names), and the path to the file you want to use.
You can get a list of audio source loaders with "AudioSourceFactory::getFactory().getAudioSources()" which returns a vector of strings of audio source loaders.

Next you create Port Audio System itself with it's constructor.  From here, you can start the playback with "Play" or start adding in audio sources with "PlaySource". Port Audio System will not start playback of audio sources until the "Play" function is called.

# Example
```C++
#include <iostream>
#include "Source/AudioSource_libsnd.h"
#include "PortAudioSystem.h"

int main(){
  //Initialize port audio.
  int err = Pa_Initialize();
  if (err != paNoError) {
    std::cout<<"Port Audio Init Error: "<<Pa_GetErrorText(err)<<std::endl;
    return 1;
  }

  //create a new port audio system
  PortAudioSystem* paSys = new PortAudioSystem();

  //load an audio file using libsndfile audio source.
  IAudioSource * source = AudioSourceFactory::getFactory().newAudioSource("libsndfile", "test.ogg")

  //play the source (port audio system does not start playback until "Play" is called).
  paSys->PlaySource(source, eAS_Music);

  //start port audio system playback.
  paSys->Play();

  //wait for user input
  std::cout<<"Press enter to continue.";
  char * in;
  std::cin>>in;

  //cleanup
  delete paSys;
  delete source;

  //terminate port audio.
  err = Pa_Terminate();
  if (err != paNoError) {
    std::cout<<"Port Audio Terminate Error: "<<Pa_GetErrorText(err)<<std::endl;
    return 1;
  }

  return 0;
}
```

# Current Issues
There is some issues with resampling the sample rate of audio sources which results in audio crackling.  Best practice is to use the same sample rate for both Port Audio System AND the audio source you are using.  By default Port Audio System sets the sample rate to 44100.
