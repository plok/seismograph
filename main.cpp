#include "gpu/opengl/opengl.h"

#include "window/window-factory.h"

#include "drawing/skia-opengl.h"

#include "app-kit/timer-uv.h"
#include "app-kit/event-loop-uv.h"
#include "app-kit/signal-handler-uv.h"

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
    UvEventLoop eventLoop;
    
    auto error = eventLoop.init();
    if (error) {
        std::cout << error->name << " " << error->message << std::endl;
        return -1;
    }

    UvSignalHandler signalHandler(&eventLoop);
    error = signalHandler.init();
    if (error) {
        std::cout << error->name << " " << error->message << std::endl << std::flush;
        return -1;
    }

    error = signalHandler.start([&eventLoop](Signal signal) {
        std::cout << "Signal received: " << (int)signal << std::endl << std::flush;
        eventLoop.close();
    });

    auto [graphicsBackend, backendError] = initializeGraphicsBackend();
    if (backendError) {
        std::cout << "Failed initializing graphics backend" << backendError->message;
        return -1;
    }

    // before we can create a sufrace we need to create a window
    auto windowFactory = graphicsBackend->windowFactory();
    auto [window, createError] = windowFactory->create(800, 600, "Hello!");
    if (createError) {
        std::cout << "Failed creating window" << createError->message;
        return -1;
    }

    auto initDeviceError = graphicsBackend->initDevice();

    UvTimer timer(&eventLoop);
    timer.init();

    auto closeSubject = std::make_shared<Subject<bool>>();
    auto closeEvent = std::make_shared<FuncObserver<bool>>([&eventLoop, &signalHandler, &timer, &window](bool) {
        std::cout << "close event!" << std::endl << std::flush;

        signalHandler.stop();
        timer.stop();
        eventLoop.close();

        // TODO on event loop?
        window->close();
    });
    closeSubject->subscribe(closeEvent);
    window->attachCloseSubject(closeSubject);

    window->init();
    window->show();


    
    SkiaOpenGL skia;
    skia.init();
    skia.draw();
    window->render();

    timer.start(17, [&skia, &window]() {
        skia.draw();
        window->render();
    });

    
    // window->waitForClose();

    error = eventLoop.run();
    if (error) {
        std::cout << error->name << " " << error->message << std::endl;
        return -1;
    }

    error = signalHandler.stop();

    graphicsBackend->cleanup();

    timer.stop();

    error = eventLoop.close();
    if (error) {
        std::cout << error->name << " " << error->message << std::endl;
        return -1;
    }

    return 0;
}
