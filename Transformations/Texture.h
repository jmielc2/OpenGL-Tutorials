#pragma once
#ifndef TEXTURE_H
#define TEXTURE_H

#include <iostream>
#include <glad/glad.h>
#include <string>

class Texture {
private:
	bool initialized;
	int width, height;
	GLenum target;

	unsigned char* readPictureData(std::string sourceFile);
public:
	GLuint id;
	Texture();
	Texture(std::string sourceFile, GLenum type);
	
	void init(std::string sourceFile, GLenum type);
	void bind();
	void unbind();
	void destroy();
	bool isInitialized();
};

#endif
