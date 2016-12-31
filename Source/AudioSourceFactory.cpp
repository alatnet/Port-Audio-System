#include <StdAfx.h>

#include "../PortAudioSystemCommon.h"

#include "IAudioSource.h"
#include <functional>

#include "AudioSourceFactory.h"

NSGameStart

AudioSourceFactory::AudioSourceFactory() {
	this->m_funcs = new std::unordered_map<std::string, NewAudioSourceFunc>();
	this->m_names = new std::vector<std::string>();
}

AudioSourceFactory::~AudioSourceFactory() {
	this->m_funcs->clear();
	delete this->m_funcs;

	this->m_names->clear();
	delete this->m_names;
}


IAudioSource* AudioSourceFactory::newAudioSource(std::string name, const char * path) {
	auto funcEntry = this->m_funcs->find(name);
	if (funcEntry != this->m_funcs->end()) return funcEntry->second(path);
	return nullptr;
}

void AudioSourceFactory::registerAudioSource(std::string name, NewAudioSourceFunc ptr) {
	this->m_funcs->insert({ name, ptr });
	this->m_names->push_back(name);
}

AudioSourceFactory& AudioSourceFactory::getFactory() {
	static AudioSourceFactory theInstance;
	return theInstance;
}

NSGameEnd