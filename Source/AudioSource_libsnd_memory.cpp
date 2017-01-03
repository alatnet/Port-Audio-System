#include <StdAfx.h>
#include "AudioSource_libsnd_memory.h"

NSGameStart
	AudioSource_Libsnd_Memory::AudioSource_Libsnd_Memory(const char * filename) : IAudioSource(){
		CLOG("[AudioSource_libsnd_memory] Loading File: %s", filename);

        //setup
        this->m_pos = this->m_numFrames = this->m_numChannels = 0;
		this->m_samplerate = 0.0f;
        this->m_buff = nullptr;

        //open the file
		SNDFILE *sndFile;
		SF_INFO sfInfo;
		sndFile = sf_open(filename, SFM_READ, &sfInfo);

        //make sure we have no error opening the file.
		if (!sndFile) {
			int err = sf_error(sndFile);
			pushError(err, sf_error_number(err));
			sndFile = nullptr;
			return;
		}

        //make sure we have no errors.
		int err = sf_error(sndFile);
		if (err != SF_ERR_NO_ERROR) {
			int err = sf_error(sndFile);
			pushError(err, sf_error_number(err));
			sf_close(sndFile); //close the libsndfile file.
			sndFile = nullptr;
			return;
		}

        //load the data
        this->m_numFrames = sfInfo.frames;
        this->m_samplerate = sfInfo.samplerate;
        this->m_numChannels = sfInfo.channels;
		
		CLOG("[AudioSource_libsnd_memory] Channels: %i", sfInfo.channels);
		CLOG("[AudioSource_libsnd_memory] Sample rate: %i", sfInfo.samplerate);

        //calculate the length of the file into a human readable format.
		this->m_timeLength.totalSec = (1.0 * sfInfo.frames) / sfInfo.samplerate;
		this->m_timeLength.hrs = (int)(this->m_timeLength.totalSec / 3600.0);
		this->m_timeLength.min = (int)((this->m_timeLength.totalSec - (this->m_timeLength.hrs * 3600.0)) / 60.0);
		this->m_timeLength.sec = this->m_timeLength.totalSec - (this->m_timeLength.hrs * 3600.0) - (this->m_timeLength.min * 60.0);

		CLOG("[AudioSource_libsnd_memory] File Length: %i:%i:%f - %f", this->m_timeLength.hrs, this->m_timeLength.min, this->m_timeLength.sec, this->m_timeLength.totalSec);

        //create the memory buffer
        this->m_buff = new float[sfInfo.frames * sfInfo.channels];

        //make sure we are at the beginning of the file.
        sf_seek(sndFile, 0, SF_SEEK_SET);
        
        //make sure we have no errors.
        err = sf_error(sndFile);
		if (err != SF_ERR_NO_ERROR) {
			int err = sf_error(sndFile);
			pushError(err, sf_error_number(err));
			sf_close(sndFile); //close the libsndfile file.
			sndFile = nullptr;
			return;
		}

        //read the entire file into memory.
        sf_readf_float(sndFile, this->m_buff, sfInfo.frames);

        //make sure we have no error.
        err = sf_error(sndFile);
		if (err != SF_ERR_NO_ERROR) {
			int err = sf_error(sndFile);
			pushError(err, sf_error_number(err));
		}

        //close the file.
        sf_close(sndFile);
	}
	
	AudioSource_Libsnd_Memory::~AudioSource_Libsnd_Memory(){
        //free up the memory.
        delete [] this->m_buff;
	}

	std::string AudioSource_Libsnd_Memory::GetVersion() {
		std::string ret = "[AudioSource_Libsnd_memory] AudioSource - libsndfile version: ";
		ret += sf_version_string();
		return ret;
	}
	
	bool AudioSource_Libsnd_Memory::Seek(long long position){
        //make sure we are within bounds.
        if (position >= this->m_numFrames) position = this->m_numFrames;
        if (position <= 0) position = 0;

        //set internal position.
		this->m_pos = position;

        //return that the seek is good (because we have no errors and that the position is always set correctly).
        return !this->m_hasError;
	}
	
	long long AudioSource_Libsnd_Memory::GetFrames(long long framesToRead, float* buff){
        //make sure we have no errors.
		if (this->m_hasError) return 0;

        //make sure we are not at the end of the buffer.
        if (this->m_pos >= this->m_numFrames) return 0;

		//make sure we are not trying to copy more than what the buffer holds
		if (this->m_pos + framesToRead > this->m_numFrames) framesToRead = (this->m_pos + framesToRead) - this->m_numFrames;

        //copy memory.
        for (long long i=(this->m_pos * this->m_numChannels), i2=0; i<((framesToRead + this->m_pos) * this->m_numChannels); i++, i2++){
            if (i > this->m_numFrames * this->m_numChannels) break;
            buff[i2] = this->m_buff[i];
        }

        //update internal position.
		this->m_pos += framesToRead;

        //return the number of frames read.
		return framesToRead;
	}

	bool AudioSource_Libsnd_Memory::GetFrame(float* frame) {
        //make sure we have no errors.
        if (this->m_hasError) return false;

        //make sure we are not at the end of the buffer.
        if (this->m_pos >= this->m_numFrames) return false;

		//make sure that we are not trying to copy more than what the buffer holds.
		if (this->m_pos + 1 > this->m_numFrames) return false;

		//copy memory.
		for (long long i = (this->m_pos * this->m_numChannels), i2 = 0; i<(this->m_pos * this->m_numChannels); i++, i2++) {
			if (i > this->m_numFrames * this->m_numChannels) break;
			frame[i2] = this->m_buff[i];
		}

        //update internal position
        this->m_pos++;

        //return that we have read the frame.
		return true;
	}

	#ifdef PAS_CryGame
		void AudioSource_Libsnd_Memory::GetMemoryUsage(ICrySizer* pSizer) const{
			pSizer->AddObject(this, sizeof(*this));
            pSizer->AddObject(this->m_buff, sizeof(float) * this->m_numFrames * this->m_numChannels);
		}
	#endif
NSGameEnd