
#include "application.hpp"
#include "shader.hpp"
void programQuit([[maybe_unused]] int key, [[maybe_unused]] int action, Application *app) {
  app->close();
  LOG_S(INFO) << "Quiting...";
}

int main(int argc, char *argv[]) {
  Application app({640, 480}, argc, argv);
  Application::setOpenGLFlags();
  app.registerKeyCallback(GLFW_KEY_ESCAPE, programQuit);
  Shader shader("shaders/basic_layout_shader.glsl",true);

  float points[] = {
	  0.0f,  0.5f,  0.0f,
	  0.5f, -0.5f,  0.0f,
	  -0.5f, -0.5f,  0.0f
  };

  GLuint vbo = 0;
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), points, GL_STATIC_DRAW);
  GLuint vao = 0;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

  while (!app.getShouldClose()) {
    shader.reload();
	glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
	shader.bind();
	glBindVertexArray(vao);
	// draw points 0-3 from the currently bound VAO with current in-use shader
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glCall(glfwSwapBuffers(app.getWindow()->getGLFWWindow()));
	glfwPollEvents();
  }
  glfwTerminate();
  exit(EXIT_SUCCESS);
}