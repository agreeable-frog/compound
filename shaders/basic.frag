#version 330 core

#extension GL_ARB_shading_language_420pack : require
#extension GL_ARB_explicit_attrib_location : require
#extension GL_ARB_separate_shader_objects : require
#extension GL_ARB_explicit_uniform_location : require

uniform sampler2DArray atlas;

layout(location = 0) in vec2 uv;
layout(location = 1) in vec3 atlasCoords;

layout(location = 0) out vec4 outFragColor;

void main() {
    outFragColor = texture(atlas, atlasCoords * vec3(uv, 1.0));
}