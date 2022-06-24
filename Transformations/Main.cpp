#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>

#include "Shader.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "Texture.h"

// Callback Functions
void processInput(GLFWwindow* window);
void framebufferResize(GLFWwindow* window, int width, int height);

// main function/entry point
int main() {
	// Init and set window settings
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLfloat triangle[] = {
		// Triangle Vertices		// Texture Coordinates
		-0.5f, -0.5f, 0.0f,			0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f,			0.0f, 1.0f,
		0.5f, -0.5f, 0.0f,			1.0f, 0.0f,
		0.5f, 0.5f, 0.0f,			1.0f, 1.0f
	};

	GLuint indices[] = {
		// Square Indice Order
		0, 1, 2,  // Triangle 1
		1, 2, 3   // Triangle 2
	};

	// Create the window and set it to the current context
	GLFWwindow* window = glfwCreateWindow(800, 550, "Transformations", NULL, NULL);
	if (window == NULL) {
		std::cerr << "ERROR: FAILED TO CREATE GLFW WINDOW." << std::endl;
		glfwTerminate();
		return EXIT_FAILURE;
	}

	// Load the GL Functions and set the viewport of the window
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebufferResize);
	gladLoadGL();

	// Configure the Texture Drawing Formats
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Create Different Shaders
	Shader shader("vertexShader.vs", "firstFragShader.fs");
	shader.activate();

	// Create Texture
	Texture texture("pics/kaede.jpg", GL_TEXTURE_2D);

	// Create VBOs & 
	VAO VAO;
	VBO VBO;
	EBO EBO;

	// Read in the triangle
	VAO.bind();
	VBO.init(triangle, sizeof(triangle));
	VBO.bind();
	EBO.init(indices, sizeof(indices));
	EBO.bind();
	VAO.linkVBO(VBO, 0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	VAO.linkVBO(VBO, 1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	VAO.unbind();
	VBO.unbind();
	EBO.unbind();

	// Set the clear color buffer for wiping/resetting the window
	glClearColor(0.07f, 0.13f, 0.17f, 1.0f);

	VAO.bind();
	while (!glfwWindowShouldClose(window)) {
		// Process Input
		glfwPollEvents();
		processInput(window);

		// Setup Transformation
		glm::mat4 trans = glm::mat4(1.0f);
		trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
		trans = glm::rotate(trans, (float) glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
		trans = glm::scale(trans, glm::vec3(0.75f, 0.75f, 0.75f));

		// Input Transformation as Uniform (Shader needs to be activated before uniform can be loaded)
		GLint transformation = glGetUniformLocation(shader.id, "transform");
		glUniformMatrix4fv(transformation, 1, GL_FALSE, glm::value_ptr(trans));

		// Wipe the Scene Clean
		glClear(GL_COLOR_BUFFER_BIT);
		texture.bind();
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		texture.unbind();

		// Actually Update the Window with new buffer
		glfwSwapBuffers(window);
	}
	VAO.unbind();

	// Clean Up and Close
	VAO.destroy();
	VBO.destroy();
	EBO.destroy();
	shader.destroy();
	texture.destroy();
	glfwDestroyWindow(window);
	glfwTerminate();
	return EXIT_SUCCESS;
}



void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
}

void framebufferResize(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}