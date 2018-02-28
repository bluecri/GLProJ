#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <list>

#include "src/glMain/shader/ShaderParticle.h"
#include "src/glMain/objectAndTexture/texture.h"

#include "./ParticleSystem.h"
#include "./ParticleInfo.h"


class ParticleManager {
public:
	ParticleSystem * particleSystemPtr;

	const static int max_particle_num;
	static GLfloat* g_particule_position_size_data;
	static GLubyte* g_particule_color_data;
	static const GLfloat g_vertex_buffer_data[12];

	std::list<ParticleInfo*> particleInfoPtrList;	//particle info num

	GLuint particleVertexID;
	ParticleManager() {
		particleSystemPtr = new ParticleSystem(max_particle_num);
	}


	GLuint billboard_vertex_buffer;
	GLuint particles_position_buffer;
	GLuint particles_color_buffer;

	ShaderParticle *shaderParticlePtr;

	int particlesCountForBuffer = 0;

	GLuint particleCommonTexture;

	glm::mat4 m_viewMatrix;
	glm::mat4 m_viewProjectionMatrix;

	void init() {
		shaderParticlePtr = new ShaderParticle("shader/Particle.vertexshader", "shader/Particle.fragmentshader");
		particleCommonTexture = loadDDS("texture/particle.DDS");
	}

	ParticleInfo* registerNewParticleInfo() {
		ParticleInfo* tempParticleInfoPtr = new ParticleInfo();
		particleInfoPtrList.push_back(tempParticleInfoPtr);
		return tempParticleInfoPtr;
	}

	ParticleInfo* registerNewParticleInfo(ParticleInfo* pInfo) {
		particleInfoPtrList.push_back(pInfo);
		return pInfo;
	}

