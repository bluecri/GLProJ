#pragma once
#include <al.h>
#include <alc.h>

class ALListener {
private:
	float listenerOrientationAtUp[6];
public:
	float listenerPos[3];
	float listenerVelocity[3];
	float *listenerOrientationAt;
	float *listenerOrientationUp;
	

	ALListener() {
		listenerOrientationAt = &listenerOrientationAtUp[0];
		listenerOrientationUp = &listenerOrientationAtUp[3];

		for (int i = 0; i < 3; i++) {
			listenerPos[i] = 0.0f;
			listenerVelocity[i] = 0.0f;
			listenerOrientationAtUp[i];
			listenerOrientationAt[i] = 0.0f;
			listenerOrientationUp[i] = 0.0f;
		}

		float defaultOrientationAt[3] = {0.0f, 0.0f, 1.0f};
		float defaultOrientationUp[3] = {0.0f, 1.0f, 0.0f};

		alListenerfv(AL_POSITION, listenerPos);
		alListenerfv(AL_VELOCITY, listenerVelocity);
		alListenerfv(AL_ORIENTATION, listenerOrientationAtUp);
	}
	void modifyPos(float pos[3]) {
		for (int i = 0; i < 3; i++) {
			listenerPos[i] = pos[i];
		}
	}
	void modifyVelocity(float vel[3]) {
		for (int i = 0; i < 3; i++) {
			listenerVelocity[i] = vel[i];
		}
	}
	
	void modifyOrientationAt(float orientationAt[3]) {
		for (int i = 0; i < 3; i++) {
			listenerOrientationAt[i] = orientationAt[i];
		}
	}
	void modifyOrientationUp(float orientationUp[3]) {
		for (int i = 0; i < 3; i++) {
			listenerOrientationUp[i] = orientationUp[i];
		}
	}
};