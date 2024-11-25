#pragma once

#include <sys/types.h>
#include <stddef.h>
#include <vector>
#include <memory>
#include <string>
#include <log4cplus/log4cplus.h>
#include "glm/glm.hpp"

namespace compound {
namespace impl {
class Texture2DArray;
}
class TextureAtlas;
// TODO : separate file + generic for other uses than atlas
class Texture : public std::enable_shared_from_this<Texture> {
    friend TextureAtlas;

public:
    Texture(size_t width, size_t height, u_char* data);
    Texture(const std::string& path);
    ~Texture();
    size_t width() const {
        return _width;
    }
    size_t height() const {
        return _height;
    }
    bool isAtlased() const {
        return _isAtlased;
    }
    glm::vec3 atlasCoords() const {
        return _atlasCoords;
    }

private:
    log4cplus::Logger _logger =
        log4cplus::Logger::getInstance("compound.Texture.public");
    size_t _width = 0;
    size_t _height = 0;
    u_char* _data = 0;
    // atlas
    bool _isAtlased = false;
    std::weak_ptr<TextureAtlas> _atlas;
    glm::vec3 _atlasCoords;


    void clearHeap();
};

class TextureAtlas : public std::enable_shared_from_this<TextureAtlas> {
public:
    TextureAtlas(size_t mipmaplevels);
    ~TextureAtlas();
    void addTexture(std::shared_ptr<Texture> texture);
    void bind();
    void generate();
    size_t width() const {
        return _width;
    }
    size_t height() const {
        return _height;
    }

private:
    log4cplus::Logger _logger =
        log4cplus::Logger::getInstance("compound.TextureAtlas.public");
    std::unique_ptr<impl::Texture2DArray> _pTexture2DArray;
    bool _generated = false;
    size_t _width = 0;
    size_t _height = 0;
    size_t _depth = 0;
    size_t _mipmapLevels = 0;
    std::vector<std::shared_ptr<Texture>> _textures = {};
};
} // namespace compound