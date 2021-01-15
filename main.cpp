
#include "application.hpp"
#include "camera.hpp"
#include "diffuse_light.hpp"
#include "mesh.hpp"
#include "renderer.hpp"
#include "shader.hpp"

void programQuit([[maybe_unused]] int key, [[maybe_unused]] int action, Application *app) {
  app->close();
  LOG_S(INFO) << "Quiting...";
}
std::vector<glm::vec3> getCoordsForVertices(double xc, double yc, double size, int n) {
  std::vector<glm::vec3> vertices;
  auto xe = xc + size;
  auto ye = yc;
  vertices.emplace_back(xe, yc, ye);
  double alpha = 0;
  for (int i = 0; i < n - 1; i++) {
	alpha += 2 * M_PI / n;
	auto xr = xc + size * cos(alpha);
	auto yr = yc + size * sin(alpha);
	xe = xr;
	ye = yr;
	vertices.emplace_back(xe, yc, ye);
  }
  return vertices;
}
int main(int argc, char *argv[]) {
  Application app({640, 480}, argc, argv);
  Application::setOpenGLFlags();
  app.registerKeyCallback(GLFW_KEY_ESCAPE, programQuit);

  Shader shader("../shaders/basic_lightmaps_shader.glsl", true);
  shader.bind();

  Mesh mesh("../resources/models/Crate1.obj");
  mesh.addTexture("../textures/crate.png");
  mesh.addTexture("../textures/crate_specular.png");

  Camera camera(app.getWindow()->getWindowSize());
  camera.moveTo({0, 0, 4});
  camera.lookAt({0, 0, 0.1});

 
  mesh.compile();
  std::vector<glm::vec3> cameraPositions = getCoordsForVertices(0, 0, 2, 500);/// координаты для точек гиперболойды
  int cameraPosition{0};

  DiffuseLight light("l_1", {cameraPositions[cameraPosition], {1, 1, 1}, 1});
  shader.setUniform3f("light.ambient", {0.2f, 0.2f, 0.2f});
  shader.setUniform3f("light.specular", {1.0f, 1.0f, 1.0f});

  shader.setUniform1f("material.shininess", 64.0f);
  shader.setUniform1i("material.specular", 1);
  while (!app.getShouldClose()) {
	//rotating scene
	//camera.setModel(glm::rotate(camera.getModel(), 0.001f, {0, 1, 0}));

	light.moveTo(cameraPositions[cameraPosition]);
	light.setIntensity(1);
	shader.setUniform3f("viewPos", camera.getPosition());
	//updating data for shader
	shader.reload();
	camera.passDataToShader(&shader);
	light.passDataToShader(&shader);

	//drawing stuff
	Renderer::clear({0, 0, 0, 1});
	mesh.draw(&shader);
	cameraPosition++;
	if (cameraPosition >= cameraPositions.size()) {
	  cameraPosition = 0;
	}
	glCall(glfwSwapBuffers(app.getWindow()->getGLFWWindow()));
	glfwPollEvents();
  }
  glfwTerminate();
  exit(EXIT_SUCCESS);
}