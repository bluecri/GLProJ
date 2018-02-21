#pragma once
#include <al.h>
#include <alc.h>

#include <stdio.h>
#include <iostream>

using namespace std;

class ALSound {
public:
	ALuint soundBufferID;
	ALuint sourceID;
	
	ALvoid	*data = 0;
	ALsizei size = 0;
	ALenum format = 0;
	ALsizei freq = 0;
	
	ALSound(ALvoid	*data, ALsizei size, ALenum format, ALsizei freq)
	: data(data), size(size), format(format), freq(freq){
		alGenSources(1, &sourceID);
		alGenBuffers(1, &soundBufferID);

		alBufferData(soundBufferID, format, data, size, freq);
		alSourcei(sourceID, AL_BUFFER, soundBufferID);
		//1 listener
		//1 buffer per file
		//multiple source with sound origin

		//listener
		/*
		alListenerfv(AL_POSITION, ListenerPos);
		alListenerfv(AL_VELOCITY, ListenerVel);
		alListenerfv(AL_ORIENTATION, ListenerOri);
		*/
		//source
		/*
		alSourcei(sourceID, AL_BUFFER, soundBufferID);
		alSourcef(sourceID, AL_PITCH, 1.0f);
		alSourcef(sourceID, AL_GAIN, 1.0f);
		alSourcefv(sourceID, AL_POSITION, SourcePos);
		alSourcefv(sourceID, AL_POSITION, SourcePos);
		alSourcei(sourceID, AL_LOOPING, AL_FALSE);
		*/

		//play
		alSourcePlay(sourceID);

	}

	virtual ~ALSound() {
		alDeleteBuffers(1, &soundBufferID);
		alDeleteSources(1, &sourceID);

		delete data;
	}

	
};