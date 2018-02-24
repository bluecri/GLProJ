#pragma once
#include <al.h>
#include <alc.h>
#include <ALSound.h>

class ALSource {
public:
	ALuint m_sourceID;
	float m_pitch;
	float m_gain;
	float m_sourcePos[3];
	ALboolean m_isLoop = AL_FALSE;

	ALSource() {
		alGenSources(1, &m_sourceID);
		m_pitch = 1.0f;
		m_gain = 1.0f;
		for (int i = 0; i < 3; i++) {
			m_sourcePos[i] = 0.0f;
		}
		m_isLoop = AL_FALSE;

		alSourcef(m_sourceID, AL_PITCH, m_pitch);
		alSourcef(m_sourceID, AL_GAIN, m_gain);
		alSourcefv(m_sourceID, AL_POSITION, m_sourcePos);
		alSourcei(m_sourceID, AL_LOOPING, m_isLoop);
		alSourcei(m_sourceID, AL_MAX_DISTANCE, 10.0f);
		alSourcei(m_sourceID, AL_REFERENCE_DISTANCE, 1.0);
		
	}

	ALSource(float pitch, float gain) {
		alGenSources(1, &m_sourceID);
		m_pitch = pitch;
		m_gain = gain;
		for (int i = 0; i < 3; i++) {
			m_sourcePos[i] = 0.0f;
		}
		m_isLoop = AL_FALSE;

		alSourcef(m_sourceID, AL_PITCH, m_pitch);
		alSourcef(m_sourceID, AL_GAIN, m_gain);
		alSourcefv(m_sourceID, AL_POSITION, m_sourcePos);
		alSourcei(m_sourceID, AL_LOOPING, m_isLoop);
		alSourcei(m_sourceID, AL_MAX_DISTANCE, 10.0f);
		alSourcei(m_sourceID, AL_REFERENCE_DISTANCE, 1.0);

	}
	void initSource(ALuint soundBufferID) {
		bindSourceToBuffer(soundBufferID);
	}

	void bindSourceToBuffer(ALuint soundBufferID) {
		alSourcei(m_sourceID, AL_BUFFER, soundBufferID);
	}
	void bindSourceToALSound(ALSound *alSound) {
		alSourcei(m_sourceID, AL_BUFFER, alSound->soundBufferID);
	}

	void modifyPos(glm::vec3 pos) {
		for (int i = 0; i < 3; i++) {
			m_sourcePos[i] = pos[i];
		}
		alSourcefv(m_sourceID, AL_POSITION, m_sourcePos);
	}

	void sourcePlay() {
		alSourcePlay(m_sourceID);
	}
	void sourceStop() {
		alSourceStop(m_sourceID);
	}

	~ALSource(){
		alDeleteSources(1, &m_sourceID);
	}
};