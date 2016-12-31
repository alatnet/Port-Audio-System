#ifndef _H_AUDIOSOURCE_LIBSND
#define _H_AUDIOSOURCE_LIBSND

#pragma once

#include <sndfile.h>
#include "IAudioSource.h"

NSGameStart
	class AudioSource_Libsnd : public IAudioSource {
	REGISTER_AUDIO_SOURCE("libsndfile", AudioSource_Libsnd)
	public:
		AudioSource_Libsnd(const char * filename);
		~AudioSource_Libsnd();
	public:
		static std::string GetVersion();
	public:
		bool Seek(long long position);
		long long GetFrames(long long framesToRead, float* buff);
		bool GetFrame(float* frame);
		double GetSampleRate() { return this->sfInfo.samplerate; }
		AudioSourceTime GetLength() { return this->m_timeLength; }
		long long GetFrameLength() { return this->sfInfo.frames; }
		AudioFrame::Type GetFrameType();
	#ifdef PAS_CryGame
		public:
			void GetMemoryUsage(ICrySizer* pSizer) const;
	#endif
	private:
		SNDFILE *sndFile;
		SF_INFO sfInfo;
		AudioSourceTime m_timeLength;
	};
NSGameEnd

#endif