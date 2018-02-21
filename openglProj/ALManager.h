#pragma once

#include <al.h>
#include <alc.h>
#include <ALSound.h>
#include <iostream>
#include <stdio.h>

using namespace std;

class ALManager {
public:
	ALCdevice	*alcDevice;
	ALCcontext	*alCcontext;

	void init() {
		alcDevice = alcOpenDevice(NULL);
		if (alcDevice) {
			alCcontext = alcCreateContext(alcDevice, NULL);
			alcMakeContextCurrent(alCcontext);
		}
	}
	

	//reference : https://www.gamedev.net/forums/topic/645923-loading-wav-file-with-openal-incorrect-audioformat/
	ALSound* loadWAVE(const char* filename) {
		FILE* fp = NULL;

		fp = fopen(filename, "r");
		if (!fp) {
			cout << "Could NOT open: " << filename << "!" << endl;
			fclose(fp);
			return NULL;
		}

		char* ChunkID = new char[4];
		fread(ChunkID, sizeof(char), 4, fp);

		if (strcmp(ChunkID, "RIFF")) {
			delete[] ChunkID;
			cout << "Not RIFF!" << endl;
			fclose(fp);
			return NULL;
		}

		fseek(fp, 8, SEEK_SET);
		char* Format = new char[4];
		fread(Format, 4, sizeof(char), fp);

		if (strcmp(Format, "WAVE")) {
			delete[] ChunkID;
			delete[] Format;
			cout << "Not WAVE!" << endl;
			fclose(fp);
			return NULL;
		}

		char* SubChunk1ID = new char[4];
		fread(SubChunk1ID, 4, sizeof(char), fp);

		if (strcmp(SubChunk1ID, "fmt ")) {
			delete[] ChunkID;
			delete[] Format;
			delete[] SubChunk1ID;
			cout << "Corrupt SubChunk1ID!" << endl;
			fclose(fp);
			return NULL;
		}

		unsigned int SubChunk1Size;
		fread(&SubChunk1Size, 1, sizeof(unsigned int), fp);
		unsigned int SubChunk2Location = (unsigned int)ftell(fp) + SubChunk1Size;

		// -------------------------------------- THIS PART

		unsigned short AudioFormat;
		fread(&AudioFormat, 1, sizeof(unsigned short), fp);

		if (AudioFormat != 1) { // AudioFormat = 85, should be 1
			delete[] ChunkID;
			delete[] Format;
			delete[] SubChunk1ID;
			cout << "Audio is NOT PCM!" << endl;
			fclose(fp);
			return NULL;
		}

		// --------------------------------------

		unsigned short NumChannels;
		fread(&NumChannels, 1, sizeof(unsigned short), fp);
		unsigned int SampleRate;
		fread(&SampleRate, 1, sizeof(unsigned int), fp);

		fseek(fp, 34, SEEK_SET);

		unsigned short BitsPerSample;
		fread(&BitsPerSample, 1, sizeof(unsigned short), fp);

		int ALFormat;
		if (NumChannels == 1 && BitsPerSample == 8) {
			ALFormat = AL_FORMAT_MONO8;
		}
		else if (NumChannels == 1 && BitsPerSample == 16) {
			ALFormat = AL_FORMAT_MONO16;
		}
		else if (NumChannels == 2 && BitsPerSample == 8) {
			ALFormat = AL_FORMAT_STEREO8;
		}
		else if (NumChannels == 2 && BitsPerSample == 16) {
			ALFormat = AL_FORMAT_STEREO16;
		}
		else {
			delete[] ChunkID;
			delete[] Format;
			delete[] SubChunk1ID;
			cout << "Audio is Not correctly formatted!" << endl;
			fclose(fp);
			return NULL;
		}

		fseek(fp, SubChunk2Location, SEEK_SET);
		char* SubChunk2ID = new char[4];
		fread(SubChunk2ID, 4, sizeof(char), fp);

		if (strcmp(SubChunk2ID, "data")) {
			delete[] ChunkID;
			delete[] Format;
			delete[] SubChunk1ID;
			delete[] SubChunk2ID;
			cout << "Corrupt SubChunk2ID!" << endl;
			fclose(fp);
			return NULL;
		}

		unsigned int SubChunk2Size;
		fread(&SubChunk2Size, 1, sizeof(unsigned int), fp);

		unsigned char* Data = new unsigned char[SubChunk2Size];
		fread(Data, SubChunk2Size, sizeof(unsigned char), fp);

		delete[] ChunkID;
		delete[] Format;
		delete[] SubChunk1ID;
		delete[] SubChunk2ID;
		fclose(fp);

		return new ALSound(Data, SubChunk2Size, ALFormat, SampleRate);

	}

	void text() {
		ALuint soundBufferID;
		alGenBuffers(1, &soundBufferID);
		ALenum format = 0;
		ALvoid	*data = 0;
		ALsizei size = 0;
		ALsizei freq = 0;

		loadWAVFile("test.wav", &format, &data, &size, &freq, &loop);
		alBufferData(soundBufferID, format, data, size, freq);

		ALuint sourceID;
		alGenSources(1, &sourceID);
		
		alcMakeContextCurrent(NULL);
		alcDestroyContext(alCcontext);
		alcCloseDevice(alcDevice);
	}
	
};