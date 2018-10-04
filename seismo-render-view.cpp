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
    _skia.init();

    _startTime = std::chrono::high_resolution_clock::now();
}

void SeismoRenderView::render()
{
    auto surface = _skia.surface();
    if (!surface) {
        return;
    }

    if (_lastRenderValid) {

    }

    int width = surface->width();
    int height = surface->height();

    SkCanvas* canvas = surface->getCanvas();
    
    canvas->clear(SK_ColorBLACK);

    SkPaint gridPaint;
    gridPaint.setColor(0x20FFFFFF);
    
    SkPaint foregroundPaint;
    foregroundPaint.setColor(0xFFFFFFFF);

    const float gridSize = (float)width / 10.f;

    auto currentTime = std::chrono::high_resolution_clock::now();
    auto timeSinceStart = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - _startTime);
    
    auto timeInPeriod = ((timeSinceStart.count() % 1000) / 1000.f);
    auto offset = gridSize * timeInPeriod;
    // std::cout << (msSinceEpoch.count() % 1000) << offset << std::endl;
    
    for(float x=0; x<width; x += gridSize) {
        SkPoint up {offset + x, 0};
        SkPoint down {offset + x, height-10.f};
        
        canvas->drawLine(up, down, gridPaint);
    }
    for(float y=0; y<height; y += gridSize) {
        SkPoint left {0, y};
        SkPoint right {width-10.f, y};
        
        canvas->drawLine(left, right, gridPaint);
    }  

    float timeScale = (1000.f/gridSize)/((float)M_PI);
    float t = timeScale * ((float)timeSinceStart.count()/1000.f);

    float timespan = (1.f / 0.017) * 10; 

    auto ampl = (float)height/2;
    auto yOffset = ampl;
    auto value = ampl * sin(t) + yOffset;

    _buffer.push_front(value);
    if (_buffer.size() > 600) {
        _buffer.pop_back();
    }

    float lastX = 0;
    float lastY = 0;
    for(float t=0; t<_buffer.size(); t++) {
        if (t >= _buffer.size()) {
            continue;
        }

        auto x = t * (width/timespan);
        auto y = (float)_buffer[t];

        if (t>0) {
            SkPoint last {lastX, lastY};
            SkPoint point {x, y};
            canvas->drawLine(last, point, foregroundPaint);
        }

        lastX = x;
        lastY = y;
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
    