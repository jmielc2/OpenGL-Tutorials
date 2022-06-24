#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Shader.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"

// Callback Functions
void processInput(GLFWwindow* window);

// main function/entry point
int main() {
	// Init and set window settings
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	GLfloat triangle[] = {
		// Triangle Vertices	// Colors
		-0.5f, -0.5f, 0.0f,		0.0f, 1.0f, 0.0f,  // Bottom Left
		-0.5f, 0.5f, 0.0f,		0.0f, 0.0f, 1.0f,  // Top Left
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f, 0.0f,  // Bottom Right
		0.5f, 0.5f, 0.0f,		1.0f, 1.0f, 1.0f   // Top Right
	};

	GLuint indices[] = {
		// Square Indice Order
		0, 1, 2,  // Triangle 1
		1, 2, 3   // Triangle 2
	};

	// Create the window and set it to the current context
	GLFWwindow* window = glfwCreateWindow(700, 700, "Hello Triangle", NULL, NULL);
	if (window == NULL) {
		std::cerr << "ERROR: FAILED TO CREATE GLFW WINDOW." << std::endl;
		glfwTerminate();
		return EXIT_FAILURE;
	}

	// Load the GL Functions and set the viewport of the window
	glfwMakeContextCurrent(window);
	gladLoadGL();

	// Create Different Shaders
	Shader shader("vertexShader.vs", "firstFragShader.fs");

	// Create VBOs & 
	VAO VAO;
	VBO VBO;
	EBO EBO;

	// Read in the triangle
	VAO.bind();

	VBO.init(triangle, sizeof(triangle));
	EBO.init(indices, sizeof(indices));
	VAO.linkVBO(VBO, 0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	VAO.linkVBO(VBO, 1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	
	VAO.unbind();
	VBO.unbind();
	EBO.unbind();

	// Set the clear color buffer for wiping/resetting the window
	glClearColor(0.07f, 0.13f, 0.17f, 1.0f);

	// Enter the Program While Loop
	while (!glfwWindowShouldClose(window)) {
		// Process Input
		glfwPollEvents();
		processInput(window);

		// Wipe the Scene Clean
		glClear(GL_COLOR_BUFFER_BIT);

		// Use the Uniform to Change the Triangle Scale
		float scaleValue = (float) sin(glfwGetTime()) * 2;
		GLint scaleLocation = glGetUniformLocation(shader.id, "scale");
		shader.activate();
		glUniform1f(scaleLocation, scaleValue);

		// Draw Triangle (w/out EBO)
		/*VAO.bind();
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glDrawArrays(GL_TRIANGLES, 1, 3);
		VAO.unbind();*/
		
		
		// Draw Triangle (w/ EBO)
		VAO.bind();
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		VAO.unbind();

		// Actually Update the Window with new buffer
		glfwSwapBuffers(window);
	}

	// Clean Up and Close
	VAO.destroy();
	VBO.destroy();
	EBO.destroy();
	shader.destroy();
	glfwDestroyWindow(window);
	glfwTerminate();
	return EXIT_SUCCESS;
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
}