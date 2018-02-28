#pragma once
#include <al.h>
#include <alc.h>

#include <stdio.h>
#include <iostream>

using namespace std;

/*
 * contain soundBufferID & information of file
*/

class ALSound {
public:
	
	ALuint soundBufferID;
	std::string m_soundName;
	std::string fileName;
	ALvoid	*data = 0;
	ALsizei size = 0;
	ALenum format = 0;
	ALsizei freq = 0;
	
	ALSound(std::string fileName, ALvoid *data, ALsizei size, ALenum format, ALsizei freq)
	: data(data), size(size), format(format), freq(freq), fileName(fileName){

	}

	void allocBuffer(std::string soundName) {
		alGenBuffers(1, &soundBufferID);
		m_soundName = soundName;
		alBufferData(soundBufferID, format, data, size, freq);

	}

	virtual ~ALSound() {
		alDeleteBuffers(1, &soundBufferID);
		//alDeleteSources(1, &sourceID);

		delete data;
	}

	
};