#pragma once

#include "Resources.h"
#include <string>
#include "External Libraries/Glew/include/glew.h"

class Texture : public Resource
{
public:
    Texture(unsigned int _uid);

    ~Texture();

    bool Load(const std::string _path);

    bool Load32(GLuint* pixels, GLuint width, GLuint height);

    void Bind();

    void Unload();

    GLuint GetTextureID();

    GLuint GetTextureWidth();

    GLuint GetTextureHeight();

private:

    GLuint mTextureID;

    GLuint mTextureWidth;
    GLuint mTextureHeight;
};