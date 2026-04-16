// the h file, import this at the top of examplemain.cpp(it should be already imported)
#pragma once
#include "StateSetter.h"

namespace RLGC {
	class RandomState : public StateSetter {
	public:
		bool
			randBallSpeed, randCarSpeed, carsOnGround;

		RandomState(bool randBallSpeed = true, bool randCarSpeed = true, bool carsOnGround = true) :
			randBallSpeed(randBallSpeed), randCarSpeed(randCarSpeed), carsOnGround(carsOnGround) {
		}

		virtual void ResetArena(Arena* arena);
	};
}
