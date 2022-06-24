#include "stdafx.hpp"

#include "Shader.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "Texture.h"
#include "Camera.h"

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

float deltaTime = 0.0f;
float lastFrame = 0.0f;

// Callback Functions
void processInput(GLFWwindow* window);
void mouseCallback(GLFWwindow* window, double xpos, double ypos);
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
void framebufferCallback(GLFWwindow* window, int width, int height);

// main function/entry point
int main() {
	// Init and set window settings
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	float triangles[] = {
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	};

	GLuint indices[] = {
		// Square Indice Order
		0, 1, 2,  // Triangle 1
		0, 2, 3,  // Triangle 2
		4, 5, 6,  // etc.
		4, 6, 7,
		8, 9, 10,
		8, 10, 11,
		12, 13, 14,
		12, 14, 15,
		16, 17, 18,
		16, 18, 19,
		20, 21, 22,
		20, 22, 23
	};

	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	// Create the window and set it to the current context
	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Camera", NULL, NULL);
	if (window == NULL) {
		LOG("ERROR: FAILED TO CREATE GLFW WINDOW.");
		glfwTerminate();
		return EXIT_FAILURE;
	}

	// Configure the Window
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	glfwSetCursorPos(window, camera.lastX, camera.lastY);

	// Load the GL Functions and set the viewport of the window
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebufferCallback);
	glfwSetCursorPosCallback(window, mouseCallback);
	glfwSetScrollCallback(window, scrollCallback);
	gladLoadGL();

	// Configure the Texture Drawing Formats
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glEnable(GL_DEPTH_TEST);

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
	VBO.init(triangles, sizeof(triangles));
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
		// Update the Time
		double currentFrame = glfwGetTime();
		deltaTime = (float) currentFrame - lastFrame;
		lastFrame = (float) currentFrame;

		// Process Input
		glfwPollEvents();
		processInput(window);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Calculate Camera Position/View
		glm::mat4 view = camera.getViewMatrix();
		shader.setMat4("view", view);

		// Projection Matrix (Depth Perception)
		glm::mat4 projection;
		projection = glm::perspective(glm::radians(camera.zoom), (float) WINDOW_WIDTH / WINDOW_HEIGHT, 0.1f, 100.0f);
		shader.setMat4("projection", projection);

		texture.bind();
		for (int i = 0; i < 10; i++) {
			// Model Matrix (The object we will see)
			glm::mat4 model(1.0);
			model = glm::translate(model, cubePositions[i]);
			float degrees = (float)glfwGetTime() * 25.0f;
			model = glm::rotate(model, (float)glm::radians(degrees), glm::vec3(1.0, 0.3, 0.5));

			// Add the Cube Coordinate Space (Shader needs to be activated before uniform can be loaded)
			shader.setMat4("model", model);

			// Wipe the Scene Clean
			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		}
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



void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
	if (xpos == 0.0f) {
		xpos += static_cast<double>(WINDOW_WIDTH - 1);
		camera.lastX += (WINDOW_WIDTH - 1.0f);
		glfwSetCursorPos(window, xpos, ypos);
	} else if (xpos + 0.1 >= WINDOW_WIDTH) {
		xpos -= static_cast<double>(WINDOW_WIDTH - 1);
		camera.lastX -= (WINDOW_WIDTH - 1.0f);
		glfwSetCursorPos(window, xpos, ypos);
	}

	float x = static_cast<float>(xpos);
	float y = static_cast<float>(ypos);

	float xoffset = x - camera.lastX;
	float yoffset = camera.lastY - y;

	camera.lastX = x;
	camera.lastY = y;

	camera.processMouseMovement(xoffset, yoffset);
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		camera.processKeyboard(Camera::Movement::FORWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		camera.processKeyboard(Camera::Movement::BACKWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		camera.processKeyboard(Camera::Movement::LEFT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		camera.processKeyboard(Camera::Movement::RIGHT, deltaTime);
	}

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
}

void framebufferCallback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
	camera.processMouseScroll((float) yoffset);
}