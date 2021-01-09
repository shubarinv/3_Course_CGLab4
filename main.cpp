
#include "application.hpp"
#include "mesh.hpp"
#include "renderer.hpp"
#include "shader.hpp"

void programQuit([[maybe_unused]] int key, [[maybe_unused]] int action, Application *app) {
  app->close();
  LOG_S(INFO) << "Quiting...";
}

int main(int argc, char *argv[]) {
  Application app({640, 480}, argc, argv);
  Application::setOpenGLFlags();
  app.registerKeyCallback(GLFW_KEY_ESCAPE, programQuit);
  Shader shader("shaders/basic_texture_shader.glsl", true);
  std::vector<float> points = {
	  0.0f, 0.5f, 0.0f,
	  0.5f, -0.5f, 0.0f,
	  -0.5f, -0.5f, 0.0f};
  Mesh test(points);
 // test.setColor(glm::vec3(0.f,0.8f,0.3f));
  test.setTexture("textures/wood.png");
  shader.bind();
  shader.setUniform1i("u_Texture", 0);
  test.compile();

  while (!app.getShouldClose()) {
	shader.reload();
	Renderer::clear();
	test.draw(&shader);

	glCall(glfwSwapBuffers(app.getWindow()->getGLFWWindow()));
	glfwPollEvents();
  }
  glfwTerminate();
  exit(EXIT_SUCCESS);
}