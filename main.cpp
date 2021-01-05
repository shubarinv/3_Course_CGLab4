
#include "application.hpp"
#include "shader.hpp"
#include "vertex_buffer.hpp"
#include "vertex_array.hpp"
void programQuit([[maybe_unused]] int key, [[maybe_unused]] int action, Application *app) {
  app->close();
  LOG_S(INFO) << "Quiting...";
}

int main(int argc, char *argv[]) {
  Application app({640, 480}, argc, argv);
  Application::setOpenGLFlags();
  app.registerKeyCallback(GLFW_KEY_ESCAPE, programQuit);
  Shader shader("shaders/basic_layout_shader.glsl", true);

  std::vector<float> points = {
	  0.0f, 0.5f, 0.0f,
	  0.5f, -0.5f, 0.0f,
	  -0.5f, -0.5f, 0.0f
  };

  VertexBuffer vbo(points);
  VertexArray vao;
  VertexBufferLayout layout;
  layout.push<float>(3);      ///< number of params for each vertex
  vao.addBuffer(vbo, layout);
  vbo.bind();

  while (!app.getShouldClose()) {
	shader.reload();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	shader.bind();
	vao.bind();
	// draw points 0-3 from the currently bound VAO with current in-use shader
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glCall(glfwSwapBuffers(app.getWindow()->getGLFWWindow()));
	glfwPollEvents();
  }
  glfwTerminate();
  exit(EXIT_SUCCESS);
}