#include "seismo-render-view.h"

#include <GLFW/glfw3.h>

#include "gpu/GrContext.h"
#include "gpu/GrRenderTarget.h"
#include "gpu/gl/GrGLInterface.h"

#include "core/SkData.h"
#include "core/SkImage.h"
#include "core/SkStream.h"
#include "core/SkSurface.h"
#include "core/SkCanvas.h"

#include "GrBackendSurface.h"

#include <iostream>

SeismoRenderView::SeismoRenderView()
{
}

void SeismoRenderView::init()
{
    auto interface = GrGLMakeNativeInterface();
    
    m_context = GrContext::MakeGL(interface);
    
    int width = 800;
    int height = 600;

    // Get the current draw framebuffer 
    GLint drawFboId = 0;
    glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &drawFboId);


    GrGLFramebufferInfo info;
    info.fFBOID = (GrGLuint) drawFboId;
    

    // TODO: detect!
    info.fFormat = GL_RGBA8;
    SkColorType colorType = kRGBA_8888_SkColorType;
    ///

    // Create render target
    GrBackendRenderTarget target(width, height, /*kMsaaSampleCount*/ 0, /*kStencilBits*/ 8, info);


    SkSurfaceProps props(SkSurfaceProps::kLegacyFontHost_InitType);
    

    m_surface = sk_sp<SkSurface>(
        SkSurface::MakeFromBackendRenderTarget(m_context.get(),
                                            target,
                                               kBottomLeft_GrSurfaceOrigin,
                                               colorType,
                                               nullptr,
                                               &props));
    if (!m_surface) {
        SkDebugf("SkSurface::MakeRenderTarget returned null\n");
        return;
    }

}

void SeismoRenderView::render()
{
    if (!m_surface) {
        return;
    }

    if (_lastRenderValid) {

    }

    int width = m_surface->width();
    int height = m_surface->height();

    SkCanvas* canvas = m_surface->getCanvas();
    
    canvas->clear(SK_ColorBLACK);

    SkPaint paint;
    paint.setColor(0x20FFFFFF);
    
    const float gridSize = (float)width / 10.f;

    auto time = std::chrono::high_resolution_clock::now();
    auto msSinceEpoch = std::chrono::duration_cast<std::chrono::milliseconds>(time.time_since_epoch());
    
    auto offset = gridSize * ((msSinceEpoch.count() % 1000) / 1000.f);
    // std::cout << (msSinceEpoch.count() % 1000) << offset << std::endl;
    
    for(float x=0; x<width; x += gridSize) {
        SkPoint up {offset + x, 0};
        SkPoint down {offset + x, height-10.f};
        
        canvas->drawLine(up, down, paint);
    }
    for(float y=0; y<height; y += gridSize) {
        SkPoint left {0, y};
        SkPoint right {width-10.f, y};
        
        canvas->drawLine(left, right, paint);
    }  

    canvas->flush();
}

void SeismoRenderView::resize(int width, int height)
{
    //render();
}

void SeismoRenderView::cleanup()
{
    // TODO?
}
    