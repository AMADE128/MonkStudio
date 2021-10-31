#pragma once

#include <string>
#include "External Libraries/Glew/include/glew.h"

class Texture
{
public:
    Texture();

    ~Texture();

    bool Load(const std::string _path);

    bool Load32(GLuint* pixels, GLuint width, GLuint height);

    void Bind();

    void Unload();

    GLuint GetTextureID();

    GLuint GetTextureWidth();

    GLuint GetTextureHeight();

	const std::string GetTexPath();

private:

    GLuint mTextureID;

	std::string path;

    GLuint mTextureWidth;
    GLuint mTextureHeight;
};