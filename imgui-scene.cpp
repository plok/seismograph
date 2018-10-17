#include "imgui-scene.h"

#include <GLFW/glfw3.h>
#include <render/imgui/imgui_impl_glfw.h>
#include <render/imgui/imgui_impl_opengl3.h>

#include "external/imgui/imgui.h"

#include <iomanip>
#include <iostream>

ImGuiScene::ImGuiScene(RenderStatsPtr renderStats) :
    _renderStats(renderStats)
{
}

void ImGuiScene::init()
{
}

void ImGuiScene::render()
{
    auto io = ImGui::GetIO();
//    io.DisplaySize= ImVec2(400, 300);

    ImGui::SetNextWindowSize(ImVec2(400, 300));

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    auto stats = _renderStats->stats();

    std::stringstream lastDurationStringStream;
    lastDurationStringStream << std::fixed << std::setprecision(2) << stats.LastDuration / 1000.f;

    float interval = stats.LastRenderInterval / 1000000.f;
    float fps = 1.f / interval;
    float load = stats.LastDuration * 100.f / static_cast<float>(stats.LastRenderInterval);

    _renderLoadGraph.push_back(load);
    while (_renderLoadGraph.size() > 100) {
        _renderLoadGraph.pop_front();
    }

    _fpsGraph.push_back(fps);
    while (_fpsGraph.size() > 100) {
        _fpsGraph.pop_front();
    }

    std::stringstream fpsStrStream;
    fpsStrStream << std::fixed << std::setprecision(2) << fps;

    ImGui::Text("Render time: %s ms", lastDurationStringStream.str().c_str());
    ImGui::Text("FPS: %s", fpsStrStream.str().c_str());


    ImGui::PlotLines("Load", [](void* aself, int idx) {
        auto self = reinterpret_cast<ImGuiScene*>(aself);

        return self->_renderLoadGraph[idx];
    }, this, static_cast<int>(_renderLoadGraph.size()), /*values_offset*/ 0, /*overlay_text*/ NULL, /*scale_min*/ 0, /*scale_max*/ 100, ImVec2(0, 80));

    ImGui::PlotLines("FPS", [](void* aself, int idx) {
        auto self = reinterpret_cast<ImGuiScene*>(aself);

        return self->_fpsGraph[idx];
    }, this, static_cast<int>(_renderLoadGraph.size()), /*values_offset*/ 0, /*overlay_text*/ NULL, /*scale_min*/ 0, /*scale_max*/ 100, ImVec2(0, 80));


    ImGui::Render();

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());




}

void ImGuiScene::resize(int /*width*/, int /*height*/)
{
    //render();
}

void ImGuiScene::cleanup()
{
    // TODO?
}
    