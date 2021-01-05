
#include "application.hpp"
#include "shader.hpp"
#include "vertex_buffer.hpp"
#include "vertex_array.hpp"
#include "renderer.hpp"
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

  VertexBufferLayout layout;
  layout.push<float>(3);      ///< number of params for each vertex

  VertexArray vao;
  vao.addBuffer(vbo, layout);

  while (!app.getShouldClose()) {
	shader.reload();
	Renderer::clear();
	Renderer::draw(&vao, &shader, 3, GL_TRIANGLES);

	glCall(glfwSwapBuffers(app.getWindow()->getGLFWWindow()));
	glfwPollEvents();
  }
  glfwTerminate();
  exit(EXIT_SUCCESS);
}