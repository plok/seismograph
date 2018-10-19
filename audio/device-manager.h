#ifndef SEISMO_DEVICE_MANAGER_H
#define SEISMO_DEVICE_MANAGER_H
#include "rxcpp/rx.hpp"
namespace Rx {
using namespace rxcpp;
using namespace rxcpp::sources;
using namespace rxcpp::operators;
using namespace rxcpp::util;
}
using namespace Rx;

#include <portaudio.h>
#include <boost/lockfree/spsc_queue.hpp>

class DeviceManager {
    
    public:
        DeviceManager();
        void findDevices();
        
        static int streamCallback( const void *input, void *output, unsigned long frameCount, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void *userData )
        {
            return  ((DeviceManager*)userData)->memberStreamCallback(input, output, frameCount,timeInfo, statusFlags);
        }

        int memberStreamCallback(const void *input, void *output, unsigned long frameCount, const PaStreamCallbackTimeInfo* timeInfo,PaStreamCallbackFlags statusFlags);
        void startStream();
        void read(std::vector<float> * data);

    private:
        PaError err;
        PaStreamParameters inputParameters;
        boost::lockfree::spsc_queue<float, boost::lockfree::capacity<1024>> ringbuffer;
        PaStream *stream;
};

#endif
