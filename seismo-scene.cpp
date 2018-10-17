#include "seismo-scene.h"

#include "core/SkCanvas.h"

SeismoScene::SeismoScene() :
    _lastRenderValid(false)
{}

void SeismoScene::init()
{
    _skia.init();

    _startTime = std::chrono::high_resolution_clock::now();
}

void SeismoScene::render()
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

    const int gridSize = width / 10;

    auto currentTime = std::chrono::high_resolution_clock::now();
    auto timeSinceStart = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - _startTime);
    
    auto timeInPeriod = ((timeSinceStart.count() % 1000) / 1000.f);
    auto offset = gridSize * timeInPeriod;

    for(int x=0; x<width; x += gridSize) {
        auto fx = static_cast<float>(x);

        SkPoint up {offset + fx, 0};
        SkPoint down {offset + fx, height-10.f};
        
        canvas->drawLine(up, down, gridPaint);
    }
    for(int y=0; y<height; y += gridSize) {
        auto fy = static_cast<float>(y);

        SkPoint left {0, fy};
        SkPoint right {width-10.f, fy};
        
        canvas->drawLine(left, right, gridPaint);
    }  

    float timeScale = (1000.f/gridSize)/((float)M_PI);
    float t = timeScale * ((float)timeSinceStart.count()/1000.f);

    float timeSpan = (1.f / 0.01667f) * 10;

    auto amplitude = (float)height/2;
    auto yOffset = amplitude;
    auto value = amplitude * sin(t) + yOffset;

    _buffer.push_front(value);
    if (_buffer.size() > 600) {
        _buffer.pop_back();
    }

    float lastX = 0;
    float lastY = 0;
    for(int i=0; i<_buffer.size(); i++) {
        if (i >= _buffer.size()) {
            continue;
        }

       auto x = (float)i * (width/timeSpan);
       auto y = (float)_buffer[i];

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

void SeismoScene::resize(int /*width*/, int /*height*/)
{
    //render();
}

void SeismoScene::cleanup()
{
    // TODO?
}
    
