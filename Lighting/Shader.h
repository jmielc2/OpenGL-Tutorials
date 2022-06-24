#pragma once

#ifndef SHADER_CLASS_H
#define SHADER_CLASS_H

#include "stdafx.hpp"

std::string getFileContents(std::string source);

class Shader {
public:
	GLuint id;
	Shader(std::string vertexFile, std::string fragFile);

	void setMat4(std::string name, const glm::mat4& value);
	void setVec3(std::string name, const glm::vec3& value);
	void activate();
	void destroy();
};

#endif