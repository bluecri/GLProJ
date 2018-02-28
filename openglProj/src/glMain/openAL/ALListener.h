#pragma once
#include <al.h>
#include <alc.h>
#include <iostream>

#include <glm/glm.hpp>

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

		float defaultOrientationAt[3] = {1.0f, 0.0f, 1.0f};
		float defaultOrientationUp[3] = {0.0f, 1.0f, 0.0f};

		alListenerfv(AL_POSITION, listenerPos);
		alListenerfv(AL_VELOCITY, listenerVelocity);
		alListenerfv(AL_ORIENTATION, listenerOrientationAtUp);
	}
	void modifyPos(float pos[3]) {
		for (int i = 0; i < 3; i++) {
			listenerPos[i] = pos[i];
		}
		alListenerfv(AL_POSITION, listenerPos);
	}
	void modifyPos(float x, float y, float z) {
		listenerPos[0] = x;
		listenerPos[1] = y;
		listenerPos[2] = z;
		alListenerfv(AL_POSITION, listenerPos);
	}
	void modifyPos(glm::vec3 vec) {
		listenerPos[0] = vec.x;
		listenerPos[1] = vec.y;
		listenerPos[2] = vec.z;
		alListenerfv(AL_POSITION, listenerPos);
	}

	void modifyVelocity(float vel[3]) {
		for (int i = 0; i < 3; i++) {
			listenerVelocity[i] = vel[i];
		}
		alListenerfv(AL_VELOCITY, listenerVelocity);
	}
	
	void modifyOrientationAt(float orientationAt[3]) {
		for (int i = 0; i < 3; i++) {
			listenerOrientationAt[i] = orientationAt[i];
		}
		alListenerfv(AL_ORIENTATION, listenerOrientationAtUp);
	}
	void modifyOrientationUp(float orientationUp[3]) {
		for (int i = 0; i < 3; i++) {
			listenerOrientationUp[i] = orientationUp[i];
		}
		alListenerfv(AL_ORIENTATION, listenerOrientationAtUp);
	}

	void modifyOrientationAtWithRotationMatrix(glm::mat4 rMaxtrix) {
		glm::vec3 atVec = glm::vec3(rMaxtrix * glm::vec4(0.0f, 0.0f, 1.0f, 0.0f));
		for (int i = 0; i < 3; i++) {
			listenerOrientationAt[i] = atVec[i];
		}
		alListenerfv(AL_ORIENTATION, listenerOrientationAtUp);
	}

	void modifyOrientationUpWithRotationMatrix(glm::mat4 rMaxtrix) {
		glm::vec3 upVec = glm::vec3(rMaxtrix * glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
		for (int i = 0; i < 3; i++) {
			listenerOrientationUp[i] = upVec[i];
		}
		alListenerfv(AL_ORIENTATION, listenerOrientationAtUp);
	}

	void modifyOrientationWithRotationMatrix(glm::mat4 rMaxtrix) {
		glm::vec3 atVec = glm::vec3(rMaxtrix * glm::vec4(0.0f, 0.0f, 1.0f, 0.0f));
		for (int i = 0; i < 3; i++) {
			listenerOrientationAt[i] = atVec[i];
		}
		glm::vec3 upVec = glm::vec3(rMaxtrix * glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
		for (int i = 0; i < 3; i++) {
			listenerOrientationUp[i] = upVec[i];
		}
		alListenerfv(AL_ORIENTATION, listenerOrientationAtUp);
	}
	
};