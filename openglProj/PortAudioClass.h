#pragma once
#include "portaudio.h"
#include <thread>
#include <map>
#include <vector>

#define PA_SAMPLE_TYPE  paFloat32
typedef float SAMPLE;
#define SAMPLE_SILENCE  (0.0f)
#define PRINTF_S_FORMAT "%.8f"
#define SAMPLE_RATE  (44100)
#define FRAMES_PER_BUFFER (512)
#define NUM_SECONDS     (2)
#define NUM_CHANNELS    (2)
#define DITHER_FLAG     (0)

typedef struct
{
	int          frameIndex;  /* Index into sample array. */
	int          maxFrameIndex;
	SAMPLE      *recordedSamples;
}
paTestData;

//typedef int(*callBackFunctionPtr) (const void *, void *, unsigned long, const PaStreamCallbackTimeInfo*, PaStreamCallbackFlags, void*);


/* This routine will be called by the PortAudio engine when audio is needed.
** It may be called at interrupt level on some machines so don't do anything
** that could mess up the system like calling malloc() or free().
*/
static int s_playCallback(const void *inputBuffer, void *outputBuffer,
	unsigned long framesPerBuffer,
	const PaStreamCallbackTimeInfo* timeInfo,
	PaStreamCallbackFlags statusFlags,
	void *userData)
{
	paTestData *data = (paTestData*)userData;
	SAMPLE *rptr = &data->recordedSamples[data->frameIndex * NUM_CHANNELS];
	SAMPLE *wptr = (SAMPLE*)outputBuffer;
	unsigned int i;
	int finished;
	unsigned int framesLeft = data->maxFrameIndex - data->frameIndex;

	(void)inputBuffer; /* Prevent unused variable warnings. */
	(void)timeInfo;
	(void)statusFlags;
	(void)userData;

	if (framesLeft < framesPerBuffer)
	{
		/* final buffer... */
		for (i = 0; i<framesLeft; i++)
		{
			*wptr++ = *rptr++;  /* left */
			if (NUM_CHANNELS == 2) *wptr++ = *rptr++;  /* right */
		}
		for (; i<framesPerBuffer; i++)
		{
			*wptr++ = 0;  /* left */
			if (NUM_CHANNELS == 2) *wptr++ = 0;  /* right */
		}
		data->frameIndex += framesLeft;
		finished = paComplete;
	}
	else
	{
		for (i = 0; i<framesPerBuffer; i++)
		{
			*wptr++ = *rptr++;  /* left */
			if (NUM_CHANNELS == 2) *wptr++ = *rptr++;  /* right */
		}
		data->frameIndex += framesPerBuffer;
		finished = paContinue;
	}
	return finished;
}


class SampleInfo {
public:
	SAMPLE * sample;
	int maxFrameIndex;

	SampleInfo(SAMPLE * sample, int maxFrameIndex)
		: sample(sample), maxFrameIndex(maxFrameIndex)
	{
	}
};


void thread_function(PaStreamParameters * outputParameter, SampleInfo * sampleInfo);



class PortAudioClass {
public:

	PaStreamParameters  inputParameters,
		outputParameters;
	PaError             err = paNoError;
	
	int                 i;
	int                 totalFrames;
	int                 numSamples;
	int                 numBytes;
	SAMPLE              max, val;
	double              average;

	std::map<std::string, SampleInfo*> loadedSampleFilesMap;


	void init() {
		initOutputDevice();

	}

	int initOutputDevice() {
		outputParameters.device = Pa_GetDefaultOutputDevice(); /* default output device */
		if (outputParameters.device == paNoDevice) {
			fprintf(stderr, "Error: No default output device.\n");
			return -1;
		}
		outputParameters.channelCount = 2;                     /* stereo output */
		outputParameters.sampleFormat = PA_SAMPLE_TYPE;
		outputParameters.suggestedLatency = Pa_GetDeviceInfo(outputParameters.device)->defaultLowOutputLatency;
		outputParameters.hostApiSpecificStreamInfo = NULL;
		return 0;
	}

	void loadSamplefiles() {
		std::vector<std::pair<std::string, std::string>> sampleFilesName = { std::make_pair("laserSound", "sound\recorded.raw") };
		for (std::pair<std::string, std::string> elem : sampleFilesName) {
			{
				FILE  *fid;
				fid = fopen(elem.second.c_str(), "rb");
				if (fid == NULL)
				{
					printf("Could not open file.");
				}
				else
				{
					fseek(fid, 0, SEEK_END);
					long fileSizeL = ftell(fid);
					rewind(fid);
					SAMPLE* sampleStroage = (SAMPLE*)malloc(sizeof(char)*fileSizeL);
					fread(sampleStroage, sizeof(char), sizeof(char)*fileSizeL, fid);

					SampleInfo* sampleInfoPtr = new SampleInfo(sampleStroage, (int)((sizeof(char)*fileSizeL) / (NUM_CHANNELS * sizeof(SAMPLE))));
					loadedSampleFilesMap.insert(std::make_pair(elem.first, sampleInfoPtr));
					fclose(fid);
					printf("Wrote data to 'recorded.raw'\n");
				}
			}
			
		}
	}

	void playSound(std::string soundName) {
		PaStream* paStreamPtr;
		std::map<std::string, SampleInfo*>::iterator it = loadedSampleFilesMap.find(soundName);
		if (it != loadedSampleFilesMap.end()) {
			SampleInfo * sampleInfoPtr = (*it).second;
			std::thread t(thread_function, &outputParameters, sampleInfoPtr);
			t.detach();
		}
	}
};


void thread_function(PaStreamParameters * outputParameter, SampleInfo * sampleInfo)
{
	PaStream* inStream;
	paTestData paData;

	paData.frameIndex = 0;
	paData.recordedSamples = sampleInfo->sample;
	paData.maxFrameIndex = sampleInfo->maxFrameIndex;

	Pa_OpenStream(
		&inStream,
		NULL, /* no input */
		outputParameter,
		SAMPLE_RATE,
		FRAMES_PER_BUFFER,
		paClipOff,      /* we won't output out of range samples so don't bother clipping them */
		s_playCallback,
		&paData);

	if (inStream)
	{
		PaError err = paNoError;
		err = Pa_StartStream(inStream);
		if (err != paNoError)
			return;

		printf("Waiting for playback to finish.\n"); fflush(stdout);

		while ((err = Pa_IsStreamActive(inStream)) == 1) {
			Pa_Sleep(1000);
			printf("W....\n"); fflush(stdout);
			Pa_StartStream(inStream);
		}
		if (err < 0)
			return;

		err = Pa_CloseStream(inStream);
		if (err != paNoError) return;

		printf("Done.\n"); fflush(stdout);

		delete inStream;
	}
}