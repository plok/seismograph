#ifndef SEISMO_RENDER_VIEW_H
#define SEISMO_RENDER_VIEW_H

#include "katla/gpu/render/render-view.h"
#include "modules/katla/gpu/render/skia-opengl-binder.h"

#include <chrono>
#include <deque>

class SeismoRenderView : public RenderView
{
public:
    SeismoRenderView();

    void init();
    void render();
    void cleanup();

    void resize(int width, int height);

private:
    SkiaOpenGlBinder _skia;

    bool _lastRenderValid;
    std::chrono::high_resolution_clock::time_point _startTime;
    std::chrono::high_resolution_clock::time_point _gridTime;

    std::deque<double> _buffer;
};

#endif