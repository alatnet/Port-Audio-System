#include <StdAfx.h>
#include "IAudioSource.h"

NSGameStart
	namespace AudioFrame {
		namespace Convert {
			//mono
			_1 ToMono(_2 in) {
				_1 ret;
				ret.mono = CLAMP(in.left + in.right,-1.0f,1.0f);
				return ret;
			}
			_1 ToMono(_21 in) {
				_1 ret;
				ret.mono = CLAMP(in.left + in.right + in.sub, -1.0f, 1.0f);
				return ret;
			}
			_1 ToMono(_3 in) {
				_1 ret;
				ret.mono = CLAMP(in.left + in.right + in.center, -1.0f, 1.0f);
				return ret;
			}
			_1 ToMono(_31 in) {
				_1 ret;
				ret.mono = CLAMP(in.left + in.right + in.center + in.sub, -1.0f, 1.0f);
				return ret;
			}
			_1 ToMono(_5 in) {
				_1 ret;
				ret.mono = CLAMP(in.front.left + in.front.right + in.center + in.back.left + in.back.right, -1.0f, 1.0f);
				return ret;
			}
			_1 ToMono(_51 in) {
				_1 ret;
				ret.mono = CLAMP(in.front.left + in.front.right + in.center + in.back.left + in.back.right + in.sub, -1.0f, 1.0f);
				return ret;
			}
			_1 ToMono(_7 in) {
				_1 ret;
				ret.mono = CLAMP(in.front.left + in.front.right + in.center + in.side.left + in.side.right + in.back.left + in.back.right, -1.0f, 1.0f);
				return ret;
			}
			_1 ToMono(_71 in) {
				_1 ret;
				ret.mono = CLAMP(in.front.left + in.front.right + in.center + in.side.left + in.side.right + in.back.left + in.back.right + in.sub, -1.0f, 1.0f);
				return ret;
			}

			//2 ch
			_2 To2CH(_1 in) {
				_2 ret;
				ret.left = in.mono;
				ret.right = in.mono;
				return ret;
			}
			_2 To2CH(_21 in) {
				_2 ret;
				ret.left = CLAMP(in.left + (in.sub / 2), -1.0f, 1.0f);
				ret.right = CLAMP(in.right + (in.sub / 2), -1.0f, 1.0f);
				return ret;
			}
			_2 To2CH(_3 in) {
				_2 ret;
				ret.left = CLAMP(in.left + (in.center / 2), -1.0f, 1.0f);
				ret.right = CLAMP(in.right + (in.center / 2), -1.0f, 1.0f);
				return ret;
			}
			_2 To2CH(_31 in) {
				_2 ret;
				ret.left = CLAMP(in.left + (in.center / 2) + (in.sub / 2), -1.0f, 1.0f);
				ret.right = CLAMP(in.right + (in.center / 2) + (in.sub / 2), -1.0f, 1.0f);
				return ret;
			}
			_2 To2CH(_5 in) {
				_2 ret;
				ret.left = CLAMP(in.front.left + (in.center / 2) + (in.back.left / 2), -1.0f, 1.0f);
				ret.right = CLAMP(in.front.right + (in.center / 2) + (in.back.right / 2), -1.0f, 1.0f);
				return ret;
			}
			_2 To2CH(_51 in) {
				_2 ret;
				ret.left = CLAMP(in.front.left + (in.center / 2) + (in.back.left / 2) + (in.sub / 2), -1.0f, 1.0f);
				ret.right = CLAMP(in.front.right + (in.center / 2) + (in.back.right / 2) + (in.sub / 2), -1.0f, 1.0f);
				return ret;
			}
			_2 To2CH(_7 in) {
				_2 ret;
				ret.left = CLAMP(in.front.left + (in.center / 2) + (in.side.left / 2) + (in.back.left / 3), -1.0f, 1.0f);
				ret.right = CLAMP(in.front.right + (in.center / 2) + (in.side.right / 2) + (in.back.right / 3), -1.0f, 1.0f);
				return ret;
			}
			_2 To2CH(_71 in) {
				_2 ret;
				ret.left = CLAMP(in.front.left + (in.center / 2) + (in.side.left / 2) + (in.back.left / 3) + (in.sub / 2), -1.0f, 1.0f);
				ret.right = CLAMP(in.front.right + (in.center / 2) + (in.side.right / 2) + (in.back.right / 3) + (in.sub / 2), -1.0f, 1.0f);
				return ret;
			}

			//2.1 ch
			_21 To21CH(_1 in) {
				_21 ret;
				ret.left = in.mono;
				ret.right = in.mono;
				ret.sub = in.mono;
				return ret;
			}
			_21 To21CH(_2 in) {
				_21 ret;
				ret.left = in.left;
				ret.right = in.right;
				ret.sub = CLAMP(in.left + in.right, -1.0f, 1.0f);
				return ret;
			}
			_21 To21CH(_3 in) {
				_21 ret;
				ret.left = CLAMP(in.left + (in.center / 2), -1.0f, 1.0f);
				ret.right = CLAMP(in.right + (in.center / 2), -1.0f, 1.0f);
				ret.sub = CLAMP(in.left + in.right + in.center, -1.0f, 1.0f);
				return ret;
			}
			_21 To21CH(_31 in) {
				_21 ret;
				ret.left = CLAMP(in.left + (in.center / 2), -1.0f, 1.0f);
				ret.right = CLAMP(in.right + (in.center / 2), -1.0f, 1.0f);
				ret.sub = in.sub;
				return ret;
			}
			_21 To21CH(_5 in) {
				_21 ret;
				ret.left = CLAMP(in.front.left + (in.center / 2) + (in.back.left / 2), -1.0f, 1.0f);
				ret.right = CLAMP(in.front.right + (in.center / 2) + (in.back.right / 2), -1.0f, 1.0f);
				ret.sub = CLAMP(in.front.left + in.front.right + in.center + (in.back.left / 2) + (in.back.right / 2), -1.0f, 1.0f);
				return ret;
			}
			_21 To21CH(_51 in) {
				_21 ret;
				ret.left = CLAMP(in.front.left + (in.center / 2) + (in.back.left / 2), -1.0f, 1.0f);
				ret.right = CLAMP(in.front.right + (in.center / 2) + (in.back.right / 2), -1.0f, 1.0f);
				ret.sub = in.sub;
				return ret;
			}
			_21 To21CH(_7 in) {
				_21 ret;
				ret.left = CLAMP(in.front.left + (in.center / 2) + (in.side.left / 2) + (in.back.left / 3), -1.0f, 1.0f);
				ret.right = CLAMP(in.front.right + (in.center / 2) + (in.side.right / 2) + (in.back.right / 3), -1.0f, 1.0f);
				ret.sub = (in.front.left + in.front.right + in.center + (in.side.left / 2) + (in.side.right / 2) + (in.back.left / 3) + (in.back.right / 3), -1.0f, 1.0f);
				return ret;
			}
			_21 To21CH(_71 in) {
				_21 ret;
				ret.left = CLAMP(in.front.left + (in.center / 2) + (in.side.left / 2) + (in.back.left / 3), -1.0f, 1.0f);
				ret.right = CLAMP(in.front.right + (in.center / 2) + (in.side.right / 2) + (in.back.right / 3), -1.0f, 1.0f);
				ret.sub = in.sub;
				return ret;
			}

			//3 ch
			_3 To3CH(_1 in) {
				_3 ret;
				ret.left = in.mono;
				ret.right = in.mono;
				ret.center = in.mono;
				return ret;
			}
			_3 To3CH(_2 in) {
				_3 ret;
				ret.left = in.left;
				ret.right = in.right;
				ret.center = CLAMP(in.left + in.right, -1.0f, 1.0f);
				return ret;
			}
			_3 To3CH(_21 in) {
				_3 ret;
				ret.left = CLAMP(in.left + (in.sub / 2), -1.0f, 1.0f);
				ret.right = CLAMP(in.right + (in.sub / 2), -1.0f, 1.0f);
				ret.center = CLAMP(in.right + in.left + (in.sub / 2), -1.0f, 1.0f);
				return ret;
			}
			_3 To3CH(_31 in) {
				_3 ret;
				ret.left = CLAMP(in.left + (in.sub / 2), -1.0f, 1.0f);
				ret.right = CLAMP(in.right + (in.sub / 2), -1.0f, 1.0f);
				ret.center = CLAMP(in.center + (in.sub / 2), -1.0f, 1.0f);
				return ret;
			}
			_3 To3CH(_5 in) {
				_3 ret;
				ret.left = CLAMP(in.front.left + (in.back.left / 2), -1.0f, 1.0f);
				ret.right = CLAMP(in.front.right + (in.back.right / 2), -1.0f, 1.0f);
				ret.center = in.center;
				return ret;
			}
			_3 To3CH(_51 in) {
				_3 ret;
				ret.left = CLAMP(in.front.left + (in.back.left / 2) + (in.sub / 2), -1.0f, 1.0f);
				ret.right = CLAMP(in.front.right + (in.back.right / 2) + (in.sub / 2), -1.0f, 1.0f);
				ret.center = CLAMP(in.center + (in.sub / 2), -1.0f, 1.0f) / 2;
				return ret;
			}
			_3 To3CH(_7 in) {
				_3 ret;
				ret.left = CLAMP(in.front.left + (in.side.left / 2) + (in.back.left / 3), -1.0f, 1.0f);
				ret.right = CLAMP(in.front.right + (in.side.right / 2) + (in.back.right / 3), -1.0f, 1.0f);
				ret.center = in.center;
				return ret;
			}
			_3 To3CH(_71 in) {
				_3 ret;
				ret.left = CLAMP(in.front.left + (in.side.left / 2) + (in.back.left / 3) + (in.sub / 2), -1.0f, 1.0f);
				ret.right = CLAMP(in.front.right + (in.side.right / 2) + (in.back.right / 3) + (in.sub / 2), -1.0f, 1.0f);
				ret.center = CLAMP(in.center + (in.sub / 2), -1.0f, 1.0f);
				return ret;
			}

			//3.1 ch
			_31 To31CH(_1 in) {
				_31 ret;
				ret.left = in.mono;
				ret.right = in.mono;
				ret.center = in.mono;
				ret.sub = in.mono;
				return ret;
			}
			_31 To31CH(_2 in) {
				_31 ret;
				ret.left = in.left;
				ret.right = in.right;
				ret.center = CLAMP(in.left + in.right, -1.0f, 1.0f);
				ret.sub = CLAMP(in.left + in.right, -1.0f, 1.0f);
				return ret;
			}
			_31 To31CH(_21 in) {
				_31 ret;
				ret.left = in.left;
				ret.right = in.right;
				ret.center = CLAMP(in.right + in.left, -1.0f, 1.0f);
				ret.sub = in.sub;
				return ret;
			}
			_31 To31CH(_3 in) {
				_31 ret;
				ret.left = in.left;
				ret.right = in.right;
				ret.center = in.center;
				ret.sub = CLAMP(in.left + in.right + in.center, -1.0f, 1.0f);
				return ret;
			}
			_31 To31CH(_5 in) {
				_31 ret;
				ret.left = CLAMP(in.front.left + (in.back.left / 2), -1.0f, 1.0f);
				ret.right = CLAMP(in.front.right + (in.back.right / 2), -1.0f, 1.0f);
				ret.center = in.center;
				ret.sub = CLAMP(in.front.left + in.front.right + in.center + (in.back.left / 2) + (in.back.right / 2), -1.0f, 1.0f);
				return ret;
			}
			_31 To31CH(_51 in) {
				_31 ret;
				ret.left = CLAMP(in.front.left + (in.back.left / 2), -1.0f, 1.0f);
				ret.right = CLAMP(in.front.right + (in.back.right / 2), -1.0f, 1.0f);
				ret.center = in.center;
				ret.sub = in.sub;
				return ret;
			}
			_31 To31CH(_7 in) {
				_31 ret;
				ret.left = CLAMP(in.front.left + (in.side.left / 2) + (in.back.left / 3), -1.0f, 1.0f);
				ret.right = CLAMP(in.front.right + (in.side.right / 2) + (in.back.right / 3), -1.0f, 1.0f);
				ret.center = in.center;
				ret.sub = CLAMP(in.front.left + in.front.right + in.center + (in.side.left / 2) + (in.side.right / 2) + (in.back.left / 3) + (in.back.right / 3), -1.0f, 1.0f);
				return ret;
			}
			_31 To31CH(_71 in) {
				_31 ret;
				ret.left = CLAMP(in.front.left + (in.side.left / 2) + (in.back.left / 3), -1.0f, 1.0f);
				ret.right = CLAMP(in.front.right + (in.side.right / 2) + (in.back.right / 3), -1.0f, 1.0f);
				ret.center = in.center;
				ret.sub = in.sub;
				return ret;
			}

			//5 ch
			_5 To5CH(_1 in) {
				_5 ret;
				ret.front.left = in.mono;
				ret.front.right = in.mono;
				ret.center = in.mono;
				ret.back.left = in.mono / 2;
				ret.back.right = in.mono / 2;
				return ret;
			}
			_5 To5CH(_2 in) {
				_5 ret;
				ret.front.left = in.left;
				ret.front.right = in.right;
				ret.center = CLAMP(in.left + in.right, -1.0f, 1.0f);
				ret.back.left = in.left / 2;
				ret.back.right = in.right / 2;
				return ret;
			}
			_5 To5CH(_21 in) {
				_5 ret;
				ret.front.left = CLAMP(in.left + (in.sub / 2), -1.0f, 1.0f);
				ret.front.right = CLAMP(in.right + (in.sub / 2), -1.0f, 1.0f);
				ret.center = CLAMP(in.right + in.left + (in.sub / 2), -1.0f, 1.0f);
				ret.back.left = CLAMP((in.left / 2) + (in.sub / 2), -1.0f, 1.0f);
				ret.back.right = CLAMP((in.right / 2) + (in.sub / 2), -1.0f, 1.0f);
				return ret;
			}
			_5 To5CH(_3 in) {
				_5 ret;
				ret.front.left = in.left;
				ret.front.right = in.right;
				ret.center = in.center;
				ret.back.left = in.left / 2;
				ret.back.right = in.right / 2;
				return ret;
			}
			_5 To5CH(_31 in) {
				_5 ret;
				ret.front.left = CLAMP(in.left + (in.sub / 2), -1.0f, 1.0f);
				ret.front.right = CLAMP(in.right + (in.sub / 2), -1.0f, 1.0f);
				ret.center = CLAMP(in.center + (in.sub / 2), -1.0f, 1.0f);
				ret.back.left = CLAMP((in.left / 2) + (in.sub / 2), -1.0f, 1.0f);
				ret.back.right = CLAMP((in.right / 2) + (in.sub / 2), -1.0f, 1.0f);
				return ret;
			}
			_5 To5CH(_51 in) {
				_5 ret;
				ret.front.left = CLAMP(in.front.left + (in.sub / 2), -1.0f, 1.0f);
				ret.front.right = CLAMP(in.front.right + (in.sub / 2), -1.0f, 1.0f);
				ret.center = CLAMP(in.center + (in.sub / 2), -1.0f, 1.0f);
				ret.back.left = CLAMP(in.back.left + (in.sub / 2), -1.0f, 1.0f);
				ret.back.right = CLAMP(in.back.right + (in.sub / 2), -1.0f, 1.0f);
				return ret;
			}
			_5 To5CH(_7 in) {
				_5 ret;
				ret.front.left = CLAMP(in.front.left + (in.side.left / 2), -1.0f, 1.0f);
				ret.front.right = CLAMP(in.front.right + (in.side.right / 2), -1.0f, 1.0f);
				ret.center = in.center;
				ret.back.left = CLAMP(in.back.left + (in.side.left / 2), -1.0f, 1.0f);
				ret.back.right = CLAMP(in.back.right + (in.side.left / 2), -1.0f, 1.0f);
				return ret;
			}
			_5 To5CH(_71 in) {
				_5 ret;
				ret.front.left = CLAMP(in.front.left + (in.side.left / 2) + (in.sub / 2), -1.0f, 1.0f);
				ret.front.right = CLAMP(in.front.right + (in.side.right / 2) + (in.sub / 2), -1.0f, 1.0f);
				ret.center = CLAMP(in.center + (in.sub / 2), -1.0f, 1.0f);
				ret.back.left = CLAMP(in.back.left + (in.side.left / 2) + (in.sub / 2), -1.0f, 1.0f);
				ret.back.right = CLAMP(in.back.right + (in.side.left / 2) + (in.sub / 2), -1.0f, 1.0f);
				return ret;
			}

			//5.1 ch
			_51 To51CH(_1 in) {
				_51 ret;
				ret.front.left = in.mono;
				ret.front.right = in.mono;
				ret.center = in.mono;
				ret.back.left = in.mono / 2;
				ret.back.right = in.mono / 2;
				ret.sub = in.mono;
				return ret;
			}
			_51 To51CH(_2 in) {
				_51 ret;
				ret.front.left = in.left;
				ret.front.right = in.right;
				ret.center = CLAMP(in.left + in.right, -1.0f, 1.0f);
				ret.back.left = in.left / 2;
				ret.back.right = in.right / 2;
				ret.sub = CLAMP(in.left + in.right, -1.0f, 1.0f);
				return ret;
			}
			_51 To51CH(_21 in) {
				_51 ret;
				ret.front.left = in.left;
				ret.front.right = in.right;
				ret.center = CLAMP(in.left + in.right, -1.0f, 1.0f);
				ret.back.left = in.left / 2;
				ret.back.right = in.right / 2;
				ret.sub = in.sub;
				return ret;
			}
			_51 To51CH(_3 in) {
				_51 ret;
				ret.front.left = in.left;
				ret.front.right = in.right;
				ret.center = in.center;
				ret.back.left = in.left / 2;
				ret.back.right = in.right / 2;
				ret.sub = CLAMP(in.left + in.right + in.center, -1.0f, 1.0f);
				return ret;
			}
			_51 To51CH(_31 in) {
				_51 ret;
				ret.front.left = in.left;
				ret.front.right = in.right;
				ret.center = in.center;
				ret.back.left = in.left / 2;
				ret.back.right = in.right / 2;
				ret.sub = in.sub;
				return ret;
			}
			_51 To51CH(_5 in) {
				_51 ret;
				ret.front.left = in.front.left;
				ret.front.right = in.front.right;
				ret.center = in.center;
				ret.back.left = in.back.left;
				ret.back.right = in.back.right;
				ret.sub = CLAMP(in.front.left + in.front.right + in.center + in.back.left + in.back.right, -1.0f, 1.0f);
				return ret;
			}
			_51 To51CH(_7 in) {
				_51 ret;
				ret.front.left = CLAMP(in.front.left + (in.side.left / 2), -1.0f, 1.0f);
				ret.front.right = CLAMP(in.front.right + (in.side.right / 2), -1.0f, 1.0f);
				ret.center = in.center;
				ret.back.left = CLAMP(in.back.left + (in.side.left / 2), -1.0f, 1.0f);
				ret.back.right = CLAMP(in.back.right + (in.side.right / 2), -1.0f, 1.0f);
				ret.sub = CLAMP(in.front.left + in.front.right + in.center + (in.side.left / 2) + (in.side.right / 2) + in.back.left + in.back.right, -1.0f, 1.0f);
				return ret;
			}
			_51 To51CH(_71 in) {
				_51 ret;
				ret.front.left = CLAMP(in.front.left + (in.side.left / 2), -1.0f, 1.0f);
				ret.front.right = CLAMP(in.front.right + (in.side.right / 2), -1.0f, 1.0f);
				ret.center = in.center;
				ret.back.left = CLAMP(in.back.left + (in.side.left / 2), -1.0f, 1.0f);
				ret.back.right = CLAMP(in.back.right + (in.side.right / 2), -1.0f, 1.0f);
				ret.sub = in.sub;
				return ret;
			}

			//7 ch
			_7 To7CH(_1 in) {
				_7 ret;
				ret.front.left = in.mono;
				ret.front.right = in.mono;
				ret.center = in.mono;
				ret.side.left = in.mono / 2;
				ret.side.right = in.mono / 2;
				ret.back.left = in.mono / 3;
				ret.back.right = in.mono / 3;
				return ret;
			}
			_7 To7CH(_2 in) {
				_7 ret;
				ret.front.left = in.left;
				ret.front.right = in.right;
				ret.center = CLAMP(in.left + in.right, -1.0f, 1.0f);
				ret.side.left = in.left / 2;
				ret.side.right = in.right / 2;
				ret.back.left = in.left / 3;
				ret.back.right = in.right / 3;
				return ret;
			}
			_7 To7CH(_21 in) {
				_7 ret;
				ret.front.left = CLAMP(in.left + (in.sub / 2), -1.0f, 1.0f);
				ret.front.right = CLAMP(in.right + (in.sub / 2), -1.0f, 1.0f);
				ret.center = CLAMP(in.left + in.right + (in.sub / 2), -1.0f, 1.0f);
				ret.side.left = CLAMP((in.left / 2) + (in.sub / 2), -1.0f, 1.0f);
				ret.side.right = CLAMP((in.right / 2) + (in.sub / 2), -1.0f, 1.0f);
				ret.back.left = CLAMP((in.left / 3) + (in.sub / 2), -1.0f, 1.0f);
				ret.back.right = CLAMP((in.right / 3) + (in.sub / 2), -1.0f, 1.0f);
				return ret;
			}
			_7 To7CH(_3 in) {
				_7 ret;
				ret.front.left = in.left;
				ret.front.right = in.right;
				ret.center = in.center;
				ret.side.left = in.left / 2;
				ret.side.right = in.right / 2;
				ret.back.left = in.left / 3;
				ret.back.right = in.right / 3;
				return ret;
			}
			_7 To7CH(_31 in) {
				_7 ret;
				ret.front.left = CLAMP(in.left + (in.sub / 2), -1.0f, 1.0f);
				ret.front.right = CLAMP(in.right + (in.sub / 2), -1.0f, 1.0f);
				ret.center = CLAMP(in.center + (in.sub / 2), -1.0f, 1.0f);
				ret.side.left = CLAMP((in.left / 2) + (in.sub / 2), -1.0f, 1.0f);
				ret.side.right = CLAMP((in.right / 2) + (in.sub / 2), -1.0f, 1.0f);
				ret.back.left = CLAMP((in.left / 3) + (in.sub / 2), -1.0f, 1.0f);
				ret.back.right = CLAMP((in.right / 3) + (in.sub / 2), -1.0f, 1.0f);
				return ret;
			}
			_7 To7CH(_5 in) {
				_7 ret;
				ret.front.left = in.front.left;
				ret.front.right = in.front.right;
				ret.center = in.center;
				ret.side.left = CLAMP((in.front.left / 2) + (in.back.left / 2), -1.0f, 1.0f);
				ret.side.right = CLAMP((in.front.right / 2) + (in.back.right / 2), -1.0f, 1.0f);
				ret.back.left = in.back.left;
				ret.back.right = in.back.right;
				return ret;
			}
			_7 To7CH(_51 in) {
				_7 ret;
				ret.front.left = CLAMP(in.front.left + (in.sub / 2), -1.0f, 1.0f);
				ret.front.right = CLAMP(in.front.right + (in.sub / 2), -1.0f, 1.0f);
				ret.center = CLAMP(in.center + (in.sub / 2), -1.0f, 1.0f);
				ret.side.left = CLAMP((in.front.left / 2) + (in.back.left / 2) + (in.sub / 2), -1.0f, 1.0f);
				ret.side.right = CLAMP((in.front.right / 2) + (in.back.right / 2) + (in.sub / 2), -1.0f, 1.0f);
				ret.back.left = CLAMP(in.back.left + (in.sub / 2), -1.0f, 1.0f);
				ret.back.right = CLAMP(in.back.right + (in.sub / 2), -1.0f, 1.0f);
				return ret;
			}
			_7 To7CH(_71 in) {
				_7 ret;
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
			_71 To71CH(_1 in) {
				_71 ret;
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
			_71 To71CH(_2 in) {
				_71 ret;
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
			_71 To71CH(_21 in) {
				_71 ret;
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
			_71 To71CH(_3 in) {
				_71 ret;
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
			_71 To71CH(_31 in) {
				_71 ret;
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
			_71 To71CH(_5 in) {
				_71 ret;
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
			_71 To71CH(_51 in) {
				_71 ret;
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
			_71 To71CH(_7 in) {
				_71 ret;
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