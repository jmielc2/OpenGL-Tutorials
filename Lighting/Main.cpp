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

glm::vec3 lightPosition(0.0f, 0.0f, 0.0f);

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
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	float triangles[] = {
	-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
	 0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
	-0.5f,  0.5f, -0.5f, 0.0f, 0.0f, -1.0f,

	-0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
	-0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f,

	-0.5f,  0.5f,  0.5f, -1.0f, 0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
	-0.5f, -0.5f,  0.5f, -1.0f, 0.0f, 0.0f,

	 0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f,
	 0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 0.0f,

	-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,
	 0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f, 0.0f, -1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f, 0.0f, -1.0f, 0.0f,

	-0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f
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

	// Create the window and set it to the current context
	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Lighting", NULL, NULL);
	if (window == NULL) {
		LOG("ERROR: Failed to create GLFW window.");
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
	if (!gladLoadGL()) {
		LOG("ERROR: Failed to initialize GLAD.");
		glfwDestroyWindow(window);
		glfwTerminate();
		return EXIT_FAILURE;
	}

	// Configure the Texture Drawing Formats
	glEnable(GL_DEPTH_TEST);

	// Create Different Shaders
	Shader objectShader("vertexShader.vs", "fragObjectShader.fs");
	Shader lightShader("vertexShader.vs", "fragLightShader.fs");

	// Create VBOs & 
	VAO objectVAO, lightVAO;
	VBO VBO;
	EBO EBO;

	// Read in the triangle
	VBO.init(triangles, sizeof(triangles));
	EBO.init(indices, sizeof(indices));

	// Cube Object VAO
	objectVAO.bind();
	VBO.bind();
	EBO.bind();
	objectVAO.linkVBO(VBO, 0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) 0);
	objectVAO.linkVBO(VBO, 1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) (3 * sizeof(float)));
	objectVAO.unbind();
	VBO.unbind();
	EBO.unbind();

	// Light Source VAO
	lightVAO.bind();
	VBO.bind();
	EBO.bind();
	lightVAO.linkVBO(VBO, 0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) 0);
	lightVAO.linkVBO(VBO, 1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) (3 * sizeof(float)));
	lightVAO.unbind();
	VBO.unbind();
	EBO.unbind();



	// Set the clear color buffer for wiping/resetting the window
	glClearColor(0.07f, 0.13f, 0.17f, 1.0f);

	while (!glfwWindowShouldClose(window)) {
		// Update the Time
		double currentFrame = glfwGetTime();
		deltaTime = (float) currentFrame - lastFrame;
		lastFrame = (float) currentFrame;

		// Process Input
		glfwPollEvents();
		processInput(window);

		// Clear the Buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Set the Lighting Values
		objectShader.setVec3("objectColor", glm::vec3(1.0f, 0.5f, 0.31f));
		objectShader.setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
		objectShader.setVec3("lightPos", lightPosition);

		// Calculate Camera Position/View
		glm::mat4 view = camera.getViewMatrix();
		objectShader.setMat4("view", view);
		lightShader.setMat4("view", view);

		// Projection Matrix (Depth Perception)
		glm::mat4 projection = glm::perspective(glm::radians(camera.zoom), (float) WINDOW_WIDTH / WINDOW_HEIGHT, 0.1f, 100.0f);
		objectShader.setMat4("projection", projection);
		lightShader.setMat4("projection", projection);

		// Model Matrix (The object we will see)
		glm::mat4 model(1.0);
		float xPos = 3 * cos(glm::radians(currentFrame * 50));
		float yPos = 3 * sin(glm::radians(currentFrame * 50));
		model = glm::translate(model, glm::vec3(xPos, -1.0f, yPos));
		objectShader.setMat4("model", model);
		objectShader.activate();
		objectVAO.bind();
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		
		// Draw the Light Source
		model = glm::mat4(1.0f);
		model = glm::translate(model, lightPosition);
		lightShader.setMat4("model", model);
		lightShader.activate();
		lightVAO.bind();
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

		// Actually Update the Window with new buffer
		glfwSwapBuffers(window);
	}

	// Clean Up and Close
	objectVAO.destroy();
	lightVAO.destroy();
	VBO.destroy();
	EBO.destroy();
	objectShader.destroy();
	lightShader.destroy();
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