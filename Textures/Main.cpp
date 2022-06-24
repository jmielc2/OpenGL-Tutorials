#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>

#include "Shader.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "Texture.h"

// Callback Functions
void processInput(GLFWwindow* window, bool &pause);
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
		-1.0f, -1.0f, 0.0f,			0.0f, 0.0f,
		-1.0f, 1.0f, 0.0f,			0.0f, 1.0f,
		1.0f, -1.0f, 0.0f,			1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,			1.0f, 1.0f
	};

	GLuint indices[] = {
		// Square Indice Order
		0, 1, 2,  // Triangle 1
		1, 2, 3   // Triangle 2
	};

	// Create the window and set it to the current context
	GLFWwindow* window = glfwCreateWindow(800, 550, "Bad Apple!!", NULL, NULL);
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
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	const int NUM_FRAMES = 6569;
	// const int BUFFER_SIZE = 50;
	const int LOAD_BUFFER = 100;
	
	// Load in the Textures
	Texture textures[NUM_FRAMES];
	for (int i = 0; i < LOAD_BUFFER; i++) {
		textures[i].init("pics/frame" + std::to_string(i) + ".png", GL_TEXTURE_2D);
	}

	// Create Different Shaders
	Shader shader("vertexShader.vs", "firstFragShader.fs");

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

	// Enter the Program While Loop
	int curFrame = 0, loadFrame = LOAD_BUFFER;
	bool pause = false;
	shader.activate();
	VAO.bind();

	float prev = glfwGetTime(), temp;
	while (!glfwWindowShouldClose(window)) {

		// Process Input
		glfwPollEvents();
		processInput(window, pause);

		// Load Next Frame
		textures[loadFrame].init("pics/frame" + std::to_string(loadFrame) + ".png", GL_TEXTURE_2D);
		temp = glfwGetTime();

		if (!pause && temp - prev >= 0.0333f) {
			// Set the new time
			prev = temp;

			// Wipe the Scene Clean
			glClear(GL_COLOR_BUFFER_BIT);

			textures[curFrame].bind();
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			textures[curFrame].unbind();


			// Actually Update the Window with new buffer
			glfwSwapBuffers(window);
			
			textures[curFrame].destroy();
			if (curFrame + 1 == NUM_FRAMES) {
				curFrame = 0;
			} else {
				curFrame++;
			}
		}

		// Choose Next Load Frame
		if (loadFrame + 1 == NUM_FRAMES) {
			loadFrame = 0;
		} else if (abs(loadFrame - curFrame) < LOAD_BUFFER) {
			loadFrame++;
		}

	}
	VAO.unbind();

	// Clean Up and Close
	VAO.destroy();
	VBO.destroy();
	EBO.destroy();
	shader.destroy();
	for (int i = 0; i < NUM_FRAMES; i++) {
		textures[i].destroy();
	}
	glfwDestroyWindow(window);
	glfwTerminate();
	return EXIT_SUCCESS;
}

void processInput(GLFWwindow* window, bool &pause) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
	else if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		pause = !pause;
	}
}

void framebufferResize(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}