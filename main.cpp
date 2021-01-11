
#include "application.hpp"
#include "camera.hpp"
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
  Shader shader("../shaders/basic_texture_shader.glsl", true);
  Mesh mesh("../resources/models/Tesla Model.obj");
  Camera camera(app.getWindow()->getWindowSize());

  camera.moveTo({0, 0, 3});
  camera.lookAt({0, 0, 1});

  shader.bind();
  shader.setUniform1i("u_Texture", 0);
  mesh.compile();

  while (!app.getShouldClose()) {
	camera.setModel(glm::rotate(camera.getModel(), 0.006f, {0, 1, 0}));
	shader.setUniformMat4f("u_MVP", camera.getMVP());
	shader.reload();
	Renderer::clear({0.6,0.6,0.6,1});
	mesh.draw(&shader);

	glCall(glfwSwapBuffers(app.getWindow()->getGLFWWindow()));
	glfwPollEvents();
  }
  glfwTerminate();
  exit(EXIT_SUCCESS);
}