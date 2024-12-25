//
//  main.cpp
//  playground
//
//  Created by Shawn Best on 2024-12-22.
//
// ---------------------------------------------------------------------------------------

//--------------------------------
#include <stdio.h>

//#include <cstdio>
#include "3rdparty/portaudio.h"
#include "core/Scene.hpp"
#include "core/SoundObject.hpp"

#define NUM_SECONDS   (5)
#define SAMPLE_RATE   (44100)
#define FRAMES_PER_BUFFER  (64)

class PaWrapper
{
public:
    PaWrapper() : stream(0)
    {
        scene = new Scene(SAMPLE_RATE, 2);
    }

    Scene* GetScene(void) { return scene; }
    
    bool open(PaDeviceIndex index)
    {
        PaStreamParameters outputParameters;

        outputParameters.device = index;
        if (outputParameters.device == paNoDevice) {
            return false;
        }

        const PaDeviceInfo* pInfo = Pa_GetDeviceInfo(index);
        if (pInfo != 0)
        {
            printf("Output device name: '%s'\r", pInfo->name);
        }

        outputParameters.channelCount = 2;       /* stereo output */
        outputParameters.sampleFormat = paFloat32; /* 32 bit floating point output */
        outputParameters.suggestedLatency = Pa_GetDeviceInfo( outputParameters.device )->defaultLowOutputLatency;
        outputParameters.hostApiSpecificStreamInfo = NULL;

        PaError err = Pa_OpenStream(
            &stream,
            NULL, /* no input */
            &outputParameters,
            SAMPLE_RATE,
            paFramesPerBufferUnspecified,
            paClipOff,      /* we won't output out of range samples so don't bother clipping them */
            &PaWrapper::paCallback,
            this            /* Using 'this' for userData so we can cast to Sine* in paCallback method */
            );

        if (err != paNoError)
        {
            /* Failed to open stream to device !!! */
            return false;
        }

        err = Pa_SetStreamFinishedCallback( stream, &PaWrapper::paStreamFinished );

        if (err != paNoError)
        {
            Pa_CloseStream( stream );
            stream = 0;

            return false;
        }

        return true;
    }

    bool close()
    {
        if (stream == 0)
            return false;

        PaError err = Pa_CloseStream( stream );
        stream = 0;

        return (err == paNoError);
    }


    bool start()
    {
        if (stream == 0)
            return false;

        PaError err = Pa_StartStream( stream );

        return (err == paNoError);
    }

    bool stop()
    {
        if (stream == 0)
            return false;

        PaError err = Pa_StopStream( stream );

        return (err == paNoError);
    }

private:
    int paCallbackMethod(const void *inData, void *outData, unsigned long numFrames, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags)
    {
        (void) statusFlags;

        scene->ProcessBuffer((float *)inData, (float *)outData, (int)numFrames, timeInfo->currentTime);
        return paContinue;
    }

    /* This routine will be called by the PortAudio engine when audio is needed.
    ** It may called at interrupt level on some machines so don't do anything
    ** that could mess up the system like calling malloc() or free().
    */
    static int paCallback( const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void *userData )
    {
        return ((PaWrapper*)userData)->paCallbackMethod(inputBuffer, outputBuffer, framesPerBuffer, timeInfo, statusFlags);
    }


    void paStreamFinishedMethod()
    {
        printf( "Stream Finished\n" );
    }

    /*
     * This routine is called by portaudio when playback is done.
     */
    static void paStreamFinished(void* userData)
    {
        return ((PaWrapper*)userData)->paStreamFinishedMethod();
    }

    PaStream *stream;
    Scene *scene;
};


int main(int argc, char* argv[])
{
    PaWrapper wrapper;

    auto result = Pa_Initialize();
    if (result != paNoError )
    {
        fprintf( stderr, "An error occurred while using the portaudio stream\n" );
        fprintf( stderr, "Error number: %d    ( %s )\n", result, Pa_GetErrorText( result));
        return 1;
    }

    wrapper.GetScene()->CreateBuildingBlock("SoundObject", "sin1");
    if (wrapper.open(Pa_GetDefaultOutputDevice()))
    {
        if (wrapper.start())
        {
            printf(" hacky temp polling loop.... hit x<Enter> to break a s start/stop \n");
            char ch = 0;
            while(ch != 'x')
            {
                ch = getchar();
                if(ch == 'a')
                    wrapper.GetScene()->Start();
                if(ch == 's')
                    wrapper.GetScene()->Stop();
            }

            wrapper.stop();
        }

        wrapper.close();
    }
    Pa_Terminate();
    return paNoError;
}
