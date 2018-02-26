#pragma once
#include <glm/glm.hpp>

class ParticleInfo;

//reference : opengl tutorial
struct Particle {
	glm::vec3 pos, speed;
	unsigned char color[4]; // Color(rgba)
	float size, angle, weight;
	float life; // Remaining life of the particle. if <0 : dead and unused.
	float cameradistance; // *Squared* distance to the camera. if dead : -1.0f

	ParticleInfo* particleInfoPtr;

	bool operator<(const Particle& that) const {
		// Sort in reverse order : far particles drawn first.
		return this->cameradistance > that.cameradistance;
	}
};