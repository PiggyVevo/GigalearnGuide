/* EVERY time you make a new state setter, 
you must reconfigure with cmake!!!! For rewards, you can plug them all into one commonrewards.h file 
and modifying that will only require building again. this is just an example of the already included randomstate.cpp file but with a slight modification
Replace the default random state with this, as its better
*/
#include "RandomState.h" //you must include the .h file
#include "../Math.h"

using RocketSim::Math::RandFloat; //access rocketsim for stuff
using RLGC::Math::RandVec;

Vec RandNormVec() {
	return RandVec(Vec(-1, -1, -1), Vec(1, 1, 1)).Normalized();
}

void RLGC::RandomState::ResetArena(Arena* arena) {
	
	// Reset boost pads and everything
	arena->ResetToRandomKickoff();

	constexpr float
		X_MAX = 3500,
		Y_MAX = 4000,
		Z_MAX = 1820,               // ball max height
		CAR_Z_MIN = 17,             // ground height
		CAR_Z_MAX = 17,             // only ground if carsOnGround true
		PITCH_MAX = M_PI / 2,
		YAW_MAX = M_PI,
		ROLL_MAX = M_PI,
		ANGVEL_MAX = 5.5f;

	{ // Randomize ball
		BallState bs = {};
		bs.pos = Math::RandVec(Vec(-X_MAX, -Y_MAX, CommonValues::BALL_RADIUS), Vec(X_MAX, Y_MAX, Z_MAX));
		if (randBallSpeed) {
			bs.vel = RandNormVec() * RandFloat(0, 4000);
			bs.angVel = Math::RandVec(Vec(-4, -4, -4), Vec(4, 4, 4));
		}
		arena->ball->SetState(bs);
	}

	for (Car* car : arena->_cars) { // Randomize cars
		CarState cs = {};

		// Position
		if (carsOnGround) {
			// Car on ground – only Z = 17
			cs.pos = Math::RandVec(Vec(-X_MAX, -Y_MAX, CAR_Z_MIN), Vec(X_MAX, Y_MAX, CAR_Z_MIN));
		} else {
			// Not used, but if needed, limit to 1/4 ceiling
			cs.pos = Math::RandVec(Vec(-X_MAX, -Y_MAX, CAR_Z_MIN), Vec(X_MAX, Y_MAX, CommonValues::CEILING_Z / 4));
		}

		if (randCarSpeed) {
			Vec randVelDir = Math::RandVec(Vec(-1, -1, -1), Vec(1, 1, 1)).Normalized();
			cs.vel = RandNormVec() * RandFloat(0, RLConst::CAR_MAX_SPEED);
			cs.angVel = RandNormVec() * ANGVEL_MAX;
		}

		Angle angle = Angle(RandFloat(-YAW_MAX, YAW_MAX), RandFloat(-PITCH_MAX, PITCH_MAX), RandFloat(-ROLL_MAX, ROLL_MAX));

		if (carsOnGround) {
			// On ground: no pitch/roll, zero vertical velocity and angular vel
			angle.pitch = angle.roll = 0;
			cs.vel.z = 0;
			cs.angVel = {};
		}

		cs.rotMat = angle.ToRotMat();

		cs.boost = RandFloat(0, 100);

		car->SetState(cs);
	}
}
