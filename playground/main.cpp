//
//  main.cpp
//  playground
//
//  Created by Shawn Best on 2024-12-22.
//
// ---------------------------------------------------------------------------------------

//--------------------------------
#include <stdio.h>

#include "3rdparty/portaudio.h"
#include "CommandProcessor.hpp"
#include "core/Scene.hpp"
#include "core/Event.hpp"

#define SAMPLE_RATE   (44100)
#define FRAMES_PER_BUFFER  (64)

Scene* gScene = nullptr;
CommandProcessor *gCmdProc = nullptr;

class PaWrapper
{
public:
    PaWrapper() : stream(0)
    {
        
    }
    
    //Scene* GetScene(void) { return scene; }
    
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

    PaTime time()
    {
        return Pa_GetStreamTime( stream );
    }

private:
    int paCallbackMethod(const void *inData, void *outData, unsigned long numFrames, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags)
    {
        (void) statusFlags;

        gScene->ProcessBuffer((float *)inData, (float *)outData, (int)numFrames, timeInfo->currentTime);
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
    //Scene *scene;
};

void InitScene(void)
{
    gScene = new Scene(SAMPLE_RATE, 2);
    gCmdProc = new CommandProcessor(gScene);

    gCmdProc->ProcessCommandString("create SineGenerator sin1");
    gCmdProc->ProcessCommandString("create Envelope adsr1");

    gCmdProc->ProcessCommandString("connect sin1 outputLeft adsr1 inputLeft");
    gCmdProc->ProcessCommandString("connect sin1 outputRight adsr1 inputRight");
    //scene->CreateComponent("SineGenerator", "sin1");
    //scene->CreateComponent("Envelope", "adsr1");

    //scene->Connect("sin1", "outputLeft", "adsr1", "inputLeft");
    //scene->Connect("sin1", "outputRight", "adsr1", "inputRight");

    // TODO:
    gScene->ConnectOutput("adsr1", "outputLeft", 0);
    gScene->ConnectOutput("adsr1", "outputRight", 1);
}

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

    InitScene();
    
    if (wrapper.open(Pa_GetDefaultOutputDevice()))
    {
        if (wrapper.start())
        {
            printf(" hacky temp polling loop.... hit x<Enter> to break a s start/stop \n");
            char ch = 0;
            uint8_t pitch = 32;
            float freqRatio = 1.0;
            while(ch != 'x')
            {
                ch = getchar();
                if(ch == 'a')
                {
                    NoteOnEvent event(0, ++pitch, 164);
                    gScene->HandleEvent(&event, (double)wrapper.time());
                }
                
                if(ch == 's')
                {
                    NoteOffEvent event(0, pitch);
                    gScene->HandleEvent(&event, (double)wrapper.time());
                }

                if (ch == 'd')
                {
                    freqRatio += 0.2;
                    gScene->SetParameter("sin1", "freqRatio", &freqRatio);
                }
            }

            wrapper.stop();
        }

        wrapper.close();
    }
    Pa_Terminate();
    return paNoError;
}
