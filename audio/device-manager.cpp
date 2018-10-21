#include "device-manager.h"
#include <iostream>
#define SAMPLE_RATE  (44100)
#define FRAMES_PER_BUFFER (1024)

/***
 * Static functions
 */
int DeviceManager::streamCallback(const void *input, void *output, unsigned long frameCount,
                                  const PaStreamCallbackTimeInfo *timeInfo, PaStreamCallbackFlags statusFlags,
                                  void *userData) {
    return  ((DeviceManager*)userData)->memberStreamCallback(input, output, frameCount, timeInfo, statusFlags);
}
/****/

DeviceManager::DeviceManager() {

    auto error = Pa_Initialize();
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

    _inputParameters.device = Pa_GetDefaultInputDevice();
    _inputParameters.channelCount = 1;
    _inputParameters.sampleFormat = paFloat32;
    _inputParameters.suggestedLatency = Pa_GetDeviceInfo( _inputParameters.device )->defaultLowInputLatency;
    _inputParameters.hostApiSpecificStreamInfo = NULL;

    this->startStream();

}

void DeviceManager::findDevices() {

}

int DeviceManager::memberStreamCallback( const void *input, void *output,
        unsigned long frameCount,
        const PaStreamCallbackTimeInfo *timeInfo, PaStreamCallbackFlags
        statusFlags)
{
    const float* rawSamples = static_cast<const float*>(input);

    std::vector<double> samples;
    samples.reserve(frameCount);

    for(int i =0; i<frameCount; i++) {
        samples.push_back(static_cast<double>(rawSamples[i]));
    }

    _ringbuffer.push(samples.begin(), samples.end());

    return paContinue;
}

void DeviceManager::startStream() {
    _error = Pa_OpenStream(
            &_stream,
            &_inputParameters,
            NULL,                  /* &outputParameters, */
            SAMPLE_RATE,
            FRAMES_PER_BUFFER,
            paClipOff,      /* we won't output out of range samples so don't bother clipping them */
            DeviceManager::streamCallback,
            this );
    std::cout << "Error: "  << (_error != paNoError)  << std::endl;
    Pa_StartStream( _stream );
    std::cout << "Error: "  << (_error != paNoError)  << std::endl;

}

void DeviceManager::read(std::vector<double>& data) {
    if ( Pa_IsStreamActive(_stream) == 1) {
  //      size_t num_read = 0;
       // while (auto a = _ringbuffer.pop(data.data() + num_read, data->size() - num_read)) {
       //     num_read += a;
       //    if (num_read >= 1024) {
       //         break;
       //     }
       // }

        _ringbuffer.consume_all([&data](double sample) {
            data.push_back(sample);
        });

    }
}
