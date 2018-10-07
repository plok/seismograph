#include "device-manager.h"
#include <iostream>
DeviceManager::DeviceManager() {
    auto err = Pa_Initialize();
    auto nrofdevices = Pa_GetDeviceCount();
    std::cout << "Device count:" << nrofdevices << std::endl;
    if(nrofdevices <= 0){
        std::cout << "No devices found" << std::endl;
    } // if

    for(int d=0;d<nrofdevices;d++)
    {   
        auto info = Pa_GetDeviceInfo(d);
        std::cout << "Device " << d << "\t" << info->name;
        if(info->maxInputChannels > 0) std::cout << " (IN)";
        if(info->maxOutputChannels > 0) std::cout << " (OUT)";
        std::cout << std::endl;
    } // for
}
