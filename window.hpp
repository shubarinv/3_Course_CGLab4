//
// Created by Vladimir Shubarin on 02.12.2020.
//

#ifndef CGLABS__WINDOW_HPP_
#define CGLABS__WINDOW_HPP_
#include "functions.hpp"
class Window {
 private:
  GLFWwindow *window= nullptr; ///< reference to glfw window
  glm::vec2 windowSize{};

 public:
  /**
   * @brief returns window size
   * @return vec2<int>
   */
  [[maybe_unused]] [[nodiscard]] const glm::vec2 &getWindowSize() const {
	return windowSize;
  }
/**
 * @brief returns reference to glfwWindow
 * @return GLFWwindow *
 */
  [[nodiscard]] GLFWwindow *getGLFWWindow() const {
	return window;
  }

  /**
   * @brief init for window class
   * @param size vec2<int>window size
   */
  explicit Window(glm::vec2 size) {
	windowSize = size;
	// GLFW lib init
	glfwSetErrorCallback(glfwErrorHandler);
	if (!glfwInit()) {
	  LOG_S(FATAL) << "GLFW INIT - FAIL"<<std::endl;
	  throw std::runtime_error("Failed to init glfw");
	}
	std::cout << "GLFW init - OK"<<std::endl;

	if (isMac()) {
	  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	  std::cout << "System: MacOS"<<std::endl;
	}
	glfwWindowHint(GLFW_FLOATING, GL_TRUE);
	glfwWindowHint(GLFW_FOCUS_ON_SHOW, GL_TRUE);
	// GLFW Window creation
	bruteforceGLVersion();
	glfwMakeContextCurrent(window);
	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	if (window == nullptr) {
	  LOG_S(FATAL) << "GLFW was unable to create window"<<std::endl;
	  glfwTerminate();
	}
	// GLAD lib init
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
	  LOG_S(FATAL) << "GLAD init Failed"<<std::endl;
	}

	GLint maxShaderTextures;
	GLint maxTotalTextures;
	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxShaderTextures);
	glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &maxTotalTextures);
	glEnable(GL_MULTISAMPLE);
	std::cout << "Status: Using OpenGL v" << glGetString(GL_VERSION)<<std::endl;
	std::cout << "Renderer: " << glGetString(GL_RENDERER)<<std::endl; /* get renderer string */
	std::cout << "Number of textures that can be accessed by the fragment shader: " << maxShaderTextures<<std::endl;
	std::cout << "Total number of textures that can be used " << maxTotalTextures<<std::endl;
	std::cout << "Init DONE!"<<std::endl;
  }
  static void glfwErrorHandler(int error, const char *message) {
	if (error != 65543)
	  LOG_S(ERROR) << "GLFW error: " << error << " " << message<<std::endl;
  }
  ~Window() {
	glfwDestroyWindow(window);
	std::cout << "GLFW window destroyed"<<std::endl;
	std::cout << "Window(" << this << ") destroyed"<<std::endl;
  }
 private:
  /**
   * @brief this function attempts to load latest version of OpenGL
   */
  void bruteforceGLVersion() {
	LOG_SCOPE_F(INFO, "Getting latest OpenGL");
	for (int major = 4; major > 2; major--) {
	  for (int minor = 9; minor > 0; minor--) {
		if (major == 3 & minor == 1) { return; }
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);
		std::string tmp = "Trying OpenGL v" + std::to_string(major) + "." + std::to_string(minor);
		window = glfwCreateWindow(windowSize.x, windowSize.y, "UNSET", nullptr, nullptr);
		if (window == nullptr) {
		  std::cout << tmp << " - FAIL"<<std::endl;
		} else {
		  std::cout << tmp << " - OK"<<std::endl;
		  break;
		}
	  }
	}
  }
};

#endif//CGLABS__WINDOW_HPP_