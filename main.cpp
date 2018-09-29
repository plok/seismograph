#include "gpu/opengl/opengl.h"

#include "gui-kit/window/window-factory.h"

#include "drawing/skia-opengl.h"

#include "app-kit/timer-uv.h"
#include "app-kit/event-loop-uv.h"
#include "app-kit/signal-handler-uv.h"

#include "app-kit/core-application.h"

#include "string.h"
#include <iostream>
#include <vector>

// TODO remove
#include <GLFW/glfw3.h>

std::tuple<std::unique_ptr<GraphicsBackend>, ErrorPtr> initializeGraphicsBackend() {

    std::cout << "Creating opengl backend!" << std::endl;
    
    auto opengl = std::make_unique<OpenGl>();
    auto error = opengl->init();
    if (error) {
        return {std::unique_ptr<GraphicsBackend>(), Error::create("Failed initializing opengl backend: " + error->message)};
    }

    return {std::unique_ptr<GraphicsBackend>(std::move(opengl)), Error::none()};
}

int main(int argc, char* argv[])
{    
    CoreApplication app;

    auto error = app.init();
    if (error) {
        std::cout << error->name << " " << error->message << std::endl << std::flush;
        return -1;
    }

    auto [graphicsBackend, backendError] = initializeGraphicsBackend();
    if (backendError) {
        std::cout << "Failed initializing graphics backend" << backendError->message;
        return -1;
    }

    // before we can create a surface we need to create a window
    auto windowFactory = graphicsBackend->windowFactory();
    auto windowEvents = std::make_shared<WindowEvents>();

    auto [window, createError] = windowFactory->create(800, 600, "Hello!", windowEvents);
    if (createError) {
        std::cout << "Failed creating window" << createError->message;
        return -1;
    }

    auto timer = app.createTimer();

    windowEvents->closeRequested = std::make_shared<FuncObserver<bool>>([&app, &timer, &window](bool) {
        std::cout << "close event!" << std::endl << std::flush;

        timer->stop();
        app.close();
    });

    auto initDeviceError = graphicsBackend->initDevice();

    window->init();
    window->show();

    
    SkiaOpenGL skia;
    skia.init();
    skia.draw();
    window->render();

    timer->start(17, [&skia, &window]() {
        skia.draw();
        window->render();
    });

    error = app.run();
    if (error) {
        std::cout << error->name << " " << error->message << std::endl;
        return -1;
    }

    graphicsBackend->cleanup();
    window->close();

    return 0;
}
