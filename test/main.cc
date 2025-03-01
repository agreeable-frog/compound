#include "compound/window.hh"
#include "compound/program.hh"
#include "compound/pipeline.hh"
#include "compound/vertex_buffer.hh"
#include "compound/index_buffer.hh"
#include "compound/mesh.hh"
#include "compound/camera.hh"
#include "compound/object.hh"
#include "compound/framebuffer.hh"
#include "compound/texture_atlas.hh"

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
    auto pTestVertexBuffer = std::make_shared<compound::VertexBuffer>(
        compound::VertexBuffer::Usage::STATIC);
    auto pTestIndexBuffer = std::make_shared<compound::IndexBuffer>(
        compound::IndexBuffer::Usage::STATIC);
    auto pInstanceBuffer = std::make_shared<compound::VertexBuffer>(
        compound::VertexBuffer::Usage::STREAM);
    pTestVertexBuffer->bind();
    pTestVertexBuffer->attrib(testPipeline,
                              compound::MeshVertex().getDescriptor());
    pTestIndexBuffer->bind();
    auto cubeMesh = std::make_shared<compound::Mesh>(compound::Mesh::Cube());
    auto sphereMesh =
        std::make_shared<compound::Mesh>(compound::Mesh::Sphere(16, 16));
    auto planeMesh =
        std::make_shared<compound::Mesh>(compound::Mesh::Plane(16));
    compound::Mesh::loadMeshes({cubeMesh, sphereMesh, planeMesh},
                               pTestVertexBuffer, pTestIndexBuffer);
    compound::Camera camera(glm::vec3{0.0f, 0.0f, -4.0f},
                            glm::vec3{1.0f, 0.0f, 0.0f},
                            glm::vec3{0.0f, 0.0f, 1.0f}, 0.1f, 50.0f, M_PI_2);
    pInstanceBuffer->bind();
    pInstanceBuffer->attrib(testPipeline,
                            compound::ObjectVertex().getDescriptor());

    std::shared_ptr<compound::TextureAtlas> atlas =
        std::make_shared<compound::TextureAtlas>(1);
    atlas->bind();
    std::shared_ptr<compound::Texture> texture2 =
        std::make_shared<compound::Texture>(std::string(RESOURCES_PATH) +
                                            "grass_top.png");
    std::shared_ptr<compound::Texture> texture3 =
        std::make_shared<compound::Texture>(std::string(RESOURCES_PATH) +
                                            "log_oak.png");
    atlas->addTexture(texture2);
    atlas->addTexture(texture3);
    atlas->generate();
    std::vector<compound::Object> objects;
    for (size_t i = 0; i < 10; i++) {
        auto cube = compound::Object(
            {0.0f + 4.0f * i, 0.0f, 0.0f},
            {M_PI_2 / 4 * i, M_PI_2 / 7 * i, M_PI_2 * i}, {1.0f, 1.0f, 1.0f},
            i % 3 == 0 ? sphereMesh : cubeMesh,
            i % 2 == 0 ? texture3 : texture2);
        objects.push_back(cube);
    }
    objects.push_back(
        compound::Object({-1.0f, 0.0f, 0.0f}, {0.0f, M_PI_2, 0.0f},
                         {10.0f, 10.0f, 10.0f}, planeMesh, texture3));

    compound::Framebuffer fb(900, 600);
    compound::Renderbuffer color(900, 600,
                                 compound::Renderbuffer::Format::COLOR);
    color.bind();
    fb.bind();
    fb.attach(compound::Framebuffer::Attachment::COLOR_ATTACHMENT0, color);
    fb.unbind();

    testProgram.bind();
    int frameId = 0;
    while (!testWindow.shouldClose()) {
        // Framerate limiter
        static double lastFrameTime = testWindow.TMPgetTime();
        double currentTime = testWindow.TMPgetTime();
        double deltaTime = currentTime - lastFrameTime;
        if (deltaTime < (1.0 / 60.0)) {
            continue;
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
                camera.rotate((float)cursorMove[0] * -0.005f,
                              (float)cursorMove[1] * -0.005f, 0.0f);
            }
        }
        testProgram.setUniform(0, camera.projection(1.5f));
        testProgram.setUniform(1, camera.view());
        testWindow.TMPsetViewPortToWindow();
        testWindow.TMPclear();
        std::map<std::shared_ptr<compound::Mesh>,
                 std::vector<const compound::Object*>>
            instanceGroups;
        for (auto& object : objects) {
            if (object.mesh().expired()) continue;
            instanceGroups[object.mesh().lock()].push_back(&object);
        }
        for (auto keyvalue : instanceGroups) {
            auto mesh = keyvalue.first;
            auto objects = keyvalue.second;
            auto instanceVertices = std::vector<compound::ObjectVertex>();
            for (const auto& object : objects) {
                instanceVertices.push_back(compound::ObjectVertex(
                    object->modelMatrix(),
                    object->texture().lock()->atlasCoords()));
            }
            pInstanceBuffer->bufferData(
                instanceVertices.data(),
                instanceVertices.size() * compound::ObjectVertex().size(),
                compound::ObjectVertex().getDescriptor());
            testPipeline.drawElementsInstanced(
                mesh->nbIndices(), mesh->indexBufferOffset(), objects.size());
        }
        testWindow.swapBuffers();

        ++frameId;
    }
    return 0;
}