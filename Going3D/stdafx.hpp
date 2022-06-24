#pragma once

#if DEBUG
	#include <iostream>
	#define LOG(x) std::cout << x << std::endl;
#else
	#define LOG(x)
#endif

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <sstream>
#include <cerrno>
#include <fstream>
