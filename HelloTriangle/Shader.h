#pragma once

#ifndef SHADER_CLASS_H
#define SHADER_CLASS_H

#include <glad/glad.h>
#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include <cerrno>

std::string getFileContents(std::string source);

class Shader {
public:
	GLuint id;
	Shader(std::string vertexFile, std::string fragFile);

	void activate();
	void destroy();
};

#endif