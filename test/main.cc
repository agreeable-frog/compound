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
    compound::MeshVertexBuffer testVertexBuffer(
        compound::VertexBuffer::Usage::STATIC);
    testVertexBuffer.bind();
    testVertexBuffer.attrib();
    testVertexBuffer.TMPPushTriangle();
    testVertexBuffer.bufferData();
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