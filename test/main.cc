#include "compound/window.hh"

#include <iostream>
#include <log4cplus/configurator.h>
#include <log4cplus/log4cplus.h>
#include <unistd.h>
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_glfw.h"

int main() {
    log4cplus::initialize();
    log4cplus::PropertyConfigurator::doConfigure(std::string(LOG4CPLUS_CONFIG));
    log4cplus::Logger testLogger = log4cplus::Logger::getInstance("TEST");
    compound::Window testWindow("test", 900, 600);
    compound::Window testWindow2("test2", 900, 600);
    while (!testWindow.shouldClose() && !testWindow2.shouldClose()) {
        testWindow.makeContextCurrent();
        testWindow.pollEvents();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
        ImGui::SetNextWindowSize(ImVec2(100.0f, 100.0f));
        ImGui::SetNextWindowBgAlpha(0.3f);
        ImGui::Begin("Debug", 0, ImGuiWindowFlags_NoDecoration);
        ImGui::Text("test1");
        ImGui::End();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        testWindow.swapBuffers();


        testWindow2.makeContextCurrent();
        testWindow2.pollEvents();
        // !!!! VIEWPORT !!!!
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
        ImGui::SetNextWindowSize(ImVec2(100.0f, 100.0f));
        ImGui::SetNextWindowBgAlpha(0.3f);
        ImGui::Begin("Debug2", 0, ImGuiWindowFlags_NoDecoration);
        ImGui::Text("test2");
        ImGui::End();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        testWindow2.swapBuffers();
    }
    return 0;
}