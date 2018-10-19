#ifndef SEISMO_SCENE_H
#define SEISMO_SCENE_H

#include "katla/gpu/render/render-view.h"
#include "modules/katla/gpu/render/skia-opengl-binder.h"
#include "audio/device-manager.h"
#include <chrono>
#include <deque>

class SeismoScene : public Scene
{
public:
    SeismoScene();

    void init() override;
    void render() override;
    void cleanup() override;

    void resize(int width, int height) override;

private:
    SkiaOpenGlBinder _skia;
    DeviceManager deviceManager;
    bool _lastRenderValid;
    std::chrono::high_resolution_clock::time_point _startTime;
    std::chrono::high_resolution_clock::time_point _gridTime;

    std::deque<double> _buffer;
};

#endif
