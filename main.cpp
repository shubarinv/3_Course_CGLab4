
#include "application.hpp"
void programQuit(int key, int action, Application *app) {
  app->close();
  LOG_S(INFO) << "Quiting...";
}

int main(int argc, char *argv[]) {
  Application app;
  app.init({640, 480}, argc, argv);
  Application::setOpenGLFlags();
  app.registerKeyCallback(GLFW_KEY_ESCAPE, programQuit);

  while (!app.shouldClose) {
	glClear(GL_COLOR_BUFFER_BIT);
	// 1rst attribute buffer : vertices
	glCall(glfwSwapBuffers(app.getWindow()->getGLFWWindow()));
	glfwPollEvents();
  }
  glfwTerminate();
  exit(EXIT_SUCCESS);
}