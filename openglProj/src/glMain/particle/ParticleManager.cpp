#include "./ParticleManager.h"

const int ParticleManager::max_particle_num = 10000;
GLfloat* ParticleManager::g_particule_position_size_data = new GLfloat[max_particle_num * 4];
GLubyte* ParticleManager::g_particule_color_data = new GLubyte[max_particle_num * 4];

const GLfloat ParticleManager::g_vertex_buffer_data[] = {
	-0.5f, -0.5f, 0.0f,
	0.5f, -0.5f, 0.0f,
	-0.5f,  0.5f, 0.0f,
	0.5f,  0.5f, 0.0f,
};