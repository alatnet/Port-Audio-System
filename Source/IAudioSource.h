#ifndef _H_IAUDIOSOURCE
#define _H_IAUDIOSOURCE

#pragma once

#include "../PortAudioSystemCommon.h"

#include "AudioSourceFactory.h"

NSGameStart
	//all the number of channels for a frame
	namespace AudioFrame {
		//mono channel
		struct af1 {
			float mono;

			af1() : mono(0.0f) {}
		};

		// 2 channel(left, right) - common
		struct af2 {
			float left;
			float right;

			af2() : left(0.0f), right(0.0f) {}
		};

		// 2.1 channel (left, right, subwoofer)
		struct af21 {
			float left;
			float right;
			float sub;
			af21() : left(0.0f), right(0.0f), sub(0.0f) {}
		};

		// 3 channel (left, right, center)
		struct af3 {
			float left;
			float right;
			float center;

			af3() : left(0.0f), right(0.0f), center(0.0f) {}
		};

		// 3.1 channel (left, right, center, subwoofer)
		struct af31 {
			float left;
			float right;
			float center;
			float sub;

			af31() : left(0.0f), right(0.0f), center(0.0f), sub(0.0f) {}
		};

		// 5 channel (left, right, center, back left, back right)
		struct af5 {
			union {
				struct {
					float left;
					float right;
				} front;
				struct {
					float left;
					float right;
				};
			};
			float center;
			union {
				struct {
					float left;
					float right;
				} back;
				struct {
					float bleft;
					float bright;
				};
			};

			af5() : left(0.0f), right(0.0f), center(0.0f), bleft(0.0f), bright(0.0f) {}
		};

		// 5.1 channel (left, right, center, back left, back right, subwoofer)
		struct af51 {
			union {
				struct {
					float left;
					float right;
				} front;
				struct {
					float left;
					float right;
				};
			};
			float center;
			union {
				struct {
					float left;
					float right;
				} back;
				struct {
					float bleft;
					float bright;
				};
			};
			float sub;

			af51() : left(0.0f), right(0.0f), center(0.0f), bleft(0.0f), bright(0.0f), sub(0.0f) {}
		};

		// 7 channel  (left, right, center, side left, side right, back left, back right)
		struct af7 {
			union {
				struct {
					float left;
					float right;
				} front;
				struct {
					float left;
					float right;
				};
			};
			float center;
			union {
				struct {
					float left;
					float right;
				} side;
				struct {
					float sleft;
					float sright;
				};
			};
			union {
				struct {
					float left;
					float right;
				} back;
				struct {
					float bleft;
					float bright;
				};
			};

			af7() : left(0.0f), right(0.0f), center(0.0f), bleft(0.0f), bright(0.0f), sleft(0.0f), sright(0.0f) {}
		};

		// 7.1 channel (left, right, center, side left, side right, back left, back right, subwoofer)
		struct af71 {
			union {
				struct {
					float left;
					float right;
				} front;
				struct {
					float left;
					float right;
				};
			};
			float center;
			union {
				struct {
					float left;
					float right;
				} side;
				struct {
					float sleft;
					float sright;
				};
			};
			union {
				struct {
					float left;
					float right;
				} back;
				struct {
					float bleft;
					float bright;
				};
			};
			float sub;

			af71() : left(0.0f), right(0.0f), center(0.0f), bleft(0.0f), bright(0.0f), sleft(0.0f), sright(0.0f), sub(0.0f) {}
		};

		//audio frame types - how many channels an audio frame has
		enum Type {
			eAFT_None = 0,	//no audio channels - can be an error indicator
			eAFT_Mono = 1,	// mono - one channel
			eAFT_2 = 2,		// 2 channels - left, right
			eAFT_21 = 3,	// 2.1 channels - left, right, subwoofer
			eAFT_3 = 3,		// 3 channels - left. right, center
			eAFT_31 = 4,	// 3.1 channels - left, right, center, subwoofer
			eAFT_5 = 5,		// 5 channels - left, right, center, back left, back right
			eAFT_51 = 6,	// 5.1 channels - left, right, center, back left, back right, subwoofer
			eAFT_7 = 7,		// 7 channels - left, right, center, side left, side right, back left, back right
			eAFT_71 = 8,	// 7.1 channels - left, right, center, side left, side right, back left, back right, subwoofer
			eAFT_Max = 8	//max audio channels - port audio (and most high end setups) have a max of 8 channels.
		};

		//channel conversion
		namespace Convert {
			//mono
			af1 ToMono(af2 in);
			af1 ToMono(af21 in);
			af1 ToMono(af3 in);
			af1 ToMono(af31 in);
			af1 ToMono(af5 in);
			af1 ToMono(af51 in);
			af1 ToMono(af7 in);
			af1 ToMono(af71 in);

			//2 ch
			af2 To2CH(af1 in);
			af2 To2CH(af21 in);
			af2 To2CH(af3 in);
			af2 To2CH(af31 in);
			af2 To2CH(af5 in);
			af2 To2CH(af51 in);
			af2 To2CH(af7 in);
			af2 To2CH(af71 in);

			//2.1 ch
			af21 To21CH(af1 in);
			af21 To21CH(af2 in);
			af21 To21CH(af3 in);
			af21 To21CH(af31 in);
			af21 To21CH(af5 in);
			af21 To21CH(af51 in);
			af21 To21CH(af7 in);
			af21 To21CH(af71 in);

			//3 ch
			af3 To3CH(af1 in);
			af3 To3CH(af2 in);
			af3 To3CH(af21 in);
			af3 To3CH(af31 in);
			af3 To3CH(af5 in);
			af3 To3CH(af51 in);
			af3 To3CH(af7 in);
			af3 To3CH(af71 in);

			//3.1 ch
			af31 To31CH(af1 in);
			af31 To31CH(af2 in);
			af31 To31CH(af21 in);
			af31 To31CH(af3 in);
			af31 To31CH(af5 in);
			af31 To31CH(af51 in);
			af31 To31CH(af7 in);
			af31 To31CH(af71 in);

			//5 ch
			af5 To5CH(af1 in);
			af5 To5CH(af2 in);
			af5 To5CH(af21 in);
			af5 To5CH(af3 in);
			af5 To5CH(af31 in);
			af5 To5CH(af51 in);
			af5 To5CH(af7 in);
			af5 To5CH(af71 in);

			//5.1 ch
			af51 To51CH(af1 in);
			af51 To51CH(af2 in);
			af51 To51CH(af21 in);
			af51 To51CH(af3 in);
			af51 To51CH(af31 in);
			af51 To51CH(af5 in);
			af51 To51CH(af7 in);
			af51 To51CH(af71 in);

			//7 ch
			af7 To7CH(af1 in);
			af7 To7CH(af2 in);
			af7 To7CH(af21 in);
			af7 To7CH(af3 in);
			af7 To7CH(af31 in);
			af7 To7CH(af5 in);
			af7 To7CH(af51 in);
			af7 To7CH(af71 in);

			//7.1 ch
			af71 To71CH(af1 in);
			af71 To71CH(af2 in);
			af71 To71CH(af21 in);
			af71 To71CH(af3 in);
			af71 To71CH(af31 in);
			af71 To71CH(af5 in);
			af71 To71CH(af51 in);
			af71 To71CH(af7 in);
		}
	}

	struct AudioSourceTime {
		int hrs, min;
		double sec, totalSec;

		AudioSourceTime() :
			hrs(0),
			min(0),
			sec(0.0f),
			totalSec(0.0f)
		{};
	};

	struct AudioSourceError {
		int code;
		const char * str;

		AudioSourceError() :
			code(0),
			str("NoError") {
		};
	};

	enum EAudioSourceFlags {
		eAF_None = 0,
		eAF_Loop = BIT(0),
		eAF_LoopSection = BIT(1), //to add support
		eAF_PausedOnStart = BIT(2)
	};

	class IAudioSource {
	public:
		//create a new audio source based on filename.
		IAudioSource(const char * filename) {};
	protected:
		//Note to developers: Call this constructor to setup basic functionality.
		IAudioSource();
	public:
		//seek to a position on the audio source (in frames)
		virtual bool Seek(long long position) = 0;
		/*
		get the frames of the audio source
		params:
		- long long framesToRead - number of frames to read
		- float * buff - pointer to a buffer to where the frames are transfered to.
		returns:
		- long long - number of frames read.
		*/
		virtual long long GetFrames(long long framesToRead, float* buff) = 0;
		/*
		get a single frame of the audio source
		params:
		- float * frame - pointer to a bufferr to where the frame is transfered to.
		returns:
		- bool - whether the frame was read or not.
		*/
		virtual bool GetFrame(float* frame) = 0;

		//returns the sample rate of the audio source.  Common sample rates: 44100, 48000, 192000
		virtual double GetSampleRate() = 0;
		//returns the audio frame type (how many channels the audio source has).
		virtual AudioFrame::Type GetFrameType() = 0;

		//returns the length of the audio source in a human readable format.
		virtual AudioSourceTime GetLength() = 0;
		//returns the length of the audio source in frames.
		virtual long long GetFrameLength() = 0;

		/*
		set the flags for the audio source.
		params:
		- int - flags to set (usually ored "|" together).
		See: EAudioSourceFlags
		*/
		virtual void SetFlags(int flags) { this->m_flags = flags; }
		//returns the flags set for the audio source.
		virtual int GetFlags() { return this->m_flags; }

		#ifdef PAS_CryGame
			virtual void GetMemoryUsage(ICrySizer* pSizer) const = 0;
		#endif
	protected:
		int m_flags;
	public: //error checking
		//returns if the audio source has an error.
		bool HasError() { return this->m_hasError; }
		/*
		returns what the error is in a human readable format.
		automatically clears error when there are no more errors to retrieve.
		*/
		AudioSourceError GetError();
	protected:
		//if the audio source has an error, this variable will be set.
		bool m_hasError;
		/*
		push an error to the list of errors.
		automatically sets the m_hasError variable.
		params:
		- int errorCode - error code for the specific audio source (library dependent).
		- const char * errorStr - human readable format for the error code.
		*/
		void pushError(int errorCode, const char * errorStr);
	private:
		std::vector<AudioSourceError> m_errors;
	};
NSGameEnd

#endif
