#include <ParticleInfo.h>

Particle& ParticleInfo::createNewParticle(Particle &refP) {
	refP.life = particleLife;
	refP.pos = startPos;

	glm::vec3 randomdir = glm::vec3(
		(rand() % 2000 - 1000.0f) / 1000.0f,
		(rand() % 2000 - 1000.0f) / 1000.0f,
		(rand() % 2000 - 1000.0f) / 1000.0f
	);
	refP.speed = maindir + randomdir * spread;

	refP.r = rand() % 256;
	refP.g = rand() % 256;
	refP.b = rand() % 256;
	refP.a = (rand() % 256) / 3;

	refP.size = (rand() % 1000) / 2000.0f + 0.3f;

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
	p->speed += glm::vec3(0.0f, -0.0981f, 0.0f) * (float)m_deltaTime * 0.5f;
	p->pos += p->speed * (float)m_deltaTime;

	p->cameradistance = glm::length(p->pos - m_cameraPos);
	p->pos += glm::vec3(0.0f, 10.0f, 0.0f) * (float)m_deltaTime;

	return;
}