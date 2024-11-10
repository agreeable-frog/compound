#include "compound/window.hh"
#include "compound/program.hh"
#include "compound/pipeline.hh"
#include "compound/vertex_buffer.hh"
#include "compound/index_buffer.hh"
#include "compound/mesh.hh"
#include "compound/camera.hh"

#include <iostream>
#include "log4cplus/configurator.h"
#include "log4cplus/log4cplus.h"
#include <unistd.h>
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_glfw.h"

int main() {
    log4cplus::initialize();
    log4cplus::PropertyConfigurator::doConfigure(std::string(LOG4CPLUS_CONFIG));
    log4cplus::Logger testLogger = log4cplus::Logger::getInstance("TEST");
    compound::Window testWindow("test", 900, 600);
    testWindow.makeContextCurrent();
    compound::Program testProgram(std::string(SHADERS_PATH) + "basic.vert",
                                  std::string(SHADERS_PATH) + "basic.frag");
    compound::Pipeline testPipeline{};
    testPipeline.bind();
    std::shared_ptr<compound::VertexBuffer> pTestVertexBuffer =
        std::make_shared<compound::VertexBuffer>(
            compound::VertexBuffer::Usage::STATIC);
    std::shared_ptr<compound::IndexBuffer> pTestIndexBuffer =
        std::make_shared<compound::IndexBuffer>(
            compound::IndexBuffer::Usage::STATIC);
    pTestVertexBuffer->bind();
    pTestVertexBuffer->attrib(testPipeline,
                              compound::MeshVertex().getDescriptor());
    pTestIndexBuffer->bind();
    std::shared_ptr<compound::Mesh> cube =
        std::make_shared<compound::Mesh>(compound::Mesh::Cube());
    compound::Mesh::loadMeshes({cube}, pTestVertexBuffer, pTestIndexBuffer);
    compound::Camera camera(glm::vec3{0.0f, 0.0f, -4.0f},
                            glm::vec3{1.0f, 0.0f, 0.0f},
                            glm::vec3{0.0f, 0.0f, 1.0f}, 0.1f, 10.0f, M_PI_2);
    testProgram.bind();
    int frameId = 0;
    while (!testWindow.shouldClose()) {
        // Framerate limiter
        static double lastFrameTime = testWindow.TMPgetTime();
        static double lastCoutTime = lastFrameTime;
        static int lastCoutFrameId = 0;
        static float actualFps = 0.0f;
        double currentTime = testWindow.TMPgetTime();
        double deltaTime = currentTime - lastFrameTime;
        if (deltaTime < (1.0 / 60.0)) {
            continue;
        }
        if (currentTime - lastCoutTime > 0.5f) {
            actualFps = float(frameId - lastCoutFrameId) * 2.0f;
            lastCoutFrameId = frameId;
            lastCoutTime = currentTime;
        }
        lastFrameTime = currentTime;
        testWindow.makeContextCurrent();
        testWindow.pollEvents();
        {
            auto keystates = testWindow.keyStates();
            if (keystates[87]) { // forward
                camera.move(camera.forward() * 1.0f * (float)deltaTime);
            }
            if (keystates[83]) { // backwards
                camera.move(-camera.forward() * 1.0f * (float)deltaTime);
            }
            if (keystates[65]) { // left
                camera.move(camera.left() * 1.0f * (float)deltaTime);
            }
            if (keystates[68]) { // right
                camera.move(-camera.left() * 1.0f * (float)deltaTime);
            }
            if (keystates[32]) { // up
                camera.move(camera.up() * 1.0f * (float)deltaTime);
            }
            if (keystates[340]) { // down
                camera.move(-camera.up() * 1.0f * (float)deltaTime);
            }
            if (keystates[81]) {
                camera.rotate(0.0f, 0.0f, -(float)deltaTime);
            }
            if (keystates[69]) {
                camera.rotate(0.0f, 0.0f, (float)deltaTime);
            }
            auto mouseButtonStates = testWindow.mouseButtonStates();
            if (mouseButtonStates[0]) {
                auto cursorMove = testWindow.cursorMove();
                camera.rotate((float)cursorMove[0] * -0.005f, (float)cursorMove[1] * -0.005f, 0.0f);
            }
        }
        testProgram.setUniform(0, camera.projection(1.5f));
        testProgram.setUniform(1, camera.view());
        testWindow.TMPsetViewPortToWindow();
        testWindow.TMPclear();
        testPipeline.drawElements(cube->nbIndices(), cube->indexBufferOffset());
        testWindow.swapBuffers();

        ++frameId;
    }
    return 0;
}