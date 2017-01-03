#include <StdAfx.h>
#include "IAudioSource.h"

NSGameStart
	namespace AudioFrame {
		namespace Convert {
			//mono
			af1 ToMono(af2 in) {
				af1 ret;
				ret.mono = CLAMP(in.left + in.right,-1.0f,1.0f);
				return ret;
			}
			af1 ToMono(af21 in) {
				af1 ret;
				ret.mono = CLAMP(in.left + in.right + in.sub, -1.0f, 1.0f);
				return ret;
			}
			af1 ToMono(af3 in) {
				af1 ret;
				ret.mono = CLAMP(in.left + in.right + in.center, -1.0f, 1.0f);
				return ret;
			}
			af1 ToMono(af31 in) {
				af1 ret;
				ret.mono = CLAMP(in.left + in.right + in.center + in.sub, -1.0f, 1.0f);
				return ret;
			}
			af1 ToMono(af5 in) {
				af1 ret;
				ret.mono = CLAMP(in.front.left + in.front.right + in.center + in.back.left + in.back.right, -1.0f, 1.0f);
				return ret;
			}
			af1 ToMono(af51 in) {
				af1 ret;
				ret.mono = CLAMP(in.front.left + in.front.right + in.center + in.back.left + in.back.right + in.sub, -1.0f, 1.0f);
				return ret;
			}
			af1 ToMono(af7 in) {
				af1 ret;
				ret.mono = CLAMP(in.front.left + in.front.right + in.center + in.side.left + in.side.right + in.back.left + in.back.right, -1.0f, 1.0f);
				return ret;
			}
			af1 ToMono(af71 in) {
				af1 ret;
				ret.mono = CLAMP(in.front.left + in.front.right + in.center + in.side.left + in.side.right + in.back.left + in.back.right + in.sub, -1.0f, 1.0f);
				return ret;
			}

			//2 ch
			af2 To2CH(af1 in) {
				af2 ret;
				ret.left = in.mono;
				ret.right = in.mono;
				return ret;
			}
			af2 To2CH(af21 in) {
				af2 ret;
				ret.left = CLAMP(in.left + (in.sub / 2), -1.0f, 1.0f);
				ret.right = CLAMP(in.right + (in.sub / 2), -1.0f, 1.0f);
				return ret;
			}
			af2 To2CH(af3 in) {
				af2 ret;
				ret.left = CLAMP(in.left + (in.center / 2), -1.0f, 1.0f);
				ret.right = CLAMP(in.right + (in.center / 2), -1.0f, 1.0f);
				return ret;
			}
			af2 To2CH(af31 in) {
				af2 ret;
				ret.left = CLAMP(in.left + (in.center / 2) + (in.sub / 2), -1.0f, 1.0f);
				ret.right = CLAMP(in.right + (in.center / 2) + (in.sub / 2), -1.0f, 1.0f);
				return ret;
			}
			af2 To2CH(af5 in) {
				af2 ret;
				ret.left = CLAMP(in.front.left + (in.center / 2) + (in.back.left / 2), -1.0f, 1.0f);
				ret.right = CLAMP(in.front.right + (in.center / 2) + (in.back.right / 2), -1.0f, 1.0f);
				return ret;
			}
			af2 To2CH(af51 in) {
				af2 ret;
				ret.left = CLAMP(in.front.left + (in.center / 2) + (in.back.left / 2) + (in.sub / 2), -1.0f, 1.0f);
				ret.right = CLAMP(in.front.right + (in.center / 2) + (in.back.right / 2) + (in.sub / 2), -1.0f, 1.0f);
				return ret;
			}
			af2 To2CH(af7 in) {
				af2 ret;
				ret.left = CLAMP(in.front.left + (in.center / 2) + (in.side.left / 2) + (in.back.left / 3), -1.0f, 1.0f);
				ret.right = CLAMP(in.front.right + (in.center / 2) + (in.side.right / 2) + (in.back.right / 3), -1.0f, 1.0f);
				return ret;
			}
			af2 To2CH(af71 in) {
				af2 ret;
				ret.left = CLAMP(in.front.left + (in.center / 2) + (in.side.left / 2) + (in.back.left / 3) + (in.sub / 2), -1.0f, 1.0f);
				ret.right = CLAMP(in.front.right + (in.center / 2) + (in.side.right / 2) + (in.back.right / 3) + (in.sub / 2), -1.0f, 1.0f);
				return ret;
			}

			//2.1 ch
			af21 To21CH(af1 in) {
				af21 ret;
				ret.left = in.mono;
				ret.right = in.mono;
				ret.sub = in.mono;
				return ret;
			}
			af21 To21CH(af2 in) {
				af21 ret;
				ret.left = in.left;
				ret.right = in.right;
				ret.sub = CLAMP(in.left + in.right, -1.0f, 1.0f);
				return ret;
			}
			af21 To21CH(af3 in) {
				af21 ret;
				ret.left = CLAMP(in.left + (in.center / 2), -1.0f, 1.0f);
				ret.right = CLAMP(in.right + (in.center / 2), -1.0f, 1.0f);
				ret.sub = CLAMP(in.left + in.right + in.center, -1.0f, 1.0f);
				return ret;
			}
			af21 To21CH(af31 in) {
				af21 ret;
				ret.left = CLAMP(in.left + (in.center / 2), -1.0f, 1.0f);
				ret.right = CLAMP(in.right + (in.center / 2), -1.0f, 1.0f);
				ret.sub = in.sub;
				return ret;
			}
			af21 To21CH(af5 in) {
				af21 ret;
				ret.left = CLAMP(in.front.left + (in.center / 2) + (in.back.left / 2), -1.0f, 1.0f);
				ret.right = CLAMP(in.front.right + (in.center / 2) + (in.back.right / 2), -1.0f, 1.0f);
				ret.sub = CLAMP(in.front.left + in.front.right + in.center + (in.back.left / 2) + (in.back.right / 2), -1.0f, 1.0f);
				return ret;
			}
			af21 To21CH(af51 in) {
				af21 ret;
				ret.left = CLAMP(in.front.left + (in.center / 2) + (in.back.left / 2), -1.0f, 1.0f);
				ret.right = CLAMP(in.front.right + (in.center / 2) + (in.back.right / 2), -1.0f, 1.0f);
				ret.sub = in.sub;
				return ret;
			}
			af21 To21CH(af7 in) {
				af21 ret;
				ret.left = CLAMP(in.front.left + (in.center / 2) + (in.side.left / 2) + (in.back.left / 3), -1.0f, 1.0f);
				ret.right = CLAMP(in.front.right + (in.center / 2) + (in.side.right / 2) + (in.back.right / 3), -1.0f, 1.0f);
				ret.sub = (in.front.left + in.front.right + in.center + (in.side.left / 2) + (in.side.right / 2) + (in.back.left / 3) + (in.back.right / 3), -1.0f, 1.0f);
				return ret;
			}
			af21 To21CH(af71 in) {
				af21 ret;
				ret.left = CLAMP(in.front.left + (in.center / 2) + (in.side.left / 2) + (in.back.left / 3), -1.0f, 1.0f);
				ret.right = CLAMP(in.front.right + (in.center / 2) + (in.side.right / 2) + (in.back.right / 3), -1.0f, 1.0f);
				ret.sub = in.sub;
				return ret;
			}

			//3 ch
			af3 To3CH(af1 in) {
				af3 ret;
				ret.left = in.mono;
				ret.right = in.mono;
				ret.center = in.mono;
				return ret;
			}
			af3 To3CH(af2 in) {
				af3 ret;
				ret.left = in.left;
				ret.right = in.right;
				ret.center = CLAMP(in.left + in.right, -1.0f, 1.0f);
				return ret;
			}
			af3 To3CH(af21 in) {
				af3 ret;
				ret.left = CLAMP(in.left + (in.sub / 2), -1.0f, 1.0f);
				ret.right = CLAMP(in.right + (in.sub / 2), -1.0f, 1.0f);
				ret.center = CLAMP(in.right + in.left + (in.sub / 2), -1.0f, 1.0f);
				return ret;
			}
			af3 To3CH(af31 in) {
				af3 ret;
				ret.left = CLAMP(in.left + (in.sub / 2), -1.0f, 1.0f);
				ret.right = CLAMP(in.right + (in.sub / 2), -1.0f, 1.0f);
				ret.center = CLAMP(in.center + (in.sub / 2), -1.0f, 1.0f);
				return ret;
			}
			af3 To3CH(af5 in) {
				af3 ret;
				ret.left = CLAMP(in.front.left + (in.back.left / 2), -1.0f, 1.0f);
				ret.right = CLAMP(in.front.right + (in.back.right / 2), -1.0f, 1.0f);
				ret.center = in.center;
				return ret;
			}
			af3 To3CH(af51 in) {
				af3 ret;
				ret.left = CLAMP(in.front.left + (in.back.left / 2) + (in.sub / 2), -1.0f, 1.0f);
				ret.right = CLAMP(in.front.right + (in.back.right / 2) + (in.sub / 2), -1.0f, 1.0f);
				ret.center = CLAMP(in.center + (in.sub / 2), -1.0f, 1.0f) / 2;
				return ret;
			}
			af3 To3CH(af7 in) {
				af3 ret;
				ret.left = CLAMP(in.front.left + (in.side.left / 2) + (in.back.left / 3), -1.0f, 1.0f);
				ret.right = CLAMP(in.front.right + (in.side.right / 2) + (in.back.right / 3), -1.0f, 1.0f);
				ret.center = in.center;
				return ret;
			}
			af3 To3CH(af71 in) {
				af3 ret;
				ret.left = CLAMP(in.front.left + (in.side.left / 2) + (in.back.left / 3) + (in.sub / 2), -1.0f, 1.0f);
				ret.right = CLAMP(in.front.right + (in.side.right / 2) + (in.back.right / 3) + (in.sub / 2), -1.0f, 1.0f);
				ret.center = CLAMP(in.center + (in.sub / 2), -1.0f, 1.0f);
				return ret;
			}

			//3.1 ch
			af31 To31CH(af1 in) {
				af31 ret;
				ret.left = in.mono;
				ret.right = in.mono;
				ret.center = in.mono;
				ret.sub = in.mono;
				return ret;
			}
			af31 To31CH(af2 in) {
				af31 ret;
				ret.left = in.left;
				ret.right = in.right;
				ret.center = CLAMP(in.left + in.right, -1.0f, 1.0f);
				ret.sub = CLAMP(in.left + in.right, -1.0f, 1.0f);
				return ret;
			}
			af31 To31CH(af21 in) {
				af31 ret;
				ret.left = in.left;
				ret.right = in.right;
				ret.center = CLAMP(in.right + in.left, -1.0f, 1.0f);
				ret.sub = in.sub;
				return ret;
			}
			af31 To31CH(af3 in) {
				af31 ret;
				ret.left = in.left;
				ret.right = in.right;
				ret.center = in.center;
				ret.sub = CLAMP(in.left + in.right + in.center, -1.0f, 1.0f);
				return ret;
			}
			af31 To31CH(af5 in) {
				af31 ret;
				ret.left = CLAMP(in.front.left + (in.back.left / 2), -1.0f, 1.0f);
				ret.right = CLAMP(in.front.right + (in.back.right / 2), -1.0f, 1.0f);
				ret.center = in.center;
				ret.sub = CLAMP(in.front.left + in.front.right + in.center + (in.back.left / 2) + (in.back.right / 2), -1.0f, 1.0f);
				return ret;
			}
			af31 To31CH(af51 in) {
				af31 ret;
				ret.left = CLAMP(in.front.left + (in.back.left / 2), -1.0f, 1.0f);
				ret.right = CLAMP(in.front.right + (in.back.right / 2), -1.0f, 1.0f);
				ret.center = in.center;
				ret.sub = in.sub;
				return ret;
			}
			af31 To31CH(af7 in) {
				af31 ret;
				ret.left = CLAMP(in.front.left + (in.side.left / 2) + (in.back.left / 3), -1.0f, 1.0f);
				ret.right = CLAMP(in.front.right + (in.side.right / 2) + (in.back.right / 3), -1.0f, 1.0f);
				ret.center = in.center;
				ret.sub = CLAMP(in.front.left + in.front.right + in.center + (in.side.left / 2) + (in.side.right / 2) + (in.back.left / 3) + (in.back.right / 3), -1.0f, 1.0f);
				return ret;
			}
			af31 To31CH(af71 in) {
				af31 ret;
				ret.left = CLAMP(in.front.left + (in.side.left / 2) + (in.back.left / 3), -1.0f, 1.0f);
				ret.right = CLAMP(in.front.right + (in.side.right / 2) + (in.back.right / 3), -1.0f, 1.0f);
				ret.center = in.center;
				ret.sub = in.sub;
				return ret;
			}

			//5 ch
			af5 To5CH(af1 in) {
				af5 ret;
				ret.front.left = in.mono;
				ret.front.right = in.mono;
				ret.center = in.mono;
				ret.back.left = in.mono / 2;
				ret.back.right = in.mono / 2;
				return ret;
			}
			af5 To5CH(af2 in) {
				af5 ret;
				ret.front.left = in.left;
				ret.front.right = in.right;
				ret.center = CLAMP(in.left + in.right, -1.0f, 1.0f);
				ret.back.left = in.left / 2;
				ret.back.right = in.right / 2;
				return ret;
			}
			af5 To5CH(af21 in) {
				af5 ret;
				ret.front.left = CLAMP(in.left + (in.sub / 2), -1.0f, 1.0f);
				ret.front.right = CLAMP(in.right + (in.sub / 2), -1.0f, 1.0f);
				ret.center = CLAMP(in.right + in.left + (in.sub / 2), -1.0f, 1.0f);
				ret.back.left = CLAMP((in.left / 2) + (in.sub / 2), -1.0f, 1.0f);
				ret.back.right = CLAMP((in.right / 2) + (in.sub / 2), -1.0f, 1.0f);
				return ret;
			}
			af5 To5CH(af3 in) {
				af5 ret;
				ret.front.left = in.left;
				ret.front.right = in.right;
				ret.center = in.center;
				ret.back.left = in.left / 2;
				ret.back.right = in.right / 2;
				return ret;
			}
			af5 To5CH(af31 in) {
				af5 ret;
				ret.front.left = CLAMP(in.left + (in.sub / 2), -1.0f, 1.0f);
				ret.front.right = CLAMP(in.right + (in.sub / 2), -1.0f, 1.0f);
				ret.center = CLAMP(in.center + (in.sub / 2), -1.0f, 1.0f);
				ret.back.left = CLAMP((in.left / 2) + (in.sub / 2), -1.0f, 1.0f);
				ret.back.right = CLAMP((in.right / 2) + (in.sub / 2), -1.0f, 1.0f);
				return ret;
			}
			af5 To5CH(af51 in) {
				af5 ret;
				ret.front.left = CLAMP(in.front.left + (in.sub / 2), -1.0f, 1.0f);
				ret.front.right = CLAMP(in.front.right + (in.sub / 2), -1.0f, 1.0f);
				ret.center = CLAMP(in.center + (in.sub / 2), -1.0f, 1.0f);
				ret.back.left = CLAMP(in.back.left + (in.sub / 2), -1.0f, 1.0f);
				ret.back.right = CLAMP(in.back.right + (in.sub / 2), -1.0f, 1.0f);
				return ret;
			}
			af5 To5CH(af7 in) {
				af5 ret;
				ret.front.left = CLAMP(in.front.left + (in.side.left / 2), -1.0f, 1.0f);
				ret.front.right = CLAMP(in.front.right + (in.side.right / 2), -1.0f, 1.0f);
				ret.center = in.center;
				ret.back.left = CLAMP(in.back.left + (in.side.left / 2), -1.0f, 1.0f);
				ret.back.right = CLAMP(in.back.right + (in.side.left / 2), -1.0f, 1.0f);
				return ret;
			}
			af5 To5CH(af71 in) {
				af5 ret;
				ret.front.left = CLAMP(in.front.left + (in.side.left / 2) + (in.sub / 2), -1.0f, 1.0f);
				ret.front.right = CLAMP(in.front.right + (in.side.right / 2) + (in.sub / 2), -1.0f, 1.0f);
				ret.center = CLAMP(in.center + (in.sub / 2), -1.0f, 1.0f);
				ret.back.left = CLAMP(in.back.left + (in.side.left / 2) + (in.sub / 2), -1.0f, 1.0f);
				ret.back.right = CLAMP(in.back.right + (in.side.left / 2) + (in.sub / 2), -1.0f, 1.0f);
				return ret;
			}

			//5.1 ch
			af51 To51CH(af1 in) {
				af51 ret;
				ret.front.left = in.mono;
				ret.front.right = in.mono;
				ret.center = in.mono;
				ret.back.left = in.mono / 2;
				ret.back.right = in.mono / 2;
				ret.sub = in.mono;
				return ret;
			}
			af51 To51CH(af2 in) {
				af51 ret;
				ret.front.left = in.left;
				ret.front.right = in.right;
				ret.center = CLAMP(in.left + in.right, -1.0f, 1.0f);
				ret.back.left = in.left / 2;
				ret.back.right = in.right / 2;
				ret.sub = CLAMP(in.left + in.right, -1.0f, 1.0f);
				return ret;
			}
			af51 To51CH(af21 in) {
				af51 ret;
				ret.front.left = in.left;
				ret.front.right = in.right;
				ret.center = CLAMP(in.left + in.right, -1.0f, 1.0f);
				ret.back.left = in.left / 2;
				ret.back.right = in.right / 2;
				ret.sub = in.sub;
				return ret;
			}
			af51 To51CH(af3 in) {
				af51 ret;
				ret.front.left = in.left;
				ret.front.right = in.right;
				ret.center = in.center;
				ret.back.left = in.left / 2;
				ret.back.right = in.right / 2;
				ret.sub = CLAMP(in.left + in.right + in.center, -1.0f, 1.0f);
				return ret;
			}
			af51 To51CH(af31 in) {
				af51 ret;
				ret.front.left = in.left;
				ret.front.right = in.right;
				ret.center = in.center;
				ret.back.left = in.left / 2;
				ret.back.right = in.right / 2;
				ret.sub = in.sub;
				return ret;
			}
			af51 To51CH(af5 in) {
				af51 ret;
				ret.front.left = in.front.left;
				ret.front.right = in.front.right;
				ret.center = in.center;
				ret.back.left = in.back.left;
				ret.back.right = in.back.right;
				ret.sub = CLAMP(in.front.left + in.front.right + in.center + in.back.left + in.back.right, -1.0f, 1.0f);
				return ret;
			}
			af51 To51CH(af7 in) {
				af51 ret;
				ret.front.left = CLAMP(in.front.left + (in.side.left / 2), -1.0f, 1.0f);
				ret.front.right = CLAMP(in.front.right + (in.side.right / 2), -1.0f, 1.0f);
				ret.center = in.center;
				ret.back.left = CLAMP(in.back.left + (in.side.left / 2), -1.0f, 1.0f);
				ret.back.right = CLAMP(in.back.right + (in.side.right / 2), -1.0f, 1.0f);
				ret.sub = CLAMP(in.front.left + in.front.right + in.center + (in.side.left / 2) + (in.side.right / 2) + in.back.left + in.back.right, -1.0f, 1.0f);
				return ret;
			}
			af51 To51CH(af71 in) {
				af51 ret;
				ret.front.left = CLAMP(in.front.left + (in.side.left / 2), -1.0f, 1.0f);
				ret.front.right = CLAMP(in.front.right + (in.side.right / 2), -1.0f, 1.0f);
				ret.center = in.center;
				ret.back.left = CLAMP(in.back.left + (in.side.left / 2), -1.0f, 1.0f);
				ret.back.right = CLAMP(in.back.right + (in.side.right / 2), -1.0f, 1.0f);
				ret.sub = in.sub;
				return ret;
			}

			//7 ch
			af7 To7CH(af1 in) {
				af7 ret;
				ret.front.left = in.mono;
				ret.front.right = in.mono;
				ret.center = in.mono;
				ret.side.left = in.mono / 2;
				ret.side.right = in.mono / 2;
				ret.back.left = in.mono / 3;
				ret.back.right = in.mono / 3;
				return ret;
			}
			af7 To7CH(af2 in) {
				af7 ret;
				ret.front.left = in.left;
				ret.front.right = in.right;
				ret.center = CLAMP(in.left + in.right, -1.0f, 1.0f);
				ret.side.left = in.left / 2;
				ret.side.right = in.right / 2;
				ret.back.left = in.left / 3;
				ret.back.right = in.right / 3;
				return ret;
			}
			af7 To7CH(af21 in) {
				af7 ret;
				ret.front.left = CLAMP(in.left + (in.sub / 2), -1.0f, 1.0f);
				ret.front.right = CLAMP(in.right + (in.sub / 2), -1.0f, 1.0f);
				ret.center = CLAMP(in.left + in.right + (in.sub / 2), -1.0f, 1.0f);
				ret.side.left = CLAMP((in.left / 2) + (in.sub / 2), -1.0f, 1.0f);
				ret.side.right = CLAMP((in.right / 2) + (in.sub / 2), -1.0f, 1.0f);
				ret.back.left = CLAMP((in.left / 3) + (in.sub / 2), -1.0f, 1.0f);
				ret.back.right = CLAMP((in.right / 3) + (in.sub / 2), -1.0f, 1.0f);
				return ret;
			}
			af7 To7CH(af3 in) {
				af7 ret;
				ret.front.left = in.left;
				ret.front.right = in.right;
				ret.center = in.center;
				ret.side.left = in.left / 2;
				ret.side.right = in.right / 2;
				ret.back.left = in.left / 3;
				ret.back.right = in.right / 3;
				return ret;
			}
			af7 To7CH(af31 in) {
				af7 ret;
				ret.front.left = CLAMP(in.left + (in.sub / 2), -1.0f, 1.0f);
				ret.front.right = CLAMP(in.right + (in.sub / 2), -1.0f, 1.0f);
				ret.center = CLAMP(in.center + (in.sub / 2), -1.0f, 1.0f);
				ret.side.left = CLAMP((in.left / 2) + (in.sub / 2), -1.0f, 1.0f);
				ret.side.right = CLAMP((in.right / 2) + (in.sub / 2), -1.0f, 1.0f);
				ret.back.left = CLAMP((in.left / 3) + (in.sub / 2), -1.0f, 1.0f);
				ret.back.right = CLAMP((in.right / 3) + (in.sub / 2), -1.0f, 1.0f);
				return ret;
			}
			af7 To7CH(af5 in) {
				af7 ret;
				ret.front.left = in.front.left;
				ret.front.right = in.front.right;
				ret.center = in.center;
				ret.side.left = CLAMP((in.front.left / 2) + (in.back.left / 2), -1.0f, 1.0f);
				ret.side.right = CLAMP((in.front.right / 2) + (in.back.right / 2), -1.0f, 1.0f);
				ret.back.left = in.back.left;
				ret.back.right = in.back.right;
				return ret;
			}
			af7 To7CH(af51 in) {
				af7 ret;
				ret.front.left = CLAMP(in.front.left + (in.sub / 2), -1.0f, 1.0f);
				ret.front.right = CLAMP(in.front.right + (in.sub / 2), -1.0f, 1.0f);
				ret.center = CLAMP(in.center + (in.sub / 2), -1.0f, 1.0f);
				ret.side.left = CLAMP((in.front.left / 2) + (in.back.left / 2) + (in.sub / 2), -1.0f, 1.0f);
				ret.side.right = CLAMP((in.front.right / 2) + (in.back.right / 2) + (in.sub / 2), -1.0f, 1.0f);
				ret.back.left = CLAMP(in.back.left + (in.sub / 2), -1.0f, 1.0f);
				ret.back.right = CLAMP(in.back.right + (in.sub / 2), -1.0f, 1.0f);
				return ret;
			}
			af7 To7CH(af71 in) {
				af7 ret;
				ret.front.left = CLAMP(in.front.left + (in.sub / 2), -1.0f, 1.0f);
				ret.front.right = CLAMP(in.front.right + (in.sub / 2), -1.0f, 1.0f);
				ret.center = CLAMP(in.center + (in.sub / 2), -1.0f, 1.0f);
				ret.side.left = CLAMP(in.side.left + (in.sub / 2), -1.0f, 1.0f);
				ret.side.right = CLAMP(in.side.right + (in.sub / 2), -1.0f, 1.0f);
				ret.back.left = CLAMP(in.back.left + (in.sub / 2), -1.0f, 1.0f);
				ret.back.right = CLAMP(in.back.right + (in.sub / 2), -1.0f, 1.0f);
				return ret;
			}

			//7.1 ch
			af71 To71CH(af1 in) {
				af71 ret;
				ret.front.left = in.mono;
				ret.front.right = in.mono;
				ret.center = in.mono;
				ret.side.left = in.mono / 2;
				ret.side.right = in.mono / 2;
				ret.back.left = in.mono / 3;
				ret.back.right = in.mono / 3;
				ret.sub = in.mono;
				return ret;
			}
			af71 To71CH(af2 in) {
				af71 ret;
				ret.front.left = in.left;
				ret.front.right = in.right;
				ret.center = CLAMP(in.left + in.right, -1.0f, 1.0f);
				ret.side.left = in.left / 2;
				ret.side.right = in.right / 2;
				ret.back.left = in.left / 3;
				ret.back.right = in.right / 3;
				ret.sub = CLAMP(in.left + in.right, -1.0f, 1.0f);
				return ret;
			}
			af71 To71CH(af21 in) {
				af71 ret;
				ret.front.left = in.left;
				ret.front.right = in.right;
				ret.center = CLAMP(in.left + in.right, -1.0f, 1.0f);
				ret.side.left = in.left / 2;
				ret.side.right = in.right / 2;
				ret.back.left = in.left / 3;
				ret.back.right = in.right / 3;
				ret.sub = in.sub;
				return ret;
			}
			af71 To71CH(af3 in) {
				af71 ret;
				ret.front.left = in.left;
				ret.front.right = in.right;
				ret.center = in.center;
				ret.side.left = in.left / 2;
				ret.side.right = in.right / 2;
				ret.back.left = in.left / 3;
				ret.back.right = in.right / 3;
				ret.sub = CLAMP(in.left + in.right + in.center, -1.0f, 1.0f);
				return ret;
			}
			af71 To71CH(af31 in) {
				af71 ret;
				ret.front.left = in.left;
				ret.front.right = in.right;
				ret.center = in.center;
				ret.side.left = in.left / 2;
				ret.side.right = in.right / 2;
				ret.back.left = in.left / 3;
				ret.back.right = in.right / 3;
				ret.sub = in.sub;
				return ret;
			}
			af71 To71CH(af5 in) {
				af71 ret;
				ret.front.left = in.front.left;
				ret.front.right = in.front.right;
				ret.center = in.center;
				ret.side.left = CLAMP((in.front.left / 2) + (in.back.left / 2), -1.0f, 1.0f);
				ret.side.right = CLAMP((in.front.right / 2) + (in.back.right / 2), -1.0f, 1.0f);
				ret.back.left = in.back.left;
				ret.back.right = in.back.right;
				ret.sub = CLAMP(in.front.left + in.front.right + in.center + in.back.left + in.back.right, -1.0f, 1.0f);
				return ret;
			}
			af71 To71CH(af51 in) {
				af71 ret;
				ret.front.left = in.front.left;
				ret.front.right = in.front.right;
				ret.center = in.center;
				ret.side.left = CLAMP((in.front.left / 2) + (in.back.left / 2), -1.0f, 1.0f);
				ret.side.right = CLAMP((in.front.right / 2) + (in.back.right / 2), -1.0f, 1.0f);
				ret.back.left = in.back.left;
				ret.back.right = in.back.right;
				ret.sub = in.sub;
				return ret;
			}
			af71 To71CH(af7 in) {
				af71 ret;
				ret.front.left = in.front.left;
				ret.front.right = in.front.right;
				ret.center = in.center;
				ret.side.left = in.side.left;
				ret.side.right = in.side.right;
				ret.back.left = in.back.left;
				ret.back.right = in.back.right;
				ret.sub = CLAMP(in.front.left + in.front.right + in.center + in.side.left + in.side.right + in.back.left + in.back.right, -1.0f, 1.0f);
				return ret;
			}
		}
	}

	IAudioSource::IAudioSource() {
		m_hasError = false;
		m_flags = eAF_None;
	}

	AudioSourceError IAudioSource::GetError() {
		if (this->m_errors.size() == 0) {
			AudioSourceError noErr;
			this->m_hasError = false;
			return noErr;
		}

		AudioSourceError ret = this->m_errors.at(this->m_errors.size() - 1);
		this->m_errors.pop_back();

		if (this->m_errors.size() == 0) this->m_hasError = false;

		return ret;
	}

	void IAudioSource::pushError(int errorCode, const char * errorStr) {
		AudioSourceError err;
		err.code = errorCode;
		err.str = errorStr;
		this->m_errors.push_back(err);
		this->m_hasError = true;
	}
NSGameEnd