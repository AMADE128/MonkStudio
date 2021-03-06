#include "Textures.h"
#include "il.h"
#include "ilut.h"

#include "Application.h"
#include "Globals.h"
#include "External Libraries/Glew/include/glew.h"
#include "External Libraries/SDL/include/SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>

Texture::Texture(unsigned int _uid) : Resource(_uid, Resource::Type::TEXTURE)
{
	mTextureID = 0;

	mTextureHeight = 0;
	mTextureWidth = 0;
}

Texture::~Texture()
{
	Unload();
}

bool Texture::Load(const std::string _path)
{
	bool ret = false;

    ILuint imgID = 0;
    ilGenImages(1, &imgID);
    ilBindImage(imgID);

    //Load image
    ILboolean success = ilLoadImage(_path.c_str());

    //Image loaded successfully
    if (success == IL_TRUE)
    {
        //Convert image to RGBA
        success = ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
        if (success == IL_TRUE)
        {
            //Create texture from file pixels
            ret = Load32((GLuint*)ilGetData(), (GLuint)ilGetInteger(IL_IMAGE_WIDTH), (GLuint)ilGetInteger(IL_IMAGE_HEIGHT));
        }

        //Delete file from memory
        ilDeleteImages(1, &imgID);
		glBindTexture(GL_TEXTURE_2D, 0);
    }

    //Report error
    if (!ret)
    {
        LOG("Unable to load %s\n", _path.c_str());
    }

	return ret;
}

bool Texture::Load32(GLuint* pixels, GLuint width, GLuint height)
{
    //Free texture if it exists
    Unload();

    //Get texture dimensions
    mTextureWidth = width;
    mTextureHeight = height;

    //Generate texture ID
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	mTextureID = ilutGLBindTexImage();

    //Bind texture ID
    glBindTexture(GL_TEXTURE_2D, mTextureID);

    //Generate texture
    //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

    //Set texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);

    //Unbind texture
    glBindTexture(GL_TEXTURE_2D, NULL);

    //Check for error
    GLenum error = glGetError();
    if (error != GL_NO_ERROR)
    {
        LOG("Error loading texture from pixels! %s\n", pixels, glewGetErrorString(error));
        return false;
    }

	return true;
}

void Texture::Bind()
{
    glBindTexture(GL_TEXTURE_2D, mTextureID);
}

void Texture::Unload()
{
	if (mTextureID != 0)
	{
		glDeleteTextures(1, &mTextureID);
		mTextureID = 0;
	}

	mTextureWidth = 0;
	mTextureHeight = 0;
}

GLuint Texture::GetTextureID()
{
	return mTextureID;
}

GLuint Texture::GetTextureWidth()
{
	return mTextureWidth;
}

GLuint Texture::GetTextureHeight()
{
	return mTextureHeight;
}
