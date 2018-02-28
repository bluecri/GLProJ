#pragma once
#include "./ParticleStruct.h"
#include <algorithm>

class ParticleSystem {
	public:
		const int c_maxParticles;
		Particle particlesContainer[10000];
		int lastUsedParticleIdx = 0;
		ParticleSystem(int particlesNum) : c_maxParticles(particlesNum){
			for (int i = 0; i<c_maxParticles; i++) {
				particlesContainer[i].life = -1.0f;
				particlesContainer[i].cameradistance = -1.0f;
			}

		}

		void sortParticlesContainer() {
			std::sort(&particlesContainer[0], &particlesContainer[c_maxParticles]);
		}

		int FindUnusedParticle() {
			for (int i=lastUsedParticleIdx; i<c_maxParticles; i++) {
				if (particlesContainer[i].life < 0) {
					lastUsedParticleIdx = i;
					return i;
				}
			}

			for (int i=0; i<lastUsedParticleIdx; i++) {
				if (particlesContainer[i].life < 0) {
					lastUsedParticleIdx = i;
					return i;
				}
			}

			return 0; // All particles are taken, override the first one
		}
};