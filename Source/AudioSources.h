#ifndef _H_AUDIOSOURCES
#define _H_AUDIOSOURCES

#pragma once

//auto inclusion of audio sources
//works only if the __has_include preprocessor is available.
#ifdef __has_include
	#if __has_include(<sndfile.h>)
		#include "AudioSource_libsnd.h"
		#include "AudioSource_libsnd_memory.h"
	#endif
#else //otherwise, load all the audio sources even if we dont have that particular audio source.
	#include "AudioSource_libsnd.h"
	#include "AudioSource_libsnd_memory.h"
#endif

#include "AudioSourceFactory.h"

#endif