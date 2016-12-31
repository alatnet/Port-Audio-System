#ifndef _H_AUDIOSOURCEFACTORY
#define _H_AUDIOSOURCEFACTORY
#pragma once

NSGameStart
	class IAudioSource;

	typedef std::function<IAudioSource* (const char *)> NewAudioSourceFunc;

	class AudioSourceFactory {
	public:
		//get the audio source factory
		static AudioSourceFactory& getFactory();
	public:
		/*
		create a new audio source based on the name when registered. (usually library name)
		params:
		- std::string name - name of the audio source to use (usually library name)
		- const char * path - path to the file to use
		returns:
		- an IAudioSource pointer or nullptr for an invalid name.

		NOTE! Factory does not handle deletion of audio sources!
		*/
		IAudioSource* newAudioSource(std::string name, const char * path);

		//get the list of audio sources. (usually library names)
		std::vector<std::string>& getAudioSources() { return *(this->m_names); }
	public:
		void registerAudioSource(std::string name, NewAudioSourceFunc ptr);
	private:
		AudioSourceFactory();
		~AudioSourceFactory();
		AudioSourceFactory(const AudioSourceFactory&) = delete;
		AudioSourceFactory& operator= (const AudioSourceFactory&) = delete;
	private:
		std::unordered_map<std::string, NewAudioSourceFunc> *m_funcs;
		std::vector<std::string> *m_names;
	};
	
	//register an audio source to the factory. (name is usually the library name)
	#define REGISTER_AUDIO_SOURCE(name,entry) \
		public: \
			static IAudioSource* new##entry##FactoryReg(const char * path) { return new entry##(path); }\
			static void Register##entry##ToFactory() {\
				static_constructor<&##entry::Register##entry##ToFactory>::c;\
				AudioSourceFactory::getFactory().registerAudioSource(##name##, &##entry##::new##entry##FactoryReg);\
			}
NSGameEnd

#endif