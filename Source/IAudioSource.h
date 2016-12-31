#ifndef _H_IAUDIOSOURCE
#define _H_IAUDIOSOURCE

#pragma once

#include "../PortAudioSystemCommon.h"

#include "AudioSourceFactory.h"

NSGameStart
	//all the number of channels for a frame
	namespace AudioFrame {
		//mono channel
		struct _1 {
			float mono;

			_1() : mono(0.0f) {}
		};

		// 2 channel(left, right) - common
		struct _2 {
			float left;
			float right;

			_2() : left(0.0f), right(0.0f) {}
		};

		// 2.1 channel (left, right, subwoofer)
		struct _21 {
			float left;
			float right;
			float sub;
			_21() : left(0.0f), right(0.0f), sub(0.0f) {}
		};

		// 3 channel (left, right, center)
		struct _3 {
			float left;
			float right;
			float center;

			_3() : left(0.0f), right(0.0f), center(0.0f) {}
		};

		// 3.1 channel (left, right, center, subwoofer)
		struct _31 {
			float left;
			float right;
			float center;
			float sub;

			_31() : left(0.0f), right(0.0f), center(0.0f), sub(0.0f) {}
		};

		// 5 channel (left, right, center, back left, back right)
		struct _5 {
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

			_5() : left(0.0f), right(0.0f), center(0.0f), bleft(0.0f), bright(0.0f) {}
		};

		// 5.1 channel (left, right, center, back left, back right, subwoofer)
		struct _51 {
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

			_51() : left(0.0f), right(0.0f), center(0.0f), bleft(0.0f), bright(0.0f), sub(0.0f) {}
		};

		// 7 channel  (left, right, center, side left, side right, back left, back right)
		struct _7 {
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

			_7() : left(0.0f), right(0.0f), center(0.0f), bleft(0.0f), bright(0.0f), sleft(0.0f), sright(0.0f) {}
		};

		// 7.1 channel (left, right, center, side left, side right, back left, back right, subwoofer)
		struct _71 {
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

			_71() : left(0.0f), right(0.0f), center(0.0f), bleft(0.0f), bright(0.0f), sleft(0.0f), sright(0.0f), sub(0.0f) {}
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
			_1 ToMono(_2 in);
			_1 ToMono(_21 in);
			_1 ToMono(_3 in);
			_1 ToMono(_31 in);
			_1 ToMono(_5 in);
			_1 ToMono(_51 in);
			_1 ToMono(_7 in);
			_1 ToMono(_71 in);

			//2 ch
			_2 To2CH(_1 in);
			_2 To2CH(_21 in);
			_2 To2CH(_3 in);
			_2 To2CH(_31 in);
			_2 To2CH(_5 in);
			_2 To2CH(_51 in);
			_2 To2CH(_7 in);
			_2 To2CH(_71 in);

			//2.1 ch
			_21 To21CH(_1 in);
			_21 To21CH(_2 in);
			_21 To21CH(_3 in);
			_21 To21CH(_31 in);
			_21 To21CH(_5 in);
			_21 To21CH(_51 in);
			_21 To21CH(_7 in);
			_21 To21CH(_71 in);

			//3 ch
			_3 To3CH(_1 in);
			_3 To3CH(_2 in);
			_3 To3CH(_21 in);
			_3 To3CH(_31 in);
			_3 To3CH(_5 in);
			_3 To3CH(_51 in);
			_3 To3CH(_7 in);
			_3 To3CH(_71 in);

			//3.1 ch
			_31 To31CH(_1 in);
			_31 To31CH(_2 in);
			_31 To31CH(_21 in);
			_31 To31CH(_3 in);
			_31 To31CH(_5 in);
			_31 To31CH(_51 in);
			_31 To31CH(_7 in);
			_31 To31CH(_71 in);

			//5 ch
			_5 To5CH(_1 in);
			_5 To5CH(_2 in);
			_5 To5CH(_21 in);
			_5 To5CH(_3 in);
			_5 To5CH(_31 in);
			_5 To5CH(_51 in);
			_5 To5CH(_7 in);
			_5 To5CH(_71 in);

			//5.1 ch
			_51 To51CH(_1 in);
			_51 To51CH(_2 in);
			_51 To51CH(_21 in);
			_51 To51CH(_3 in);
			_51 To51CH(_31 in);
			_51 To51CH(_5 in);
			_51 To51CH(_7 in);
			_51 To51CH(_71 in);

			//7 ch
			_7 To7CH(_1 in);
			_7 To7CH(_2 in);
			_7 To7CH(_21 in);
			_7 To7CH(_3 in);
			_7 To7CH(_31 in);
			_7 To7CH(_5 in);
			_7 To7CH(_51 in);
			_7 To7CH(_71 in);

			//7.1 ch
			_71 To71CH(_1 in);
			_71 To71CH(_2 in);
			_71 To71CH(_21 in);
			_71 To71CH(_3 in);
			_71 To71CH(_31 in);
			_71 To71CH(_5 in);
			_71 To71CH(_51 in);
			_71 To71CH(_7 in);
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
