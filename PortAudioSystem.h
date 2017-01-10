#ifndef _H_PORTAUDIOSYSTEM
#define _H_PORTAUDIOSYSTEM

#pragma once

#include "PortAudioSystemCommon.h"

//std mutex support
#include <mutex>

//for sample rate conversion
//http://www.mega-nerd.com/SRC/index.html
#include <samplerate.h>

//port audio system
#include <portaudio.h>
#ifdef PAS_WIN
	#include <pa_win_wmme.h>
	#include <pa_win_ds.h>
	#include <pa_win_wdmks.h>
	#include <pa_asio.h>
	#include <pa_win_wasapi.h>
	#include <pa_win_waveformat.h>
#elif PAS_MAC
	#include <pa_mac_core.h>
	#include <pa_jack.h>
#elif PAS_NIX
	#include <pa_linux_alsa.h>
	#include <pa_jack.h>
#endif

#include "Source\IAudioSource.h"

NSGameStart
	struct PASError {
		int code;
		const char * str;

		PASError() :
			code(0),
			str("NoError")
		{};
	};

	//want to add in more audio sections to adjust volume on?
	//create a new enum here.
	enum EAudioSection {
		eAS_Music,
		eAS_SFX,
		eAS_Voice,
		eAS_Environment,
		eAS_UI,
		eAS_Count
	};

	/*enum EAudioPlayError {
		eAPE_None
	};*/

	class PortAudioSystem {
	public:
		/*
		Create a new port audio system.
		params:
		- double sampleRate - the sample rate to use (defaults to 44100). common sample rates: 44100, 48000, 192000
		- PaDeviceIndex device - which port audio device to use (defaults to Pa_GetDefaultOutputDevice());
		- void * hostApiSpecificStreamInfo - host api specific stream info (defaults to 0/NULL/nullptr);

		NOTE! this is subjected to change.
		NOTE! resampling the audio sample rate currently has crackling sound issues.  Best practice is to use a sample rate that is the same for both port audio system AND the audio sources you are using.
		*/
		PortAudioSystem(double sampleRate = 44100, PaDeviceIndex device = Pa_GetDefaultOutputDevice(), void * hostApiSpecificStreamInfo = 0);
		~PortAudioSystem();
	public: //basic functionallity
		//start playback
		void Play();
		//pause playback
		void Pause();
		//stop playback (clears audio sources)
		void Stop();
		//void StopReset();
		//retuns if port audio system is playing.
		bool IsPlaying() { return (this->m_pAudioStream != nullptr) ? (Pa_IsStreamActive(this->m_pAudioStream) == 1) : false; }
	public:
		//gets the versions used for port audio system.
		static std::string GetVersions();
		//gets the delta time between buffering.
		float GetDeltaTime();
	public: //audio source functionality
		/*
		plays an audio source (automatically removed from port audio system when done playing, does not delete source)
		params:
		- IAudioSource * source - pointer to the source to play.
		- EAudioSection section - which audio volume section to play on.
		returns:
		- long - an id to the audio source that is added to port audio system.
		*/
		long PlaySource(IAudioSource * source, EAudioSection section);
		/*
		pauses an audio source.
		params:
		- long id - the id that was returned from PlaySource.
		*/
		void PauseSource(long id);
		/*
		resumes a paused an audio source.
		params:
		- long id - the id that was returned from PlaySource.
		*/
		void ResumeSource(long id);
		/*
		stops and removes an audio source from port audio system.
		params:
		- long id - the id that was returned from PlaySource.
		*/
		void StopSource(long id);
		/*
		returns if the audio source is playing.
		params:
		- long id - the id that was returned from PlaySource.
		returns:
		- bool - whether if the audio source is playing or not.
		*/
		bool IsPlaying(long id);
		/*
		gets the time of the audio source in a human readable format.
		params:
		- long id - the id that was returned from PlaySource.
		returns:
		- AudioSourceTime - the time of the playing file in a human readable format (or zeroed out time if the source cannot be found or isnt playing).
		*/
		AudioSourceTime GetTime(long id);
		/*
		sets the audio source to the specific time.
		params:
		- long id - the id that was returned from PlaySource.
		- double time - the time to set the audio source in seconds.
		*/
		void SetTime(long id, double time);
	public: //volume functions
		/*
		set the master volume.
		params:
		- float vol - level to set inbetween 0.0f and 1.0f.
		*/
		void SetMasterVol(float vol);
		/*
		returns the volume level of the master volume.
		returns:
		- float - volume level between 0.0f and 1.0f.
		*/
		float GetMasterVol() { return this->m_masterVol; }
		/*
		set a specific audio section volume.
		params:
		- float vol - level to set inbetween 0.0f and 1.0f.
		- EAudioSection section - which section to set the volume to.
		*/
		void SetVolume(float vol, EAudioSection section);
		/*
		returns the volume level of a specific section.
		params:
		- EAudioSection section - which section to get the volume level.
		returns:
		- float - volume level between 0.0f and 1.0f.
		*/
		float GetVolume(EAudioSection section);
		/*
		set a specific audio source volume.
		params:
		- float vol - level to set inbetween 0.0f and 1.0f.
		- long id - the id that was returned from PlaySource.
		*/
		void SetVolume(float vol, long id);
		/*
		returns the volume level of a specific source.
		params:
		- long id - the id that was returned from PlaySource.
		returns:
		- float - volume level between 0.0f and 1.0f.
		*/
		float GetVolume(long id);
	public: //error checking
		//returns if port audio system has an error.
		bool HasError();
		/*
		returns what the error is in a human readable format.
		automatically clears error when there are no more errors to retrieve.
		*/
		PASError GetError();
	#ifdef PAS_CryGame
		public: //lumberyard stuff
			void GetMemoryUsage(ICrySizer* pSizer) const;
	#endif
	private:
		struct PlayingAudioSource {
			IAudioSource* audioSource;
			long long currentFrame;
			long long startFrame;
			long long endFrame;
			bool loop;
			bool paused;
			EAudioSection section;
			float vol;

			PlayingAudioSource() :
				audioSource(nullptr),
				currentFrame(0),
				startFrame(0),
				endFrame(0),
				loop(false),
				paused(false),
				section(eAS_Music),
				vol(1.0f)
			{}

			#ifdef PAS_CryGame
				void GetMemoryUsage(ICrySizer* pSizer) const {
					//pSizer->AddObject(this, sizeof(*this));
					pSizer->AddObject(audioSource);
				}
			#endif
		};
	private:
		typedef std::unordered_map<long, PlayingAudioSource *> PlayingAudioSourcesMap;
		PlayingAudioSourcesMap m_playingAudioSource;

		long m_nextPlayID;

		std::vector<long> m_stoppedAudioFiles;
	private:
		float m_deltaTime;
		float m_masterVol;// , m_musicVol, m_sfxVol;
		float m_vols[eAS_Count];
		int m_numChannels;
	private: //port audio stuff
		PaStream *m_pAudioStream;
	private: //samplerate conversion
		double m_sampleRate;
		SRC_STATE * m_pSrcState;
	private: //error checking
		bool m_hasError;
		std::vector<PASError> m_errors;
		void pushError(int errorCode, const char * errorStr);
	protected:
		int paCallback(const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void *userData);
	public:
		static int paCallbackCommon(const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void *userData);
	private: //mutexes
		std::mutex m_callbackMutex, m_deltaTimeMutex, m_errorMutex;
	};
NSGameEnd

#endif
