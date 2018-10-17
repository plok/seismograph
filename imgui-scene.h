#ifndef KATLA_IMGUI_SCENE_H
#define KATLA_IMGUI_SCENE_H

#include "modules/katla/gpu/render/render-view.h"
#include "modules/katla/gpu/render/imgui-glfw-binder.h"

#include "gpu/render/render-stats.h"

#include <deque>

class ImGuiScene : public Scene
{
public:
    ImGuiScene(RenderStatsPtr renderStats);

    void init() override;
    void render() override;
    void cleanup() override;

    void resize(int width, int height) override;

private:
    RenderStatsPtr _renderStats;

    std::deque<float> _renderLoadGraph;
    std::deque<float> _fpsGraph;
};

#endif