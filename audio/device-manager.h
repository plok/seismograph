#ifndef SEISMO_DEVICE_MANAGER_H
#define SEISMO_DEVICE_MANAGER_H
//#include "rxcpp/rx.hpp"
//namespace Rx {
//using namespace rxcpp;
//using namespace rxcpp::sources;
//using namespace rxcpp::operators;
//using namespace rxcpp::util;
//}
//using namespace Rx;

#include <portaudio.h>
#include <boost/lockfree/spsc_queue.hpp>
#include <vector>

class DeviceManager {
    
    public:
        DeviceManager();
        void findDevices();
        

        int memberStreamCallback(const void *input, void *output, unsigned long frameCount, const PaStreamCallbackTimeInfo* timeInfo,PaStreamCallbackFlags statusFlags);
        void startStream();
        void read(std::vector<double>& data);

    private:
        static int streamCallback( const void *input, void *output, unsigned long frameCount, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void *userData );

        PaError _error;
        PaStreamParameters _inputParameters;

        boost::lockfree::spsc_queue<double, boost::lockfree::capacity<1024>> _ringbuffer;

        PaStream* _stream;
};

#endif
