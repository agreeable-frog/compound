#include "compound/window.hh"
#include "compound/program.hh"
#include "compound/pipeline_state.hh"
#include "compound/vertex_buffer.hh"

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
    compound::PipelineState testPipelineState{};
    testPipelineState.bind();
    compound::VertexBuffer testVertexBuffer(
        compound::VertexBuffer::Usage::STATIC);
    testVertexBuffer.bind();
    testVertexBuffer.attrib(testPipelineState,
                            compound::MeshVertex().getDescriptor());
    std::vector<compound::MeshVertex> internal;
    internal.push_back(compound::MeshVertex{glm::vec3{-0.5f, -0.5f, 0.0f},
                                            glm::vec4{1.0f, 0.0f, 0.0f, 1.0f}});
    internal.push_back(compound::MeshVertex{glm::vec3{0.5f, -0.5f, 0.0f},
                                            glm::vec4{1.0f, 0.0f, 0.0f, 1.0f}});
    internal.push_back(compound::MeshVertex{glm::vec3{0.0f, 0.5f, 0.0f},
                                            glm::vec4{1.0f, 0.0f, 0.0f, 1.0f}});
    testVertexBuffer.bufferData(
        internal.data(), internal.size() * compound::MeshVertex().size());
    while (!testWindow.shouldClose()) {
        testWindow.makeContextCurrent();
        testWindow.pollEvents();
        testWindow.TMPsetViewPortToWindow();
        testWindow.TMPclear();
        testProgram.bind();
        testPipelineState.TMPdraw(0, 3);
        testWindow.swapBuffers();
    }
    return 0;
}