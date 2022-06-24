#pragma once
#ifndef VBO_H
#define VBO_H 1

#include <glad/glad.h>

class VBO {
private:
	bool initialized;

public:
	GLuint id;

	VBO();
	VBO(GLfloat* vertices, GLsizeiptr size);

	void bind();
	void unbind();
	void destroy();
	void init(GLfloat* vertices, GLsizeiptr size);
	bool isInitialized();
};

#endif