//
// Created by Vladimir Shubarin on 02.12.2020.
//
#ifndef CG_LABS_FUNCTIONS_HPP
#define CG_LABS_FUNCTIONS_HPP

#if defined(__APPLE__)
#endif
#define LOGURU_WITH_STREAMS 1
#include <glad/glad.h>
#include <GLFW/glfw3.h>


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "libs/loguru.cpp"

void logInit([[maybe_unused]] int argc, [[maybe_unused]] char *argv[]) {
  loguru::init(argc, argv);

  // Put every log message in "everything.log":
  loguru::add_file("main.log", loguru::Truncate, loguru::Verbosity_MAX);
}
#if defined(__APPLE__)
#define ASSERT(X) \
	if (!(X)) __builtin_trap()
#endif

#if defined (__WIN32__)
#define ASSERT(X) \
  if (!(X)) __debugbreak()
#endif
/**
 * @brief checks if GL function call failed or succeeded
 **/
#define glCall(x)  \
  glClearErrors(); \
  x;               \
  ASSERT(glLogCall(#x, __FILE__, __LINE__))

/**
 * @brief checks if program being built on windows
 * @return true if on Windows, false otherwise
 **/
[[maybe_unused]] bool isWindows() {
#if defined (__WIN32__)
  return true;
#endif
  return false;
}
/**
 * @brief checks if program being built on macOS
 * @return true if on macOS, false otherwise
 **/
bool isMac() {
#if defined (__APPLE__)
  return true;
#endif
  return false;
}
/**
 * @brief checks if program being built on Linux
 * @return true if on Linux, false otherwise
 **/
bool isLinux() {
#if defined (__unix__)
  return true;
#endif
  return false;
}
std::string glErrorToString(GLenum error) {
  switch (error) {
	case GL_INVALID_ENUM:return "INVALID ENUM";
	case GL_INVALID_VALUE:return "INVALID VALUE";
	case GL_INVALID_OPERATION:return "INVALID OPERATION";
	case GL_STACK_OVERFLOW:return " STACK OVERFLOW";
	case GL_STACK_UNDERFLOW:return " STACK UNDERFLOW";
	case GL_OUT_OF_MEMORY:return "OUT OF MEMORY";
	case GL_INVALID_FRAMEBUFFER_OPERATION:return "INVALID FRAMEBUFFER OPERATION";
	default:return std::to_string(error);
  }
}

/**
 * @brief checks opengl errors
 * @warning do not call this function directly, use glCall() instead
 **/
bool glLogCall(const char *function = {}, const char *file = {}, int line = -1) {
  if (function == nullptr && file == nullptr && line == -1) {
	LOG_S(ERROR) << "glLogCall() should not be called directly, wrap GL call in glCall()";
	return true;
  }
  while (GLenum error = glGetError()) {
	LOG_S(ERROR) << "OpenGL error: " << glErrorToString(error) << " in file " << file << " in function " << function << " at line: " << line;
	throw std::runtime_error("OpenGL error: "+glErrorToString(error));
  }
  return true;
}

/**
 * @brief clears opengl errors
 **/
void glClearErrors() {
  while (glGetError() != GL_NO_ERROR);
}

#endif // CG_LABS_FUNCTIONS_HPP