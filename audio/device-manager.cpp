#include "device-manager.h"
#include <iostream>
#define SAMPLE_RATE  (44100)
#define FRAMES_PER_BUFFER (1024)

DeviceManager::DeviceManager() {

    auto err = Pa_Initialize();
    auto nrofdevices = Pa_GetDeviceCount();

    std::cout << "Device count:" << nrofdevices << std::endl;

    if(nrofdevices <= 0){
        std::cout << "No devices found" << std::endl;
    }

    for(int d=0;d<nrofdevices;d++)
    {   
        auto info = Pa_GetDeviceInfo(d);
        std::cout << "Device " << d << "\t" << info->name;
        if(info->maxInputChannels > 0) std::cout << " (IN)";
        if(info->maxOutputChannels > 0) std::cout << " (OUT)";
        std::cout << std::endl;
    }
    inputParameters.device = Pa_GetDefaultInputDevice();
    inputParameters.channelCount = 1;
    inputParameters.sampleFormat = paFloat32;
    inputParameters.suggestedLatency = Pa_GetDeviceInfo( inputParameters.device )->defaultLowInputLatency;
    inputParameters.hostApiSpecificStreamInfo = NULL;
    this->startStream();

}

void DeviceManager::findDevices() {

}

int DeviceManager::memberStreamCallback( const void *input, void *output,
        unsigned long frameCount,
        const PaStreamCallbackTimeInfo *timeInfo, PaStreamCallbackFlags
        statusFlags)
{
    const float *rptr = (const float*)input;
    this->ringbuffer.push(rptr,frameCount);
    return paContinue;
}

void DeviceManager::startStream() {
    err = Pa_OpenStream(
            &stream,
            &inputParameters,
            NULL,                  /* &outputParameters, */
            SAMPLE_RATE,
            FRAMES_PER_BUFFER,
            paClipOff,      /* we won't output out of range samples so don't bother clipping them */
            DeviceManager::streamCallback,
            this );
    std::cout << "Error: "  << (err != paNoError)  << std::endl;
    Pa_StartStream( stream );
    std::cout << "Error: "  << (err != paNoError)  << std::endl;

}

void DeviceManager::read(std::vector<float> * data) {
    if ( Pa_IsStreamActive(stream) == 1) {
        size_t num_read = 0;
        while (auto a = ringbuffer.pop(data->data() + num_read, data->size() - num_read)) {
            num_read += a;
            if (num_read >= 1024) {
                break;
            }
        }
    }
}
