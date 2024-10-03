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
    testProgram.setUniform(0, camera.projection(1.5f));
    testProgram.setUniform(1, camera.view());
    while (!testWindow.shouldClose()) {
        testWindow.makeContextCurrent();
        testWindow.pollEvents();
        testWindow.TMPsetViewPortToWindow();
        testWindow.TMPclear();
        testPipeline.drawElements(cube->nbIndices(), cube->indexBufferOffset());
        testWindow.swapBuffers();
    }
    return 0;
}