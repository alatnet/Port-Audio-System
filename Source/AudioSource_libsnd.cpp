#include <StdAfx.h>
#include "AudioSource_libsnd.h"

NSGameStart
	AudioSource_Libsnd::AudioSource_Libsnd(const char * filename) : IAudioSource(){
		CLOG("[AudioSource_libsnd] Loading File: %s", filename);
		this->sndFile = nullptr;
		this->sndFile = sf_open(filename, SFM_READ, &this->sfInfo);

		if (!this->sndFile) {
			int err = sf_error(this->sndFile);
			pushError(err, sf_error_number(err));
			this->sndFile = nullptr;
			return;
		}

		int err = sf_error(this->sndFile);
		if (err != SF_ERR_NO_ERROR) {
			int err = sf_error(this->sndFile);
			pushError(err, sf_error_number(err));
			sf_close(this->sndFile); //close the libsndfile file.
			this->sndFile = nullptr;
			return;
		}
		
		CLOG("[AudioSource_libsnd] Channels: %i", this->sfInfo.channels);
		CLOG("[AudioSource_libsnd] Sample rate: %i", this->sfInfo.samplerate);
		CLOG("[AudioSource_libsnd] Seekable: %i", this->sfInfo.seekable);

		this->m_timeLength.totalSec = (1.0 * this->sfInfo.frames) / this->sfInfo.samplerate;
		this->m_timeLength.hrs = (int)(this->m_timeLength.totalSec / 3600.0);
		this->m_timeLength.min = (int)((this->m_timeLength.totalSec - (this->m_timeLength.hrs * 3600.0)) / 60.0);
		this->m_timeLength.sec = this->m_timeLength.totalSec - (this->m_timeLength.hrs * 3600.0) - (this->m_timeLength.min * 60.0);

		CLOG("[AudioSource_libsnd] File Length: %i:%i:%f - %f", this->m_timeLength.hrs, this->m_timeLength.min, this->m_timeLength.sec, this->m_timeLength.totalSec);
	}
	
	AudioSource_Libsnd::~AudioSource_Libsnd(){
		if (this->sndFile != NULL || this->sndFile != nullptr) {
			sf_close(this->sndFile); //close the libsndfile file.
			this->sndFile = nullptr;
		}
	}

	std::string AudioSource_Libsnd::GetVersion() {
		std::string ret = "[AudioSource_Libsnd] AudioSource - libsndfile version: ";
		ret += sf_version_string();
		return ret;
	}
	
	bool AudioSource_Libsnd::Seek(long long position){
		if (!this->m_hasError) {
			if (sf_seek(this->sndFile, position, SF_SEEK_SET) == -1) {
				int err = sf_error(this->sndFile);
				pushError(err, sf_error_number(err));
				return false;
			}
			return true;
		}
		return false;
	}
	
	long long AudioSource_Libsnd::GetFrames(long long framesToRead, float* buff){
		if (this->m_hasError) return 0;
		return sf_readf_float(this->sndFile, buff, framesToRead);
	}

	bool AudioSource_Libsnd::GetFrame(float* frame) {
		return sf_readf_float(this->sndFile, frame, this->sfInfo.channels) == 1;
	}

	AudioFrame::Type AudioSource_Libsnd::GetFrameType() {
		return (AudioFrame::Type)this->sfInfo.channels;
	}

	#ifdef PAS_CryGame
		void AudioSource_Libsnd::GetMemoryUsage(ICrySizer* pSizer) const{
			pSizer->AddObject(this, sizeof(*this));
			//pSizer->AddObject(this->sndFile, sizeof(float) * this->sfInfo.channels * this->sfInfo.frames);
		}
	#endif
NSGameEnd
