#include "gpu/backend/opengl/opengl.h"

#include "gpu/window/window.h"
#include "gpu/window/window-factory.h"

#include "gui-kit/gui-kit.h"

#include "app-kit/timer-uv.h"
#include "app-kit/event-loop-uv.h"
#include "app-kit/signal-handler-uv.h"

#include "app-kit/core-application.h"

#include "rxcpp/rx.hpp"

#include "seismo-render-view.h"
#include "audio/device-manager.h"
#include "string.h"
#include <iostream>
#include <vector>

int main(int argc, char* argv[])
{    
    CoreApplication app;

    auto error = app.init();
    if (error) {
        std::cout << error->name << " " << error->message << std::endl << std::flush;
        return -1;
    }
    auto values1 = rxcpp::observable<>::range(1, 5);
    // JORRIT HACKS
    values1.
        subscribe(
            [](int v){printf("OnNext: %d\n", v);},
            [](){printf("OnCompleted\n");});

    auto dm = new DeviceManager();
    //
    GuiKit guiKit(app);
    guiKit.init();

    // TODO no shared_ptr
    auto windowProperties = std::make_shared<WindowProperties>();
    windowProperties->size.width = 800;
    windowProperties->size.height = 600;
    windowProperties->title = "Hello!";

    auto renderView = std::make_shared<SeismoRenderView>();

    auto window = guiKit.createWindow(windowProperties, renderView);
    
    auto timer = app.createTimer();
    
    timer->start(17, [&window]() {
        window->render();
        window->processEvents();
    });

    error = app.run();
    if (error) {
        std::cout << error->name << " " << error->message << std::endl;
        return -1;
    }

    guiKit.close();
    window->close();

    return 0;
}
