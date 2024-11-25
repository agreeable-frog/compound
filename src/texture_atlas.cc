#include <exception>

#include "compound/texture_atlas.hh"
#include "texture_2D_array.hh"
#include "stb_image.h"

namespace compound {
Texture::Texture(size_t width, size_t height, u_char* data)
    : _width(width), _height(height), _data(data) {
}

Texture::Texture(const std::string& path) {
    int width, height, nbChannels;
    u_char* data = stbi_load(path.c_str(), &width, &height, &nbChannels, 0);
    if (!data) {
        LOG4CPLUS_FATAL(_logger, "Could not load image file");
        throw std::runtime_error("Could not load image file");
    }
    _width = width;
    _height = height;
    _data = new u_char[width * height * 4];
    switch (nbChannels) {
        case 3:
            for (size_t i = 0; i < width * height; i++) {
                _data[4 * i + 0] = data[3 * i + 0];
                _data[4 * i + 1] = data[3 * i + 1];
                _data[4 * i + 2] = data[3 * i + 2];
                _data[4 * i + 3] = 255;
            }
            break;
        case 4:
            memcpy(_data, data, width * height * sizeof(u_char) * 4);
            break;
        default: {
            LOG4CPLUS_FATAL(_logger, "Unrecognized pixel format in image file");
            throw std::runtime_error("Unrecognized pixel format in image file");
        }
    }
    stbi_image_free(data);
}

Texture::~Texture() {
    if (_data) delete _data;
}

void Texture::clearHeap() {
    if (_data) delete _data;
}

TextureAtlas::TextureAtlas(size_t mipmaplevels)
    : _pTexture2DArray(new impl::Texture2DArray()),
      _mipmapLevels(mipmaplevels) {
}

TextureAtlas::~TextureAtlas() {
}

void TextureAtlas::addTexture(std::shared_ptr<Texture> texture) {
    if (texture->_isAtlased) {
        LOG4CPLUS_WARN(_logger, "Texture is already atlased");
        return;
    }
    _textures.push_back(texture);
    texture->_isAtlased = true;
    texture->_atlas = shared_from_this();
}

void TextureAtlas::bind() {
    _pTexture2DArray->bind();
}

void TextureAtlas::generate() {
    LOG4CPLUS_INFO(_logger, "Generating texture atlas");
    for (auto texture : _textures) {
        if (texture->_width > _width) _width = texture->_width;
        if (texture->_height > _height) _height = texture->_height;
    }
    _pTexture2DArray->texStorage3D(_mipmapLevels, _width, _height,
                                   _textures.size());
    size_t i = 0;
    for (auto texture : _textures) {
        // TODO : uvscale
        float uRatio = texture->_width / _width;
        float vRatio = texture->_height / _height;
        texture->_atlasCoords = glm::vec3(uRatio, vRatio, (float)i);
        _pTexture2DArray->texSubImage3D(0, 0, 0, i, texture->_width,
                                        texture->_height, 1, texture->_data);
        i++;
    }
    _pTexture2DArray->generateMipmap();
    _generated = true;
}
} // namespace compound