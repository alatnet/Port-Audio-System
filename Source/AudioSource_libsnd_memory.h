#ifndef _H_AUDIOSOURCE_LIBSND_MEMORY
#define _H_AUDIOSOURCE_LIBSND_MEMORY

#pragma once

#include <sndfile.h>
#include "IAudioSource.h"

NSGameStart
    /*
    Audio source that utilizes libsndfile to load an audio file completely into memory.
    */
	class AudioSource_Libsnd_Memory : public IAudioSource {
	REGISTER_AUDIO_SOURCE("libsndfile-memory", AudioSource_Libsnd_Memory)
	public:
		AudioSource_Libsnd_Memory(const char * filename);
		~AudioSource_Libsnd_Memory();
	public:
		static std::string GetVersion();
	public:
		bool Seek(long long position);
		long long GetFrames(long long framesToRead, float* buff);
		bool GetFrame(float* frame);
		double GetSampleRate() { return this->m_samplerate; }
		AudioSourceTime GetLength() { return this->m_timeLength; }
		long long GetFrameLength() { return this->m_numFrames; }
		AudioFrame::Type GetFrameType(){ return (AudioFrame::Type) this->m_numChannels; }
	#ifdef PAS_CryGame
		public:
			void GetMemoryUsage(ICrySizer* pSizer) const;
	#endif
	private:
        long long m_pos;
        long long  m_numFrames;
        double m_samplerate;
        int m_numChannels;
		AudioSourceTime m_timeLength;
        float * m_buff;
	};
NSGameEnd

#endif