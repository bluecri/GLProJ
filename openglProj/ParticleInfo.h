#pragma once
#include <glm/glm.hpp>

#include <ParticleStruct.h>
class ParticleInfo {
public:
	int particleFlowPerFrame = 1;	//3~5
	float particleLife = 5.0f;
	glm::vec3 startPos = glm::vec3(0, 0, 0.0f);
	float spread = 1.5f;
	glm::vec3 maindir = glm::vec3(0.0f, 3.0f, 0.0f);

	//for update
	float m_deltaTime;
	glm::vec3 m_cameraPos;

	ParticleInfo() {

	}
	Particle& createNewParticle(Particle &refP); 
	void preProcessBeforeUpdate(float deltaTime, glm::vec3 cameraPos);
	void updateParticle(Particle *p);
};