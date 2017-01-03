#include <StdAfx.h>
#include "PortAudioSystem.h"

NSGameStart
	PortAudioSystem::PortAudioSystem(double sampleRate, PaDeviceIndex device, void * hostApiSpecificStreamInfo) {
		//default stuff
		this->m_masterVol /*= this->m_musicVol = this->m_sfxVol*/ = 1.0f;
		for (int i = 0; i < eAS_Count; i++) this->m_vols[i] = 1.0f;
		this->m_deltaTime = 0.0f;
		this->m_pAudioStream = nullptr;
		this->m_hasError = false;
		this->m_pSrcState = nullptr;
		this->m_nextPlayID = 0;

		this->m_sampleRate = sampleRate;

		PaStreamParameters streamParams;
		streamParams.device = device; //set device to use
		streamParams.hostApiSpecificStreamInfo = hostApiSpecificStreamInfo;
		streamParams.sampleFormat = paFloat32; // 32bit float format
		streamParams.suggestedLatency = PAS_Millsecond(200); //200 ms ought to satisfy even the worst sound card - effects delta time (higher ms - higher delta)
		streamParams.channelCount = this->m_numChannels = AudioFrame::Type::eAFT_2; //number of channels (1: mono, 2: left/right, etc)

		int err = 0;

		err = Pa_OpenStream(
			&this->m_pAudioStream,
			0, // no input
			&streamParams,
			sampleRate,
			paFramesPerBufferUnspecified, // let portaudio choose the buffersize
			paNoFlag,/* no special modes (clip off, dither off) */
			PortAudioSystem::paCallbackCommon, //set the callback to be the function that calls the per class callback
			this
		);

		if (err != paNoError) {
			pushError(err, Pa_GetErrorText(err));
			this->m_pAudioStream = nullptr;
		}

		int src_err;
		this->m_pSrcState = src_new(SRC_SINC_MEDIUM_QUALITY,2,&src_err); //create a new sample rate converter

		if (this->m_pSrcState == NULL) this->pushError(src_err, src_strerror(src_err));
	}

	PortAudioSystem::~PortAudioSystem() {
		//if we are playing, stop playing.
		this->Stop();

		//delete the sample rate converter
		src_delete(this->m_pSrcState);
		this->m_pSrcState = nullptr;

		//close the stream.
		if (this->m_pAudioStream != NULL) Pa_CloseStream(this->m_pAudioStream);
		this->m_pAudioStream = nullptr;
	}

	//volume functions
	void PortAudioSystem::SetMasterVol(float vol) {
		if (vol >= 1.0f) vol = 1.0f;
		if (vol <= 0.0f) vol = 0.0f;
		this->m_masterVol = vol;
	}

	void PortAudioSystem::SetVolume(float vol, EAudioSection section) {
		if (section < 0 || section > eAS_Count) return;
		if (vol >= 1.0f) vol = 1.0f;
		if (vol <= 0.0f) vol = 0.0f;
		this->m_vols[section];
	}

	float PortAudioSystem::GetVolume(EAudioSection section) {
		if (section < 0 || section > eAS_Count) return 0.0f;
		return this->m_vols[section];
	}

	//basic functionallity
	void PortAudioSystem::Play() {
		if (!this->IsPlaying() && this->m_pAudioStream != nullptr) {
			int err = Pa_StartStream(this->m_pAudioStream);
			if (err != paNoError) pushError(err, Pa_GetErrorText(err));
		}
	}

	void PortAudioSystem::Pause() {
		if (this->IsPlaying() && this->m_pAudioStream != nullptr) {
			int err = Pa_StopStream(this->m_pAudioStream);
			if (err != paNoError) pushError(err, Pa_GetErrorText(err));
		}
		this->m_deltaTime = 0.0f;
	}

	void PortAudioSystem::Stop() {
		if (this->IsPlaying() && this->m_pAudioStream != nullptr) {
			int err = Pa_StopStream(this->m_pAudioStream);
			if (err != paNoError) pushError(err, Pa_GetErrorText(err));
		}

		this->m_playingAudioSource.clear(); //remove all playing audio sources

		this->m_deltaTime = 0.0f;
	}

	//audio source functions
	long PortAudioSystem::PlaySource(IAudioSource * source, EAudioSection section) {
		if (source == nullptr || source == NULL) return -1;
		CLOG("[PortAudioSystem] Prepping to add file.");
		long uID = this->m_nextPlayID++;

		int flags = source->GetFlags();

		PlayingAudioSource *playingSource = new PlayingAudioSource();
		playingSource->audioSource = source;
		playingSource->section = section;
		playingSource->startFrame = 0;
		playingSource->currentFrame = 0;
		playingSource->endFrame = source->GetFrameLength();
		playingSource->loop = ((flags & eAF_Loop) == 1);
		playingSource->paused = ((flags & eAF_PausedOnStart) == 1);

		m_callbackMutex.lock();
		CLOG("[PortAudioSystem] Adding to playing files...");
		this->m_playingAudioSource.insert(std::make_pair<>(uID,playingSource));
		CLOG("[PortAudioSystem] Done!");
		m_callbackMutex.unlock();

		return uID;
	}

	void PortAudioSystem::PauseSource(long id) {
		m_callbackMutex.lock();
		if (this->m_playingAudioSource.find(id) == this->m_playingAudioSource.end()) {
			m_callbackMutex.unlock();
			return;
		}
		this->m_playingAudioSource[id]->paused = true;
		m_callbackMutex.unlock();
	}

	void PortAudioSystem::ResumeSource(long id) {
		m_callbackMutex.lock();
		if (this->m_playingAudioSource.find(id) == this->m_playingAudioSource.end()) {
			m_callbackMutex.unlock();
			return;
		}
		this->m_playingAudioSource[id]->paused = false;
		m_callbackMutex.unlock();
	}

	void PortAudioSystem::StopSource(long id) {
		m_callbackMutex.lock();
		if (this->m_playingAudioSource.find(id) != this->m_playingAudioSource.end()) this->m_playingAudioSource.erase(id);
		m_callbackMutex.unlock();
	}

	bool PortAudioSystem::IsPlaying(long id) {
		m_callbackMutex.lock();
		if (this->m_playingAudioSource.find(id) != this->m_playingAudioSource.end()) {
			bool paused = this->m_playingAudioSource[id]->paused;
			m_callbackMutex.unlock();
			return !paused;
		}
		m_callbackMutex.unlock();

		return false;
	}

	AudioSourceTime PortAudioSystem::GetTime(long id) {
		long long currTime = 0;
		m_callbackMutex.lock();
		if (this->m_playingAudioSource.find(id) != this->m_playingAudioSource.end()) currTime = this->m_playingAudioSource[id]->currentFrame;
		m_callbackMutex.unlock();

		AudioSourceTime timeLength;
		timeLength.totalSec = (1.0 * currTime) / this->m_playingAudioSource[id]->audioSource->GetSampleRate();
		timeLength.hrs = (int)(timeLength.totalSec / 3600.0);
		timeLength.min = (int)((timeLength.totalSec - (timeLength.hrs * 3600.0)) / 60.0);
		timeLength.sec = timeLength.totalSec - (timeLength.hrs * 3600.0) - (timeLength.min * 60.0);

		return timeLength;
	}

	void PortAudioSystem::SetTime(long id, double time) {
		long long setFrame = (long long)(time * this->m_playingAudioSource[id]->audioSource->GetSampleRate());
		m_callbackMutex.lock();
		if (this->m_playingAudioSource.find(id) != this->m_playingAudioSource.end()) {
			if (setFrame < this->m_playingAudioSource[id]->startFrame) setFrame = this->m_playingAudioSource[id]->startFrame;
			if (setFrame > this->m_playingAudioSource[id]->endFrame) setFrame = this->m_playingAudioSource[id]->endFrame;
			this->m_playingAudioSource[id]->currentFrame = setFrame;
		}
		m_callbackMutex.unlock();
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////
	//Error checking
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	PASError PortAudioSystem::GetError() {
		if (this->m_errors.size() == 0) {
			PASError noErr;
			this->m_hasError = false;
			return noErr;
		}

		//m_callbackMutex.lock();
		m_errorMutex.lock();
		PASError ret = this->m_errors.back();
		this->m_errors.pop_back();

		if (this->m_errors.size() == 0) this->m_hasError = false;
		m_errorMutex.unlock();
		//m_callbackMutex.unlock();
		return ret;
	}

	bool PortAudioSystem::HasError() {
		//m_callbackMutex.lock();
		m_errorMutex.lock();
		bool ret = this->m_hasError;
		m_errorMutex.unlock();
		//m_callbackMutex.unlock();
		return ret;
	}

	void PortAudioSystem::pushError(int errorCode, const char * errorStr) {
		m_errorMutex.lock();
		PASError err;
		err.code = errorCode;
		err.str = errorStr;
		this->m_errors.push_back(err);
		this->m_hasError = true;
		m_errorMutex.unlock();
	}

	//setup here ensures that we have a constant delta time.
	//by returning 0, we assume that the delta time was not updated.
	float PortAudioSystem::GetDeltaTime() {
		float ret = 0;
		if (m_deltaTimeMutex.try_lock()) {
			ret = this->m_deltaTime;
			m_deltaTimeMutex.unlock();
		}
		return ret;
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////
	//THE ALL MIGHTY CALLBACKS!
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	//TODO: fix samplerate conversion cracking sound
	int PortAudioSystem::paCallback(const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void *userData) {
		m_deltaTimeMutex.lock();
		this->m_deltaTime = (float)((double)framesPerBuffer / this->m_sampleRate); //get delta time based on sample rate and number of frames per buffer
		m_deltaTimeMutex.unlock();

		{ //clear the buffer
			float * outputFBuff = (float *)outputBuffer;
			for (int i = 0; i < framesPerBuffer * this->m_numChannels; i++) outputFBuff[i] = 0.0f;
		}

		if (this->m_playingAudioSource.size() == 0) return paContinue; //if we dont have any playing audio files, skip.
		
		float *framesOut = new float[framesPerBuffer*this->m_numChannels]; //create a buffer for frames out.
		
		//prep common data for sample rate conversion.
		SRC_DATA src_data;
		src_data.output_frames = framesPerBuffer; //set output frames to be the max output frames to have.
		src_data.data_out = framesOut; //set our output frames to be the buffer that we created.

		m_callbackMutex.lock(); //mutex lock
		for (std::pair<long, PlayingAudioSource *> entry : this->m_playingAudioSource) { //for each playing audio file
			if (entry.second->paused) continue; //if the audio file is paused, skip it.
			PlayingAudioSource* playingsource = entry.second;

			playingsource->audioSource->Seek(playingsource->currentFrame); //seek to the current position of the file

			src_data.src_ratio = this->m_sampleRate / playingsource->audioSource->GetSampleRate(); //get the ratio of the sample rate conversion

			if (src_data.src_ratio == 1.0f) { //if we are 1 to 1, dont do sample rate conversion.
				src_data.input_frames_used = src_data.output_frames_gen = (long)playingsource->audioSource->GetFrames(framesPerBuffer, framesOut);
			} else { //otherwise convert to port audio system's sample rate.
				//adjust the number of frames to read based on ratio.
				long long framesToRead;

				//framesIn = (framesOut*sRateIn)/sRateOut;

				framesToRead = ((long long)((double)framesPerBuffer / src_data.src_ratio));

				//if (fmod((double)framesPerBuffer, src_data.src_ratio) == 0) framesToRead = framesPerBuffer;
				//else {
				//	framesToRead = (long long)((double)framesPerBuffer * src_data.src_ratio) + 2; //fix this. audio crackling.
				//}

				//create a buffer for frames in.
				float * framesIn = new float[framesToRead * playingsource->audioSource->GetFrameType()];
				src_data.data_in = framesIn; //set the frame in buffer

				src_data.input_frames = (long)playingsource->audioSource->GetFrames(framesToRead, framesIn); //read the frames; //set the number of frames that were read

				if (src_data.input_frames == 0) { //if we have no data
					if (playingsource->currentFrame >= playingsource->endFrame) //if we are at the end of the file.
						m_stoppedAudioFiles.push_back(entry.first); //mark it for removal.
					delete[] framesIn; //free up the frame in buffer to prevent memory leaks.
					continue; //skip
				}

				src_data.end_of_input = (playingsource->currentFrame + src_data.input_frames >= playingsource->endFrame); //tell the sample rate converter if we are at the end or not.

				src_reset(this->m_pSrcState); //reset the sample rate conversion state
				int src_err = src_process(this->m_pSrcState, &src_data); //convert the sample rate

				if (src_err != 0) this->pushError(src_err, src_strerror(src_err)); //if we have an error, push it back.

				delete[] framesIn; //free up the frame in buffer
			}

			//multi channel up upmixing/downmixing code
			/*
			all this section of code does is based on the
			number of channels the port audio system has been set up with
			and the number of the audio sources channels, convert the
			audio sources number of channels to port audio system's number of channels.
			I.E.:
			PAS -> 2 channels
			AS -> 5 channels
			5 channels => 2 channels
			*/

			//this is how the code below breaks out for a direct 1 - 1 channel output.
			/*
			AudioFrame::af2* outFrame = (AudioFrame::_2*)outputBuffer;
			AudioFrame::af2* aframesOut = (AudioFrame::_2*)framesOut;
			for (long outFrameI = 0; outFrameI < src_data.output_frames_gen; outFrameI++) { //for each frame (based on how many frames were generated from conversion [possibly under framesPerBuffer])
				//adjust volume
				aframesOut[outFrameI].left *= (this->m_masterVol * this->m_vols[playingsource->section]);
				aframesOut[outFrameI].right *= (this->m_masterVol * this->m_vols[playingsource->section]);

				if (aframesOut[outFrameI].left != 0.0f){ //make sure we have data.
					if (outFrame[outFrameI].left == 0.0f) //if the output frame has no data
						outFrame[outFrameI].left = aframesOut[outFrameI].left; //set the audio
					else //if the output frame has data
						outFrame[outFrameI].left += aframesOut[outFrameI].left; //mix the audio
					outFrame[outFrameI].left = CLAMP(outFrame[outFrameI].left, -1.0f, 1.0f); //make sure we dont clip
				}
				if (aframesOut[outFrameI].right != 0.0f){ //make sure we have data.
					if (outFrame[outFrameI].right == 0.0f) //if the output frame has no data
						outFrame[outFrameI].right = aframesOut[outFrameI].right; //set the audio
					else //if the output frame has data
						outFrame[outFrameI].right += aframesOut[outFrameI].right; //mix the audio
					outFrame[outFrameI].right = CLAMP(outFrame[outFrameI].right, -1.0f, 1.0f); //make sure we dont clip
				}
			}
			*/

			//this is the same code above but with the macros used below. a lot cleaner than the code above and does the same exact job.
			/*
			SETOUTFRAME(2);
			FRAMECAST(2);
			for (long outFrameI = 0; outFrameI < src_data.output_frames_gen; outFrameI++) {
				FRAMEVOL(left);
				FRAMEVOL(right);
				FRAMEOUTD(left);
				FRAMEOUTD(right);
			}
			*/

			//this is how the code below breaks out for a 3 channel audio source to a 2 channel output.
			/*
			AudioFrame::af2* outFrame = (AudioFrame::_2*)outputBuffer;
			AudioFrame::af3* aframesOut = (AudioFrame::_3*)framesOut;
			for (long outFrameI = 0; outFrameI < src_data.output_frames_gen; outFrameI++) { //for each frame (based on how many frames were generated from conversion [possibly under framesPerBuffer])
				//adjust volume
				aframesOut[outFrameI].left *= (this->m_masterVol * this->m_vols[playingsource->section]);
				aframesOut[outFrameI].right *= (this->m_masterVol * this->m_vols[playingsource->section]);
				aframesOut[outFrameI].center *= (this->m_masterVol * this->m_vols[playingsource->section]);

				AudioFrame::af2 frame = AudioFrame::Convert::To2CH(aframesOut[outFrameI]);

				if (frame.left != 0.0f) { //make sure we have data.
					if (outFrame[outFrameI].left == 0.0f) //if the output frame has no data
						outFrame[outFrameI].left = frame.left; //set the audio
					else //if the output frame has data
						outFrame[outFrameI].left += frame.left; //mix the audio
					outFrame[outFrameI].left = CLAMP(outFrame[outFrameI].left, -1.0f, 1.0f); //make sure we dont clip
				}
				if (frame.right != 0.0f) { //make sure we have data.
					if (outFrame[outFrameI].right == 0.0f) //if the output frame has no data
						outFrame[outFrameI].right = frame.right; //set the audio
					else //if the output frame has data
						outFrame[outFrameI].right += frame.right; //mix the audio
					outFrame[outFrameI].right = CLAMP(outFrame[outFrameI].right, -1.0f, 1.0f); //make sure we dont clip
				}
			}
			*/
			
			//this is the same code above but with the macros used below. a lot cleaner than the code above and does the same exact job.
			/*
			SETOUTFRAME(2);
			FRAMECAST(3);
			for (long outFrameI = 0; outFrameI < src_data.output_frames_gen; outFrameI++) {
				FRAMEVOL(left);
				FRAMEVOL(right);
				FRAMEVOL(center);
				FRAMECONVERT(2);
				FRAMEOUT(left);
				FRAMEOUT(right);
			}
			*/

			//this is how the code below breaks out for a 2 channel audio source to a 3 channel output
			/*
			AudioFrame::af3* outFrame = (AudioFrame::_3*)outputBuffer;
			AudioFrame::af2* aframesOut = (AudioFrame::_2*)framesOut;
			for (long outFrameI = 0; outFrameI < src_data.output_frames_gen; outFrameI++) { //for each frame (based on how many frames were generated from conversion [possibly under framesPerBuffer])
				//adjust volume
				aframesOut[outFrameI].left *= (this->m_masterVol * this->m_vols[playingsource->section]);
				aframesOut[outFrameI].right *= (this->m_masterVol * this->m_vols[playingsource->section]);

				AudioFrame::af3 frame = AudioFrame::Convert::To3CH(aframesOut[outFrameI]);

				if (frame.left != 0.0f) { //make sure we have data.
					if (outFrame[outFrameI].left == 0.0f) //if the output frame has no data
						outFrame[outFrameI].left = frame.left; //set the audio
					else //if the output frame has data
						outFrame[outFrameI].left += frame.left; //mix the audio
					outFrame[outFrameI].left = CLAMP(outFrame[outFrameI].left, -1.0f, 1.0f); //make sure we dont clip
				}
				if (frame.right != 0.0f) { //make sure we have data.
					if (outFrame[outFrameI].right == 0.0f) //if the output frame has no data
						outFrame[outFrameI].right = frame.right; //set the audio
					else //if the output frame has data
						outFrame[outFrameI].right += frame.right; //mix the audio
					outFrame[outFrameI].right = CLAMP(outFrame[outFrameI].right, -1.0f, 1.0f); //make sure we dont clip
				}
				if (frame.center != 0.0f) { //make sure we have data.
					if (outFrame[outFrameI].center == 0.0f) //if the output frame has no data
						outFrame[outFrameI].center = frame.center; //set the audio
					else //if the output frame has data
						outFrame[outFrameI].center += frame.center; //mix the audio
					outFrame[outFrameI].center = CLAMP(outFrame[outFrameI].center, -1.0f, 1.0f); //make sure we dont clip
				}
			}
			*/

			//this is the same code above but with the macros used below. a lot cleaner than the code above and does the same exact job.
			/*
			SETOUTFRAME(3);
			FRAMECAST(2);
			for (long outFrameI = 0; outFrameI < src_data.output_frames_gen; outFrameI++) {
				FRAMEVOL(left);
				FRAMEVOL(right);
				FRAMECONVERT(3);
				FRAMEOUT(left);
				FRAMEOUT(right);
				FRAMEOUT(center)
			}
			*/

AudioFrame::af1 frame;
			//macros
			/* While most people say that macros will make everything harder to debug, this set of macros should work fine and allows for the code to be cleaner looking (that and im really, REALLY lazy...) */
			//set the output buffer frame to the specific type
			#define SETOUTFRAME(type) AudioFrame::af##type##* outFrame = (AudioFrame::af##type##*)outputBuffer;

			//cast the out frames to the correct frame type
			#define FRAMECAST(type) AudioFrame::af##type##* aframesOut = (AudioFrame::af##type##*)framesOut;

			//adjust the out frame specific channel volume
			#define FRAMEVOL(channel) aframesOut[outFrameI].##channel *= (this->m_masterVol * this->m_vols[playingsource->section]);

			//convert to a specific frame type
			#define FRAMECONVERT(type) AudioFrame::af##type frame = AudioFrame::Convert::To##type##CH(aframesOut[outFrameI]);
			#define FRAMECONVERTMONO() AudioFrame::af1 frame = AudioFrame::Convert::ToMono(aframesOut[outFrameI]);
				
			//output the out frame specific channel to the buffer
			#define FRAMEOUT(channel) \
			if (frame.##channel != 0.0f) { \
				if (outFrame[outFrameI].##channel == 0.0f) outFrame[outFrameI].##channel = frame.##channel; \
				else outFrame[outFrameI].##channel += frame.##channel; \
				outFrame[outFrameI].##channel = CLAMP(outFrame[outFrameI].##channel, -1.0f, 1.0f); \
			}

			//directly output the frame specific channel to the buffer
			#define FRAMEOUTD(channel) \
			if (aframesOut[outFrameI].##channel != 0.0f) { \
				if (outFrame[outFrameI].##channel == 0.0f) outFrame[outFrameI].##channel = aframesOut[outFrameI].##channel; \
				else outFrame[outFrameI].##channel += aframesOut[outFrameI].##channel; \
				outFrame[outFrameI].##channel = CLAMP(outFrame[outFrameI].##channel, -1.0f, 1.0f); \
			}

			//mix all the audio!
			switch (this->m_numChannels) { //based on output number of channels
			case AudioFrame::Type::eAFT_Mono: //mono is a special-ish case to convert to.
				{
					SETOUTFRAME(1);
					switch (playingsource->audioSource->GetFrameType()) { //based on audio sources number of channels
					case AudioFrame::Type::eAFT_Mono:
						{
							FRAMECAST(1);
							for (long outFrameI = 0; outFrameI < src_data.output_frames_gen; outFrameI++) {
								FRAMEVOL(mono);
								FRAMEOUTD(mono);
							}
						}
						break;
					case AudioFrame::Type::eAFT_2:
						{
							FRAMECAST(2);
							for (long outFrameI = 0; outFrameI < src_data.output_frames_gen; outFrameI++) {
								FRAMEVOL(left);
								FRAMEVOL(right);
								FRAMECONVERTMONO();
								FRAMEOUT(mono);
							}
						}
						break;
					case AudioFrame::Type::eAFT_21:
						{
							FRAMECAST(21);
							for (long outFrameI = 0; outFrameI < src_data.output_frames_gen; outFrameI++) {
								FRAMEVOL(left);
								FRAMEVOL(right);
								FRAMEVOL(sub);
								FRAMECONVERTMONO();
								FRAMEOUT(mono);
							}
						}
						break;
					case AudioFrame::Type::eAFT_31:
						{
							FRAMECAST(31);
							for (long outFrameI = 0; outFrameI < src_data.output_frames_gen; outFrameI++) {
								FRAMEVOL(left);
								FRAMEVOL(right);
								FRAMEVOL(center);
								FRAMEVOL(sub);
								FRAMECONVERTMONO();
								FRAMEOUT(mono);
							}
						}
						break;
					case AudioFrame::Type::eAFT_5:
						{
							FRAMECAST(5);
							for (long outFrameI = 0; outFrameI < src_data.output_frames_gen; outFrameI++) {
								FRAMEVOL(front.left);
								FRAMEVOL(front.right);
								FRAMEVOL(center);
								FRAMEVOL(back.left);
								FRAMEVOL(back.right);
								FRAMECONVERTMONO();
								FRAMEOUT(mono);
							}
						}
						break;
					case AudioFrame::Type::eAFT_51:
						{
							FRAMECAST(51);
							for (long outFrameI = 0; outFrameI < src_data.output_frames_gen; outFrameI++) {
								FRAMEVOL(front.left);
								FRAMEVOL(front.right);
								FRAMEVOL(center);
								FRAMEVOL(back.left);
								FRAMEVOL(back.right);
								FRAMEVOL(sub);
								FRAMECONVERTMONO();
								FRAMEOUT(mono);
							}
						}
						break;
					case AudioFrame::Type::eAFT_7:
						{
							FRAMECAST(7);
							for (long outFrameI = 0; outFrameI < src_data.output_frames_gen; outFrameI++) {
								FRAMEVOL(front.left);
								FRAMEVOL(front.right);
								FRAMEVOL(center);
								FRAMEVOL(side.left);
								FRAMEVOL(side.right);
								FRAMEVOL(back.left);
								FRAMEVOL(back.right);
								FRAMECONVERTMONO();
								FRAMEOUT(mono);
							}
						}
						break;
					case AudioFrame::Type::eAFT_71:
						{
							FRAMECAST(71);
							for (long outFrameI = 0; outFrameI < src_data.output_frames_gen; outFrameI++) {
								FRAMEVOL(front.left);
								FRAMEVOL(front.right);
								FRAMEVOL(center);
								FRAMEVOL(side.left);
								FRAMEVOL(side.right);
								FRAMEVOL(back.left);
								FRAMEVOL(back.right);
								FRAMEVOL(sub);
								FRAMECONVERTMONO();
								FRAMEOUT(mono);
							}
						}
						break;
					}
				}
				break;
			case AudioFrame::Type::eAFT_2:
				{
					SETOUTFRAME(2);
					switch (playingsource->audioSource->GetFrameType()) {
					case AudioFrame::Type::eAFT_Mono:
						{
							FRAMECAST(1);
							for (long outFrameI = 0; outFrameI < src_data.output_frames_gen; outFrameI++) {
								FRAMEVOL(mono);
								FRAMECONVERT(2);
								FRAMEOUT(left);
								FRAMEOUT(right);
							}
						}
						break;
					case AudioFrame::Type::eAFT_2:
						{
							FRAMECAST(2);
							for (long outFrameI = 0; outFrameI < src_data.output_frames_gen; outFrameI++) {
								FRAMEVOL(left);
								FRAMEVOL(right);

								FRAMEOUTD(left);
								FRAMEOUTD(right);
							}
						}
						break;
					case AudioFrame::Type::eAFT_21:
						{
							FRAMECAST(21);
							for (long outFrameI = 0; outFrameI < src_data.output_frames_gen; outFrameI++) {
								FRAMEVOL(left);
								FRAMEVOL(right);
								FRAMEVOL(sub);
								FRAMECONVERT(2);
								FRAMEOUT(left);
								FRAMEOUT(right);
							}
						}
						break;
					case AudioFrame::Type::eAFT_31:
						{
							FRAMECAST(31);
							for (long outFrameI = 0; outFrameI < src_data.output_frames_gen; outFrameI++) {
								FRAMEVOL(left);
								FRAMEVOL(right);
								FRAMEVOL(center);
								FRAMEVOL(sub);
								FRAMECONVERT(2);
								FRAMEOUT(left);
								FRAMEOUT(right);
							}
						}
						break;
					case AudioFrame::Type::eAFT_5:
						{
							FRAMECAST(5);
							for (long outFrameI = 0; outFrameI < src_data.output_frames_gen; outFrameI++) {
								FRAMEVOL(front.left);
								FRAMEVOL(front.right);
								FRAMEVOL(center);
								FRAMEVOL(back.left);
								FRAMEVOL(back.right);
								FRAMECONVERT(2);
								FRAMEOUT(left);
								FRAMEOUT(right);
							}
						}
						break;
					case AudioFrame::Type::eAFT_51:
						{
							FRAMECAST(51);
							for (long outFrameI = 0; outFrameI < src_data.output_frames_gen; outFrameI++) {
								FRAMEVOL(front.left);
								FRAMEVOL(front.right);
								FRAMEVOL(center);
								FRAMEVOL(back.left);
								FRAMEVOL(back.right);
								FRAMEVOL(sub);
								FRAMECONVERT(2);
								FRAMEOUT(left);
								FRAMEOUT(right);
							}
						}
						break;
					case AudioFrame::Type::eAFT_7:
						{
							FRAMECAST(7);
							for (long outFrameI = 0; outFrameI < src_data.output_frames_gen; outFrameI++) {
								FRAMEVOL(front.left);
								FRAMEVOL(front.right);
								FRAMEVOL(center);
								FRAMEVOL(side.left);
								FRAMEVOL(side.right);
								FRAMEVOL(back.left);
								FRAMEVOL(back.right);
								FRAMECONVERT(2);
								FRAMEOUT(left);
								FRAMEOUT(right);
							}
						}
						break;
					case AudioFrame::Type::eAFT_71:
						{
							FRAMECAST(71);
							for (long outFrameI = 0; outFrameI < src_data.output_frames_gen; outFrameI++) {
								FRAMEVOL(front.left);
								FRAMEVOL(front.right);
								FRAMEVOL(center);
								FRAMEVOL(side.left);
								FRAMEVOL(side.right);
								FRAMEVOL(back.left);
								FRAMEVOL(back.right);
								FRAMEVOL(sub);
								FRAMECONVERT(2);
								FRAMEOUT(left);
								FRAMEOUT(right);
							}
						}
						break;
					}
				}
				break;
			case AudioFrame::Type::eAFT_21:
				{
					SETOUTFRAME(21);
					switch (playingsource->audioSource->GetFrameType()) {
					case AudioFrame::Type::eAFT_Mono:
						{
							FRAMECAST(1);
							for (long outFrameI = 0; outFrameI < src_data.output_frames_gen; outFrameI++) {
								FRAMEVOL(mono);
								FRAMECONVERT(21);
								FRAMEOUT(left);
								FRAMEOUT(right);
								FRAMEOUT(sub);
							}
						}
						break;
					case AudioFrame::Type::eAFT_2:
						{
							FRAMECAST(2);
							for (long outFrameI = 0; outFrameI < src_data.output_frames_gen; outFrameI++) {
								FRAMEVOL(left);
								FRAMEVOL(right);
								FRAMECONVERT(21);
								FRAMEOUT(left);
								FRAMEOUT(right);
								FRAMEOUT(sub);
							}
						}
						break;
					case AudioFrame::Type::eAFT_21:
						{
							FRAMECAST(21);
							for (long outFrameI = 0; outFrameI < src_data.output_frames_gen; outFrameI++) {
								FRAMEVOL(left);
								FRAMEVOL(right);
								FRAMEVOL(sub);

								FRAMEOUTD(left);
								FRAMEOUTD(right);
								FRAMEOUTD(sub);
							}
						}
						break;
					case AudioFrame::Type::eAFT_31:
						{
							FRAMECAST(31);
							for (long outFrameI = 0; outFrameI < src_data.output_frames_gen; outFrameI++) {
								FRAMEVOL(left);
								FRAMEVOL(right);
								FRAMEVOL(center);
								FRAMEVOL(sub);
								FRAMECONVERT(21);
								FRAMEOUT(left);
								FRAMEOUT(right);
								FRAMEOUT(sub);
							}
						}
						break;
					case AudioFrame::Type::eAFT_5:
						{
							FRAMECAST(5);
							for (long outFrameI = 0; outFrameI < src_data.output_frames_gen; outFrameI++) {
								FRAMEVOL(front.left);
								FRAMEVOL(front.right);
								FRAMEVOL(center);
								FRAMEVOL(back.left);
								FRAMEVOL(back.left);
								FRAMECONVERT(21);
								FRAMEOUT(left);
								FRAMEOUT(right);
								FRAMEOUT(sub);
							}
						}
						break;
					case AudioFrame::Type::eAFT_51:
						{
							FRAMECAST(51);
							for (long outFrameI = 0; outFrameI < src_data.output_frames_gen; outFrameI++) {
								FRAMEVOL(front.left);
								FRAMEVOL(front.right);
								FRAMEVOL(center);
								FRAMEVOL(back.left);
								FRAMEVOL(back.left);
								FRAMEVOL(sub);
								FRAMECONVERT(21);
								FRAMEOUT(left);
								FRAMEOUT(right);
								FRAMEOUT(sub);
							}
						}
						break;
					case AudioFrame::Type::eAFT_7:
						{
							FRAMECAST(7);
							for (long outFrameI = 0; outFrameI < src_data.output_frames_gen; outFrameI++) {
								FRAMEVOL(front.left);
								FRAMEVOL(front.right);
								FRAMEVOL(center);
								FRAMEVOL(side.left);
								FRAMEVOL(side.right);
								FRAMEVOL(back.left);
								FRAMEVOL(back.left);
								FRAMECONVERT(21);
								FRAMEOUT(left);
								FRAMEOUT(right);
								FRAMEOUT(sub);
							}
						}
						break;
					case AudioFrame::Type::eAFT_71:
						{
							FRAMECAST(71);
							for (long outFrameI = 0; outFrameI < src_data.output_frames_gen; outFrameI++) {
								FRAMEVOL(front.left);
								FRAMEVOL(front.right);
								FRAMEVOL(center);
								FRAMEVOL(side.left);
								FRAMEVOL(side.right);
								FRAMEVOL(back.left);
								FRAMEVOL(back.left);
								FRAMEVOL(sub);
								FRAMECONVERT(21);
								FRAMEOUT(left);
								FRAMEOUT(right);
								FRAMEOUT(sub);
							}
						}
						break;
					}
				}
				break;
			case AudioFrame::Type::eAFT_31:
				{
					SETOUTFRAME(31);
					switch (playingsource->audioSource->GetFrameType()) {
					case AudioFrame::Type::eAFT_Mono:
						{
							FRAMECAST(1);
							for (long outFrameI = 0; outFrameI < src_data.output_frames_gen; outFrameI++) {
								FRAMEVOL(mono);
								FRAMECONVERT(31);
								FRAMEOUT(left);
								FRAMEOUT(right);
								FRAMEOUT(center);
								FRAMEOUT(sub);
							}
						}
						break;
					case AudioFrame::Type::eAFT_2:
						{
							FRAMECAST(2);
							for (long outFrameI = 0; outFrameI < src_data.output_frames_gen; outFrameI++) {
								FRAMEVOL(left);
								FRAMEVOL(right);
								FRAMECONVERT(31);
								FRAMEOUT(left);
								FRAMEOUT(right);
								FRAMEOUT(center);
								FRAMEOUT(sub);
							}
						}
						break;
					case AudioFrame::Type::eAFT_21:
						{
							FRAMECAST(21);
							for (long outFrameI = 0; outFrameI < src_data.output_frames_gen; outFrameI++) {
								FRAMEVOL(left);
								FRAMEVOL(right);
								FRAMEVOL(sub);
								FRAMECONVERT(31);
								FRAMEOUT(left);
								FRAMEOUT(right);
								FRAMEOUT(center);
								FRAMEOUT(sub);
							}
						}
						break;
					case AudioFrame::Type::eAFT_31:
						{
							FRAMECAST(31);
							for (long outFrameI = 0; outFrameI < src_data.output_frames_gen; outFrameI++) {
								FRAMEVOL(left);
								FRAMEVOL(right);
								FRAMEVOL(center);
								FRAMEVOL(sub);

								FRAMEOUTD(left);
								FRAMEOUTD(right);
								FRAMEOUTD(center);
								FRAMEOUTD(sub);
							}
						}
						break;
					case AudioFrame::Type::eAFT_5:
						{
							FRAMECAST(5);
							for (long outFrameI = 0; outFrameI < src_data.output_frames_gen; outFrameI++) {
								FRAMEVOL(front.left);
								FRAMEVOL(front.right);
								FRAMEVOL(center);
								FRAMEVOL(back.left);
								FRAMEVOL(back.right);
								FRAMECONVERT(31);
								FRAMEOUT(left);
								FRAMEOUT(right);
								FRAMEOUT(center);
								FRAMEOUT(sub);
							}
						}
						break;
					case AudioFrame::Type::eAFT_51:
						{
							FRAMECAST(51);
							for (long outFrameI = 0; outFrameI < src_data.output_frames_gen; outFrameI++) {
								FRAMEVOL(front.left);
								FRAMEVOL(front.right);
								FRAMEVOL(center);
								FRAMEVOL(back.left);
								FRAMEVOL(back.right);
								FRAMEVOL(sub);
								FRAMECONVERT(31);
								FRAMEOUT(left);
								FRAMEOUT(right);
								FRAMEOUT(center);
								FRAMEOUT(sub);
							}
						}
						break;
					case AudioFrame::Type::eAFT_7:
						{
							FRAMECAST(7);
							for (long outFrameI = 0; outFrameI < src_data.output_frames_gen; outFrameI++) {
								FRAMEVOL(front.left);
								FRAMEVOL(front.right);
								FRAMEVOL(center);
								FRAMEVOL(side.left);
								FRAMEVOL(side.right);
								FRAMEVOL(back.left);
								FRAMEVOL(back.right);
								FRAMECONVERT(31);
								FRAMEOUT(left);
								FRAMEOUT(right);
								FRAMEOUT(center);
								FRAMEOUT(sub);
							}
						}
						break;
					case AudioFrame::Type::eAFT_71:
						{
							FRAMECAST(71);
							for (long outFrameI = 0; outFrameI < src_data.output_frames_gen; outFrameI++) {
								FRAMEVOL(front.left);
								FRAMEVOL(front.right);
								FRAMEVOL(center);
								FRAMEVOL(side.left);
								FRAMEVOL(side.right);
								FRAMEVOL(back.left);
								FRAMEVOL(back.right);
								FRAMEVOL(sub);
								FRAMECONVERT(31);
								FRAMEOUT(left);
								FRAMEOUT(right);
								FRAMEOUT(center);
								FRAMEOUT(sub);
							}
						}
						break;
					}
				}
				break;
			case AudioFrame::Type::eAFT_5:
				{
					SETOUTFRAME(5);
					switch (playingsource->audioSource->GetFrameType()) {
					case AudioFrame::Type::eAFT_Mono:
						{
							FRAMECAST(1);
							for (long outFrameI = 0; outFrameI < src_data.output_frames_gen; outFrameI++) {
								FRAMEVOL(mono);
								FRAMECONVERT(5);
								FRAMEOUT(front.left);
								FRAMEOUT(front.right);
								FRAMEOUT(center);
								FRAMEOUT(back.left);
								FRAMEOUT(back.right);
							}
						}
						break;
					case AudioFrame::Type::eAFT_2:
						{
							FRAMECAST(2);
							for (long outFrameI = 0; outFrameI < src_data.output_frames_gen; outFrameI++) {
								FRAMEVOL(left);
								FRAMEVOL(right);
								FRAMECONVERT(5);
								FRAMEOUT(front.left);
								FRAMEOUT(front.right);
								FRAMEOUT(center);
								FRAMEOUT(back.left);
								FRAMEOUT(back.right);
							}
						}
						break;
					case AudioFrame::Type::eAFT_21:
						{
							FRAMECAST(21);
							for (long outFrameI = 0; outFrameI < src_data.output_frames_gen; outFrameI++) {
								FRAMEVOL(left);
								FRAMEVOL(right);
								FRAMEVOL(sub);
								FRAMECONVERT(5);
								FRAMEOUT(front.left);
								FRAMEOUT(front.right);
								FRAMEOUT(center);
								FRAMEOUT(back.left);
								FRAMEOUT(back.right);
							}
						}
						break;
					case AudioFrame::Type::eAFT_31:
						{
							FRAMECAST(31);
							for (long outFrameI = 0; outFrameI < src_data.output_frames_gen; outFrameI++) {
								FRAMEVOL(left);
								FRAMEVOL(right);
								FRAMEVOL(center);
								FRAMEVOL(sub);
								FRAMECONVERT(5);
								FRAMEOUT(front.left);
								FRAMEOUT(front.right);
								FRAMEOUT(center);
								FRAMEOUT(back.left);
								FRAMEOUT(back.right);
							}
						}
						break;
					case AudioFrame::Type::eAFT_5:
						{
							FRAMECAST(5);
							for (long outFrameI = 0; outFrameI < src_data.output_frames_gen; outFrameI++) {
								FRAMEVOL(front.left);
								FRAMEVOL(front.right);
								FRAMEVOL(center);
								FRAMEVOL(back.left);
								FRAMEVOL(back.right);

								FRAMEOUTD(front.left);
								FRAMEOUTD(front.right);
								FRAMEOUTD(center);
								FRAMEOUTD(back.left);
								FRAMEOUTD(back.right);
							}
						}
						break;
					case AudioFrame::Type::eAFT_51:
						{
							FRAMECAST(51);
							for (long outFrameI = 0; outFrameI < src_data.output_frames_gen; outFrameI++) {
								FRAMEVOL(front.left);
								FRAMEVOL(front.right);
								FRAMEVOL(center);
								FRAMEVOL(back.left);
								FRAMEVOL(back.right);
								FRAMEVOL(sub);
								FRAMECONVERT(5);
								FRAMEOUT(front.left);
								FRAMEOUT(front.right);
								FRAMEOUT(center);
								FRAMEOUT(back.left);
								FRAMEOUT(back.right);
							}
						}
						break;
					case AudioFrame::Type::eAFT_7:
						{
							FRAMECAST(7);
							for (long outFrameI = 0; outFrameI < src_data.output_frames_gen; outFrameI++) {
								FRAMEVOL(front.left);
								FRAMEVOL(front.right);
								FRAMEVOL(center);
								FRAMEVOL(side.left);
								FRAMEVOL(side.right);
								FRAMEVOL(back.left);
								FRAMEVOL(back.right);
								FRAMECONVERT(5);
								FRAMEOUT(front.left);
								FRAMEOUT(front.right);
								FRAMEOUT(center);
								FRAMEOUT(back.left);
								FRAMEOUT(back.right);
							}
						}
						break;
					case AudioFrame::Type::eAFT_71:
						{
							FRAMECAST(71);
							for (long outFrameI = 0; outFrameI < src_data.output_frames_gen; outFrameI++) {
								FRAMEVOL(front.left);
								FRAMEVOL(front.right);
								FRAMEVOL(center);
								FRAMEVOL(side.left);
								FRAMEVOL(side.right);
								FRAMEVOL(back.left);
								FRAMEVOL(back.right);
								FRAMEVOL(sub);
								FRAMECONVERT(5);
								FRAMEOUT(front.left);
								FRAMEOUT(front.right);
								FRAMEOUT(center);
								FRAMEOUT(back.left);
								FRAMEOUT(back.right);
							}
						}
						break;
					}
				}
				break;
			case AudioFrame::Type::eAFT_51:
				{
					SETOUTFRAME(51);
					switch (playingsource->audioSource->GetFrameType()) {
					case AudioFrame::Type::eAFT_Mono:
						{
							FRAMECAST(1);
							for (long outFrameI = 0; outFrameI < src_data.output_frames_gen; outFrameI++) {
								FRAMEVOL(mono);
								FRAMECONVERT(51);
								FRAMEOUT(front.left);
								FRAMEOUT(front.right);
								FRAMEOUT(center);
								FRAMEOUT(back.left);
								FRAMEOUT(back.right);
								FRAMEOUT(sub);
							}
						}
						break;
					case AudioFrame::Type::eAFT_2:
						{
							FRAMECAST(2);
							for (long outFrameI = 0; outFrameI < src_data.output_frames_gen; outFrameI++) {
								FRAMEVOL(left);
								FRAMEVOL(right);
								FRAMECONVERT(51);
								FRAMEOUT(front.left);
								FRAMEOUT(front.right);
								FRAMEOUT(center);
								FRAMEOUT(back.left);
								FRAMEOUT(back.right);
								FRAMEOUT(sub);
							}
						}
						break;
					case AudioFrame::Type::eAFT_21:
						{
							FRAMECAST(21);
							for (long outFrameI = 0; outFrameI < src_data.output_frames_gen; outFrameI++) {
								FRAMEVOL(left);
								FRAMEVOL(right);
								FRAMEVOL(sub);
								FRAMECONVERT(51);
								FRAMEOUT(front.left);
								FRAMEOUT(front.right);
								FRAMEOUT(center);
								FRAMEOUT(back.left);
								FRAMEOUT(back.right);
								FRAMEOUT(sub);
							}
						}
						break;
					case AudioFrame::Type::eAFT_31:
						{
							FRAMECAST(31);
							for (long outFrameI = 0; outFrameI < src_data.output_frames_gen; outFrameI++) {
								FRAMEVOL(left);
								FRAMEVOL(right);
								FRAMEVOL(center);
								FRAMEVOL(sub);
								FRAMECONVERT(51);
								FRAMEOUT(front.left);
								FRAMEOUT(front.right);
								FRAMEOUT(center);
								FRAMEOUT(back.left);
								FRAMEOUT(back.right);
								FRAMEOUT(sub);
							}
						}
						break;
					case AudioFrame::Type::eAFT_5:
						{
							FRAMECAST(5);
							for (long outFrameI = 0; outFrameI < src_data.output_frames_gen; outFrameI++) {
								FRAMEVOL(front.left);
								FRAMEVOL(front.right);
								FRAMEVOL(center);
								FRAMEVOL(back.left);
								FRAMEVOL(back.right);
								FRAMECONVERT(51);
								FRAMEOUT(front.left);
								FRAMEOUT(front.right);
								FRAMEOUT(center);
								FRAMEOUT(back.left);
								FRAMEOUT(back.right);
								FRAMEOUT(sub);
							}
						}
						break;
					case AudioFrame::Type::eAFT_51:
						{
							FRAMECAST(51);
							for (long outFrameI = 0; outFrameI < src_data.output_frames_gen; outFrameI++) {
								FRAMEVOL(front.left);
								FRAMEVOL(front.right);
								FRAMEVOL(center);
								FRAMEVOL(back.left);
								FRAMEVOL(back.right);
								FRAMEVOL(sub);
								FRAMEOUTD(front.left);
								FRAMEOUTD(front.right);
								FRAMEOUTD(center);
								FRAMEOUTD(back.left);
								FRAMEOUTD(back.right);
								FRAMEOUTD(sub);
							}
						}
						break;
					case AudioFrame::Type::eAFT_7:
						{
							FRAMECAST(7);
							for (long outFrameI = 0; outFrameI < src_data.output_frames_gen; outFrameI++) {
								FRAMEVOL(front.left);
								FRAMEVOL(front.right);
								FRAMEVOL(center);
								FRAMEVOL(side.left);
								FRAMEVOL(side.right);
								FRAMEVOL(back.left);
								FRAMEVOL(back.right);
								FRAMECONVERT(51);
								FRAMEOUT(front.left);
								FRAMEOUT(front.right);
								FRAMEOUT(center);
								FRAMEOUT(back.left);
								FRAMEOUT(back.right);
								FRAMEOUT(sub);
							}
						}
						break;
					case AudioFrame::Type::eAFT_71:
						{
							FRAMECAST(71);
							for (long outFrameI = 0; outFrameI < src_data.output_frames_gen; outFrameI++) {
								FRAMEVOL(front.left);
								FRAMEVOL(front.right);
								FRAMEVOL(center);
								FRAMEVOL(side.left);
								FRAMEVOL(side.right);
								FRAMEVOL(back.left);
								FRAMEVOL(back.right);
								FRAMEVOL(sub);
								FRAMECONVERT(51);
								FRAMEOUT(front.left);
								FRAMEOUT(front.right);
								FRAMEOUT(center);
								FRAMEOUT(back.left);
								FRAMEOUT(back.right);
								FRAMEOUT(sub);
							}
						}
						break;
					}
				}
				break;
			case AudioFrame::Type::eAFT_7:
				{
					SETOUTFRAME(7);
					switch (playingsource->audioSource->GetFrameType()) {
					case AudioFrame::Type::eAFT_Mono:
						{
							FRAMECAST(1);
							for (long outFrameI = 0; outFrameI < src_data.output_frames_gen; outFrameI++) {
								FRAMEVOL(mono);
								FRAMECONVERT(7);
								FRAMEOUT(front.left);
								FRAMEOUT(front.right);
								FRAMEOUT(center);
								FRAMEOUT(side.left);
								FRAMEOUT(side.right);
								FRAMEOUT(back.left);
								FRAMEOUT(back.right);
							}
						}
						break;
					case AudioFrame::Type::eAFT_2:
						{
							FRAMECAST(2);
							for (long outFrameI = 0; outFrameI < src_data.output_frames_gen; outFrameI++) {
								FRAMEVOL(left);
								FRAMEVOL(right);
								FRAMECONVERT(7);
								FRAMEOUT(front.left);
								FRAMEOUT(front.right);
								FRAMEOUT(center);
								FRAMEOUT(side.left);
								FRAMEOUT(side.right);
								FRAMEOUT(back.left);
								FRAMEOUT(back.right);
							}
						}
						break;
					case AudioFrame::Type::eAFT_21:
						{
							FRAMECAST(21);
							for (long outFrameI = 0; outFrameI < src_data.output_frames_gen; outFrameI++) {
								FRAMEVOL(left);
								FRAMEVOL(right);
								FRAMEVOL(sub);
								FRAMECONVERT(7);
								FRAMEOUT(front.left);
								FRAMEOUT(front.right);
								FRAMEOUT(center);
								FRAMEOUT(side.left);
								FRAMEOUT(side.right);
								FRAMEOUT(back.left);
								FRAMEOUT(back.right);
							}
						}
						break;
					case AudioFrame::Type::eAFT_31:
						{
							FRAMECAST(31);
							for (long outFrameI = 0; outFrameI < src_data.output_frames_gen; outFrameI++) {
								FRAMEVOL(left);
								FRAMEVOL(right);
								FRAMEVOL(center);
								FRAMEVOL(sub);
								FRAMECONVERT(7);
								FRAMEOUT(front.left);
								FRAMEOUT(front.right);
								FRAMEOUT(center);
								FRAMEOUT(side.left);
								FRAMEOUT(side.right);
								FRAMEOUT(back.left);
								FRAMEOUT(back.right);
							}
						}
						break;
					case AudioFrame::Type::eAFT_5:
						{
							FRAMECAST(5);
							for (long outFrameI = 0; outFrameI < src_data.output_frames_gen; outFrameI++) {
								FRAMEVOL(front.left);
								FRAMEVOL(front.right);
								FRAMEVOL(center);
								FRAMEVOL(back.left);
								FRAMEVOL(back.right);
								FRAMECONVERT(7);
								FRAMEOUT(front.left);
								FRAMEOUT(front.right);
								FRAMEOUT(center);
								FRAMEOUT(side.left);
								FRAMEOUT(side.right);
								FRAMEOUT(back.left);
								FRAMEOUT(back.right);
							}
						}
						break;
					case AudioFrame::Type::eAFT_51:
						{
							FRAMECAST(51);
							for (long outFrameI = 0; outFrameI < src_data.output_frames_gen; outFrameI++) {
								FRAMEVOL(front.left);
								FRAMEVOL(front.right);
								FRAMEVOL(center);
								FRAMEVOL(back.left);
								FRAMEVOL(back.right);
								FRAMEVOL(sub);
								FRAMECONVERT(7);
								FRAMEOUT(front.left);
								FRAMEOUT(front.right);
								FRAMEOUT(center);
								FRAMEOUT(side.left);
								FRAMEOUT(side.right);
								FRAMEOUT(back.left);
								FRAMEOUT(back.right);
							}
						}
						break;
					case AudioFrame::Type::eAFT_7:
						{
							FRAMECAST(7);
							for (long outFrameI = 0; outFrameI < src_data.output_frames_gen; outFrameI++) {
								FRAMEVOL(front.left);
								FRAMEVOL(front.right);
								FRAMEVOL(center);
								FRAMEVOL(side.left);
								FRAMEVOL(side.right);
								FRAMEVOL(back.left);
								FRAMEVOL(back.right);

								FRAMEOUTD(front.left);
								FRAMEOUTD(front.right);
								FRAMEOUTD(center);
								FRAMEOUTD(side.left);
								FRAMEOUTD(side.right);
								FRAMEOUTD(back.left);
								FRAMEOUTD(back.right);
							}
						}
						break;
					case AudioFrame::Type::eAFT_71:
						{
							FRAMECAST(71);
							for (long outFrameI = 0; outFrameI < src_data.output_frames_gen; outFrameI++) {
								FRAMEVOL(front.left);
								FRAMEVOL(front.right);
								FRAMEVOL(center);
								FRAMEVOL(side.left);
								FRAMEVOL(side.right);
								FRAMEVOL(back.left);
								FRAMEVOL(back.right);
								FRAMEVOL(sub);
								FRAMECONVERT(7);
								FRAMEOUT(front.left);
								FRAMEOUT(front.right);
								FRAMEOUT(center);
								FRAMEOUT(side.left);
								FRAMEOUT(side.right);
								FRAMEOUT(back.left);
								FRAMEOUT(back.right);
							}
						}
						break;
					}
				}
				break;
			case AudioFrame::Type::eAFT_71:
				{
					SETOUTFRAME(71);
					switch (playingsource->audioSource->GetFrameType()) {
					case AudioFrame::Type::eAFT_Mono:
						{
							FRAMECAST(1);
							for (long outFrameI = 0; outFrameI < src_data.output_frames_gen; outFrameI++) {
								FRAMEVOL(mono);
								FRAMECONVERT(71);
								FRAMEOUT(front.left);
								FRAMEOUT(front.right);
								FRAMEOUT(center);
								FRAMEOUT(side.left);
								FRAMEOUT(side.right);
								FRAMEOUT(back.left);
								FRAMEOUT(back.right);
								FRAMEOUT(sub);
							}
						}
						break;
					case AudioFrame::Type::eAFT_2:
						{
							FRAMECAST(2);
							for (long outFrameI = 0; outFrameI < src_data.output_frames_gen; outFrameI++) {
								FRAMEVOL(left);
								FRAMEVOL(right);
								FRAMECONVERT(71);
								FRAMEOUT(front.left);
								FRAMEOUT(front.right);
								FRAMEOUT(center);
								FRAMEOUT(side.left);
								FRAMEOUT(side.right);
								FRAMEOUT(back.left);
								FRAMEOUT(back.right);
								FRAMEOUT(sub);
							}
						}
						break;
					case AudioFrame::Type::eAFT_21:
						{
							FRAMECAST(21);
							for (long outFrameI = 0; outFrameI < src_data.output_frames_gen; outFrameI++) {
								FRAMEVOL(left);
								FRAMEVOL(right);
								FRAMEVOL(sub);
								FRAMECONVERT(71);
								FRAMEOUT(front.left);
								FRAMEOUT(front.right);
								FRAMEOUT(center);
								FRAMEOUT(side.left);
								FRAMEOUT(side.right);
								FRAMEOUT(back.left);
								FRAMEOUT(back.right);
								FRAMEOUT(sub);
							}
						}
						break;
					case AudioFrame::Type::eAFT_31:
						{
							FRAMECAST(31);
							for (long outFrameI = 0; outFrameI < src_data.output_frames_gen; outFrameI++) {
								FRAMEVOL(left);
								FRAMEVOL(right);
								FRAMEVOL(center);
								FRAMEVOL(sub);
								FRAMECONVERT(71);
								FRAMEOUT(front.left);
								FRAMEOUT(front.right);
								FRAMEOUT(center);
								FRAMEOUT(side.left);
								FRAMEOUT(side.right);
								FRAMEOUT(back.left);
								FRAMEOUT(back.right);
								FRAMEOUT(sub);
							}
						}
						break;
					case AudioFrame::Type::eAFT_5:
						{
							FRAMECAST(5);
							for (long outFrameI = 0; outFrameI < src_data.output_frames_gen; outFrameI++) {
								FRAMEVOL(front.left);
								FRAMEVOL(front.right);
								FRAMEVOL(center);
								FRAMEVOL(back.left);
								FRAMEVOL(back.right);
								FRAMECONVERT(71);
								FRAMEOUT(front.left);
								FRAMEOUT(front.right);
								FRAMEOUT(center);
								FRAMEOUT(side.left);
								FRAMEOUT(side.right);
								FRAMEOUT(back.left);
								FRAMEOUT(back.right);
								FRAMEOUT(sub);
							}
						}
						break;
					case AudioFrame::Type::eAFT_51:
						{
							FRAMECAST(51);
							for (long outFrameI = 0; outFrameI < src_data.output_frames_gen; outFrameI++) {
								FRAMEVOL(front.left);
								FRAMEVOL(front.right);
								FRAMEVOL(center);
								FRAMEVOL(back.left);
								FRAMEVOL(back.right);
								FRAMEVOL(sub);
								FRAMECONVERT(71);
								FRAMEOUT(front.left);
								FRAMEOUT(front.right);
								FRAMEOUT(center);
								FRAMEOUT(side.left);
								FRAMEOUT(side.right);
								FRAMEOUT(back.left);
								FRAMEOUT(back.right);
								FRAMEOUT(sub);
							}
						}
						break;
					case AudioFrame::Type::eAFT_7:
						{
							FRAMECAST(7);
							for (long outFrameI = 0; outFrameI < src_data.output_frames_gen; outFrameI++) {
								FRAMEVOL(front.left);
								FRAMEVOL(front.right);
								FRAMEVOL(center);
								FRAMEVOL(side.left);
								FRAMEVOL(side.right);
								FRAMEVOL(back.left);
								FRAMEVOL(back.right);
								FRAMECONVERT(71);
								FRAMEOUT(front.left);
								FRAMEOUT(front.right);
								FRAMEOUT(center);
								FRAMEOUT(side.left);
								FRAMEOUT(side.right);
								FRAMEOUT(back.left);
								FRAMEOUT(back.right);
								FRAMEOUT(sub);
							}
						}
						break;
					case AudioFrame::Type::eAFT_71:
						{
							FRAMECAST(71);
							for (long outFrameI = 0; outFrameI < src_data.output_frames_gen; outFrameI++) {
								FRAMEVOL(front.left);
								FRAMEVOL(front.right);
								FRAMEVOL(center);
								FRAMEVOL(side.left);
								FRAMEVOL(side.right);
								FRAMEVOL(back.left);
								FRAMEVOL(back.right);
								FRAMEVOL(sub);

								FRAMEOUTD(front.left);
								FRAMEOUTD(front.right);
								FRAMEOUTD(center);
								FRAMEOUTD(side.left);
								FRAMEOUTD(side.right);
								FRAMEOUTD(back.left);
								FRAMEOUTD(back.right);
								FRAMEOUTD(sub);
							}
						}
						break;
					}
				}
				break;
			}

			//clear the macros
			#undef SETOUTFRAME
			#undef FRAMECAST
			#undef FRAMEVOL
			#undef FRAMECONVERT
			#undef FRAMECONVERTMONO
			#undef FRAMEOUT
			#undef FRAMEOUTD

			playingsource->currentFrame += src_data.input_frames_used; //update the current position time based on how many frames were converted;

			if (playingsource->currentFrame >= playingsource->endFrame) { //if we are finished playing.
				if (playingsource->loop) { //if we are to loop
					playingsource->currentFrame = playingsource->startFrame; //set the current frame to the beginning.
				} else { //otherwise
					m_stoppedAudioFiles.push_back(entry.first); //mark for removal.
				}
			}
		}

		delete [] framesOut;

		//removed stopped audio files.
		while (m_stoppedAudioFiles.size() > 0) {
			this->m_playingAudioSource.erase(m_stoppedAudioFiles.back());
			m_stoppedAudioFiles.pop_back();
		}

		m_callbackMutex.unlock(); //mutex unlock

		return paContinue; //continue playback
	}

	int PortAudioSystem::paCallbackCommon(const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void *userData) {
		//redirect to the individual callback. (gives us individualized and possibly multiple streams at the same time).
		return ((PortAudioSystem *)userData)->paCallback(inputBuffer, outputBuffer, framesPerBuffer, timeInfo, statusFlags, userData);
	}

	#ifdef PAS_CryGame
		void PortAudioSystem::GetMemoryUsage(ICrySizer* pSizer) const {
			pSizer->AddObject(this, sizeof(*this));
			//pSizer->AddObject(this->m_playingAudioSource);
		}
	#endif

	std::string PortAudioSystem::GetVersions() {
		std::string ret = "[PortAudioSystem] Port Audio Version: ";
		ret.append(Pa_GetVersionText());
		ret += "\n[PortAudioSystem] libsamplerate Version: ";
		ret.append(src_get_version());
		return ret;
	}
NSGameEnd