
#ifndef SEISMO_DEVICE_MANAGER_H
#define SEISMO_DEVICE_MANAGER_H
#include <portaudio.h>

class DeviceManager {
    
    public:
        DeviceManager();
        void findDevices();
};

#endif
