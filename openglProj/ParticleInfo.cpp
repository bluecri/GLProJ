#include <ParticleInfo.h>
#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>

Particle& ParticleInfo::createNewParticle(Particle &refP) {
	refP.life = particleLife;
	refP.pos = startPos;

	glm::vec3 randomdir = glm::vec3(
		(rand() % 2000 - 1000.0f) / 1000.0f,
		(rand() % 2000 - 1000.0f) / 1000.0f,
		(rand() % 2000 - 1000.0f) / 1000.0f
	);
	refP.speed = mainDir + randomdir * spread;

	
	for (int i = 0; i < 4; i++) {
		refP.color[i] = (unsigned char)(m_colorStartRange[i] > m_colorEndRange[i]) ? glm::linearRand(m_colorEndRange[i], m_colorStartRange[i]) : glm::linearRand(m_colorStartRange[i], m_colorEndRange[i]);
	}
	
	refP.size = glm::linearRand(particleSizeStartRange, particleSizeEndRange);

	refP.particleInfoPtr = (this);

	return refP;
}
void ParticleInfo::preProcessBeforeUpdate(float deltaTime, glm::vec3 cameraPos) {
	m_deltaTime = deltaTime;
	m_cameraPos = cameraPos;
}

void ParticleInfo::updateParticle(Particle *p) {
	m_cameraPos;
	// Simulate simple physics : gravity only, no collisions
	p->pos += p->speed * (float)m_deltaTime;
	p->cameradistance = glm::length(p->pos - m_cameraPos);
	
	return;
}