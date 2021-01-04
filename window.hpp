//
// Created by Vladimir Shubarin on 02.12.2020.
//

#ifndef CGLABS__WINDOW_HPP_
#define CGLABS__WINDOW_HPP_
#include "functions.hpp"
class Window {
 private:
  GLFWwindow *window;
  glm::vec2 windowSize{};

 public:
  const glm::vec2 &getWindowSize() const {
	return windowSize;
  }

  [[nodiscard]] GLFWwindow *getGLFWWindow() const {
	return window;
  }

  Window(glm::vec2 size) {
	windowSize=size;
	glfwSetErrorCallback(glfwErrorHandler);
	if (!glfwInit()) {
	  LOG_S(FATAL) << "GLFW INIT FAILED";
	  throw std::runtime_error("Failed to init glfw");
	}
	LOG_S(INFO) << "GLFW init - OK";

	if (isMac()) {
	  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);// Highest available version for macOS
	  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);// Highest available version for macOS

	  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	  LOG_S(INFO) << "System: MacOS";
	}

	window = glfwCreateWindow(size.x, size.y, "UNSET", nullptr, nullptr);
	glfwMakeContextCurrent(window);
	if (window == nullptr) {
	  LOG_S(FATAL) << "GLFW was unable to create window";
	  glfwTerminate();
	}

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
	  LOG_S(FATAL) << "GLAD init Failed";
	}
	LOG_S(INFO) << "Status: Using OpenGL v" << glGetString(GL_VERSION);
	LOG_S(INFO) << "Renderer: " << glGetString(GL_RENDERER); /* get renderer string */
	GLint maxShaderTextures;
	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxShaderTextures);
	GLint maxTotalTextures;
	glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &maxTotalTextures);
	LOG_S(INFO) << "Number of textures that can be accessed by the fragment shader: " << maxShaderTextures;
	LOG_S(INFO) << "Total number of textures that can be used " << maxTotalTextures;
	glEnable(GL_MULTISAMPLE);
	LOG_S(INFO) << "Init DONE!";
  }
  static void glfwErrorHandler(int error, const char *message) {
	LOG_S(ERROR) << "GLFW error: " << error << " " << message;
  }
  ~Window() {
	glfwDestroyWindow(window);
	LOG_S(INFO) << "GLFW window destroyed";
	LOG_S(INFO) << "Window(" << this << ") destroyed";
  }
};

#endif//CGLABS__WINDOW_HPP_