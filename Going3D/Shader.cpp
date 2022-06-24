#include "Shader.h"

std::string getFileContents(std::string source) {
	std::ifstream in(source, std::ios::binary);
	if (in) {
		in.seekg(0, std::ios::end);
		std::string contents(in.tellg(), '\0');
		in.seekg(0, std::ios::beg);
		if (in.read(&contents[0], contents.size())) {
			in.close();
			return contents;
		}
		throw(errno);
		exit(1);
	}
	throw(errno);
	exit(1);
}

GLuint createShader(std::string sourceFile, GLenum type) {
	std::string shaderSource = getFileContents(sourceFile);
	GLuint shader = glCreateShader(type);
	const char* source = shaderSource.c_str();
	glShaderSource(shader, 1, &source, NULL);
	glCompileShader(shader);

	// Check if compiled correctly
	int success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		char logInfo[512];
		glGetShaderInfoLog(shader, 512, NULL, logInfo);
		LOG("SHADER COMPILER ERROR: ");
		LOG("SHADER FILE: '" << sourceFile << "'");
		LOG(logInfo);
		throw(errno);
		exit(1);
	}
	return shader;
}

Shader::Shader(std::string vertexSourceFile, std::string fragSourceFile) {
	GLuint vShader = createShader(vertexSourceFile, GL_VERTEX_SHADER);
	GLuint fShader = createShader(fragSourceFile, GL_FRAGMENT_SHADER);

	this->id = glCreateProgram();
	glAttachShader(this->id, vShader);
	glAttachShader(this->id, fShader);
	glLinkProgram(this->id);

	int success;
	glGetProgramiv(this->id, GL_LINK_STATUS, &success);
	if (!success) {
		char infoLog[512];
		glGetProgramInfoLog(this->id, 512, NULL, infoLog);
		LOG("PROGRAM LINKING ERROR: \n");
		LOG(infoLog);
		throw(errno);
		exit(1);
	}

	glDeleteShader(vShader);
	glDeleteShader(fShader);
	glUseProgram(0);
}

void Shader::setMat4(std::string name, const glm::mat4 &value) {
	this->activate();
	GLint reference = glGetUniformLocation(this->id, name.c_str());
	glUniformMatrix4fv(reference, 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::activate() {
	glUseProgram(this->id);
}

void Shader::destroy() {
	glDeleteProgram(this->id);
}