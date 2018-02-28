#pragma once
#include <glm/gtc/random.hpp>

#include "./ParticleStruct.h"

class ParticleInfo {
public:
	bool isOneParticlePerMultiFrame = false;

	int frameVsParticle = 1;
	int countFrame = 0;

	float particleLife = 3.0f;

	glm::vec3 startPos = glm::vec3(0, 0, 0.0f);
	glm::vec3 mainDir = glm::vec3(0.0f, 3.0f, 0.0f);
	float spread = 0.5f;
	int m_colorStartRange[4], m_colorEndRange[4];
	float particleSizeStartRange, particleSizeEndRange;

	//for update
	float m_deltaTime;
	glm::vec3 m_cameraPos;

	ParticleInfo() {
		isOneParticlePerMultiFrame = false;

		m_colorStartRange[0] = 0;
		m_colorStartRange[1] = 0;
		m_colorStartRange[2] = 0;
		m_colorStartRange[3] = 40;

		m_colorEndRange[0] = 255;
		m_colorEndRange[1] = 255;
		m_colorEndRange[2] = 255;
		m_colorEndRange[3] = 160;

		particleSizeStartRange = 0.05f;
		particleSizeEndRange = 0.1f;
	}
	ParticleInfo(bool isOneParticlePerMultiFrame, int frameVsParticle,float life, glm::vec3 startPos, glm::vec3 mainDir, float spread, int colorStartRange[4], int colorEndRange[4], float particleSizeStartRange, float particleSizeEndRange)
		:isOneParticlePerMultiFrame(isOneParticlePerMultiFrame), frameVsParticle(frameVsParticle), particleLife(life), startPos(startPos), spread(spread), mainDir(mainDir), particleSizeStartRange(particleSizeStartRange), particleSizeEndRange(particleSizeEndRange){
		for (int i = 0; i < 4; i++) {
			m_colorStartRange[i] = colorStartRange[i];
			m_colorEndRange[i] = colorEndRange[i];
		}
	}

	Particle& createNewParticle(Particle &refP); 
	virtual void preProcessBeforeUpdate(float deltaTime, glm::vec3 cameraPos);
	virtual void updateParticle(Particle *p);
};