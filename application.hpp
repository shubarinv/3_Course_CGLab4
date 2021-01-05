//
// Created by Vladimir Shubarin on 03.12.2020.
//

#ifndef CGLABS__APPLICATION_HPP_
#define CGLABS__APPLICATION_HPP_

#include <map>

#include "window.hpp"

class Application {
 private:
  Window *window{};
  typedef void (*functionType)(int, int, Application *);
  std::map<int, functionType> keyPressCallbacks;
  bool shouldClose{false};
 public:
  [[nodiscard]] Window *getWindow() const {
	return window;
  }

  void init(glm::vec2 windowSize, [[maybe_unused]] int argc, [[maybe_unused]] char *argv[]) {
	LOG_S(INFO) << "Hello world!";
	LOG_SCOPE_F(INFO, "Libs init");
	logInit(argc, argv);
	window = new Window(windowSize);
	setOpenGLFlags();
	/// following is required for keyboard related callbacks
	glfwSetKeyCallback(window->getGLFWWindow(), keyCallback);
	glfwSetWindowUserPointer(window->getGLFWWindow(), this);
  }

  void registerKeyCallback(int key, functionType func) {
	auto posOfPreviouslyRegisteredKeyPressCallbacks = keyPressCallbacks.find(key);
	if (posOfPreviouslyRegisteredKeyPressCallbacks != keyPressCallbacks.end()) {
	  LOG_S(INFO) << "Callback for \"" << glfwGetKeyName(key, 0) << "\" was already registered";
	}
	keyPressCallbacks.emplace(key, func);
	LOG_S(INFO) << "New callback was registered";
  }

  [[maybe_unused]] void unregisterKeyCallback(int key) {
	auto posOfPreviouslyRegisteredKeyPressCallbacks = keyPressCallbacks.find(key);
	if (posOfPreviouslyRegisteredKeyPressCallbacks != keyPressCallbacks.end()) {
	  keyPressCallbacks.erase(posOfPreviouslyRegisteredKeyPressCallbacks);
	}
  }

  [[maybe_unused]] static void setOpenGLFlags() {
	glEnable(GL_DEPTH_TEST);

	//glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  }

  void close() {
	shouldClose = true;
	delete (window);
  }

  ~Application() {
	LOG_S(INFO) << "Application destroyed";
  }

  bool getShouldClose() const {
	return shouldClose;
  }
 private:
  void handleKeyboard([[maybe_unused]] GLFWwindow *_window, int key, int scancode, int action, [[maybe_unused]] int mods) {
	auto requiredCallback = keyPressCallbacks.find(key);
	if (requiredCallback != keyPressCallbacks.end()) {
	  LOG_S(INFO) << "Calling registered callback";
	  requiredCallback->second(key, action, this);
	}
  }

  static void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
	auto *app = reinterpret_cast<Application *>(glfwGetWindowUserPointer(window));
	app->handleKeyboard(window, key, scancode, action, mods);
  }

};

#endif //CGLABS__APPLICATION_HPP_