#ifndef _H_PORTAUDIOSYSTEMCOMMON
#define _H_PORTAUDIOSYSTEMCOMMON

#pragma once

#include <string>
#include <unordered_map>
#include <vector>

//we are compiling on CryEngine/Lumberyard
#ifdef eCryModule
	#ifndef NSGame //default to lumberyard namespace
		#define NSGameStart namespace LYGame {
		#define NSGameEnd }
	#else //otherwise define the game namespace
		#define NSGameStart namespace NSGame {
		#define NSGameEnd }
	#endif
	#define CLOG(...) CryLog(__VA_ARGS__)
	#define PAS_CryGame //use cryengine/lumberyard specific code
#else //otherwise, generic compiling
	#define NSGameStart
	#define NSGameEnd
	#define CLOG(...)
#endif

#ifndef BIT
	// Define BIT macro for use in enums and bit masks.
	#define BIT(x) (1 << (x))
	#define BIT64(x) (1ll << (x))
#endif

//build for a specific platform
#if defined(WIN32) || defined(__MINGW32__) || defined(__MINGW64__ ) || defined(BOOST_OS_WINDOWS)
	#define PSA_WIN
#elif defined(__linux__) || defined(__unix__ ) || defined(BOOST_OS_LINUX) || defined(BOOST_OS_UNIX)
	#define PSA_NIX
#elif defined(__APPLE__) || defined(BOOST_OS_MACOS)
	#define PSA_MAC
#endif

#define PAS_Millsecond(millisecond) (double)((double)millisecond/(double)1000.0f)

#ifndef STATIC_CONSTRUCTOR
	#define STATIC_CONSTRUCTOR
	NSGameStart
		//so that we can have a simple way of static constructors
		//http://stackoverflow.com/questions/1197106/static-constructors-in-c-i-need-to-initialize-private-static-objects
		template<void(*ctor)()>
		struct static_constructor
		{
			struct constructor { constructor() { ctor(); } };
			static constructor c;
		};

		template<void(*ctor)()>
		typename static_constructor<ctor>::constructor static_constructor<ctor>::c;
	NSGameEnd
#endif

//add in defines for playform specific stuff

#endif
