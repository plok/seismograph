#ifndef SEISMO_RENDER_VIEW_H
#define SEISMO_RENDER_VIEW_H

// TODO drawing -> render
#include "katla/gpu/drawing/render-view.h"

#include "core/SkSurface.h"
#include "gpu/GrContext.h"

#include <chrono>

class SeismoRenderView : public RenderView
{
public:
    SeismoRenderView();

    void init();
    void render();
    void cleanup();

    void resize(int width, int height);

private:
    sk_sp<GrContext> m_context;
    sk_sp<SkSurface> m_surface;

    bool _lastRenderValid;
    std::chrono::high_resolution_clock::time_point _gridTime;
};

#endif