	void bufferInit() {

		glGenBuffers(1, &billboard_vertex_buffer);
		glBindBuffer(GL_ARRAY_BUFFER, billboard_vertex_buffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

		// The VBO containing the positions and sizes of the particles
		glGenBuffers(1, &particles_position_buffer);
		glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
		// Initialize with empty (NULL) buffer : it will be updated later, each frame.
		glBufferData(GL_ARRAY_BUFFER, max_particle_num * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);

		// The VBO containing the colors of the particles
		glGenBuffers(1, &particles_color_buffer);
		glBindBuffer(GL_ARRAY_BUFFER, particles_color_buffer);
		// Initialize with empty (NULL) buffer : it will be updated later, each frame.
		glBufferData(GL_ARRAY_BUFFER, max_particle_num * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW);
	
		
		glGenVertexArrays(1, &particleVertexID);
		glBindVertexArray(particleVertexID);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		glBindBuffer(GL_ARRAY_BUFFER, billboard_vertex_buffer);
		glVertexAttribPointer(
			0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		// 2nd attribute buffer : positions of particles' centers
		glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
		glVertexAttribPointer(
			1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
			4,                                // size : x + y + z + size => 4
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);

		// 3rd attribute buffer : particles' colors
		glBindBuffer(GL_ARRAY_BUFFER, particles_color_buffer);
		glVertexAttribPointer(
			2,                                // attribute. No particular reason for 1, but must match the layout in the shader.
			4,                                // size : r + g + b + a => 4
			GL_UNSIGNED_BYTE,                 // type
			GL_TRUE,                          // normalized?    *** YES, this means that the unsigned char[4] will be accessible with a vec4 (floats) in the shader ***
			0,                                // stride
			(void*)0                          // array buffer offset
		);

		// These functions are specific to glDrawArrays*Instanced*.
		// The first parameter is the attribute buffer we're talking about.
		// The second parameter is the "rate at which generic vertex attributes advance when rendering multiple instances"
		// http://www.opengl.org/sdk/docs/man/xhtml/glVertexAttribDivisor.xml
		glVertexAttribDivisor(0, 0); // particles vertices : always reuse the same 4 vertices -> 0
		glVertexAttribDivisor(1, 1); // positions : one per quad (its center)                 -> 1
		glVertexAttribDivisor(2, 1); // color : one per quad                                  -> 1

		glBindVertexArray(0);
	}

	void adjustNewParticles() {
		//for particleinfo¸¸Å­
		for (ParticleInfo* elem : particleInfoPtrList) {
			if (elem->isOneParticlePerMultiFrame) {
				if (elem->frameVsParticle != 0 && elem->countFrame >= elem->frameVsParticle) {
					int particleIndex = particleSystemPtr->FindUnusedParticle();
					Particle &refTargetParticle = particleSystemPtr->particlesContainer[particleIndex];
					elem->createNewParticle(refTargetParticle);
					elem->countFrame = 0;
				}
				else {
					elem->countFrame += 1;
				}
			}
			else {
				for (int num = 0; num < elem->frameVsParticle; num++) {
					int particleIndex = particleSystemPtr->FindUnusedParticle();
					Particle &refTargetParticle = particleSystemPtr->particlesContainer[particleIndex];
					elem->createNewParticle(refTargetParticle);
				}
			}
			
		}
	}

	void updateAndRegisterParticles(float deltaTime, glm::vec3 cameraPos) {
		for (ParticleInfo* elem : particleInfoPtrList) {
			elem->preProcessBeforeUpdate(deltaTime, cameraPos);
		}

		// Simulate all particles
		particlesCountForBuffer = 0;	//use in drawParticles count
		for (int i = 0; i<max_particle_num; i++) {

			Particle* p = &(particleSystemPtr->particlesContainer[i]); // shortcut

			if (p->life > 0.0f) {
				// Decrease life
				p->life -= deltaTime;
				if (p->life > 0.0f) {
					ParticleInfo* tempParticleInfoPtr = p->particleInfoPtr;
					tempParticleInfoPtr->updateParticle(p);
					//p.particleInfoPtr->updateParticle(p);
					
					// Fill the GPU buffer
					g_particule_position_size_data[4 * particlesCountForBuffer + 0] = p->pos.x;
					g_particule_position_size_data[4 * particlesCountForBuffer + 1] = p->pos.y;
					g_particule_position_size_data[4 * particlesCountForBuffer + 2] = p->pos.z;
					g_particule_position_size_data[4 * particlesCountForBuffer + 3] = p->size;

					g_particule_color_data[4 * particlesCountForBuffer + 0] = p->color[0];
					g_particule_color_data[4 * particlesCountForBuffer + 1] = p->color[1];
					g_particule_color_data[4 * particlesCountForBuffer + 2] = p->color[2];
					g_particule_color_data[4 * particlesCountForBuffer + 3] = p->color[3];
				}
				else {
					// Particles that just died will be put at the end of the buffer in SortParticles();
					p->cameradistance = -1.0f;
				}
				particlesCountForBuffer++;	
			}
		}
		particleSystemPtr->sortParticlesContainer();
	}
	void setUniform(glm::mat4 viewMatrix, glm::mat4 projMatrix) {
		m_viewMatrix = viewMatrix;
		m_viewProjectionMatrix = projMatrix * viewMatrix;
	}

	void drawParticles() {
		glBindVertexArray(particleVertexID);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// Use our shader
		glUseProgram(shaderParticlePtr->getShaderID());

		glActiveTexture(GL_TEXTURE8);
		glBindTexture(GL_TEXTURE_2D, particleCommonTexture);
		// Set our "myTextureSampler" sampler to use Texture Unit 0
		glUniform1i(shaderParticlePtr->m_textureID, 8);

		glUniform3f(shaderParticlePtr->m_cameraRight_worldspace_ID, m_viewMatrix[0][0], m_viewMatrix[1][0], m_viewMatrix[2][0]);
		glUniform3f(shaderParticlePtr->m_cameraUp_worldspace_ID, m_viewMatrix[0][1], m_viewMatrix[1][1], m_viewMatrix[2][1]);
		glUniformMatrix4fv(shaderParticlePtr->m_viewProjMatrixID, 1, GL_FALSE, &m_viewProjectionMatrix[0][0]);

		glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
		glBufferData(GL_ARRAY_BUFFER, max_particle_num * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.
		glBufferSubData(GL_ARRAY_BUFFER, 0, particlesCountForBuffer * sizeof(GLfloat) * 4, g_particule_position_size_data);

		glBindBuffer(GL_ARRAY_BUFFER, particles_color_buffer);
		glBufferData(GL_ARRAY_BUFFER, max_particle_num * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.
		glBufferSubData(GL_ARRAY_BUFFER, 0, particlesCountForBuffer * sizeof(GLubyte) * 4, g_particule_color_data);

		glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, particlesCountForBuffer);
		glDisable(GL_BLEND);
		glBindVertexArray(0);
		
	}
};
