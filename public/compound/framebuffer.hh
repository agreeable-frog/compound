#pragma once

#include <memory>

namespace compound {
namespace impl {
class Renderbuffer;
class Framebuffer;
} // namespace impl

class Framebuffer;

class Renderbuffer {
    friend Framebuffer;

public:
    enum class Format { COLOR, DEPTH, DEPTH_STENCIL };
    Renderbuffer(unsigned int width, unsigned int height, Format format);
    void bind();
    ~Renderbuffer();

private:
    std::unique_ptr<impl::Renderbuffer> _pImpl;
};

class Framebuffer {
public:
    Framebuffer(unsigned int width, unsigned int height);
    enum class Attachment { COLOR_ATTACHMENT0, DEPTH, DEPTH_STENCIL };
    void bind();
    static void unbind();
    void bindRead();
    static void unbindRead();
    void bindDraw();
    static void unbindDraw();
    void attach(Attachment attachment, const Renderbuffer& renderbuffer);
    std::vector<unsigned char> readPixels(int x, int y, size_t width, size_t height,
                                 Attachment attachment);
    ~Framebuffer();

private:
    std::unique_ptr<impl::Framebuffer> _pImpl;
};
} // namespace compound