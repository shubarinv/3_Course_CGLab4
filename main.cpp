
#include "application.hpp"
#include "camera.hpp"
#include "diffuse_light.hpp"
#include "lights_manager.hpp"
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

  Shader shader("../shaders/multiple_diffuse_shader.glsl", true);
  shader.bind();
  shader.setUniform1i("u_Texture", 0);
  shader.setUniform1i("numDiffLights", 1);

  Mesh mesh("../resources/models/Crate1.obj");
  mesh.addTexture("../textures/crate.png");
  mesh.compile();

  Camera camera(app.getWindow()->getWindowSize());
  camera.moveTo({0, 0, 4});
  camera.lookAt({0, 0, 0.1});

  std::vector<glm::vec3> cameraPositions = getCoordsForVertices(0, 0, 2, 500);/// координаты для точек гиперболойды
  int cameraPosition1{0};
  int cameraPosition2=cameraPositions.size()/2;

  LightsManager lightsManager;
  lightsManager.addLight(DiffuseLight("1_1", {cameraPositions[cameraPosition1], {1, 0, 0}, 1}));
  lightsManager.addLight(DiffuseLight("1_2", {cameraPositions[cameraPosition2], {0, 0, 1}, 1}));
  while (!app.getShouldClose()) {
	//rotating scene
	//camera.setModel(glm::rotate(camera.getModel(), 0.004f, {0, 1, 0}));

	lightsManager.getLightByNameDir("1_1")->moveTo(cameraPositions[cameraPosition1]);
	lightsManager.getLightByNameDir("1_2")->moveTo(cameraPositions[cameraPosition2]);


	//updating data for shader
	shader.reload();
	camera.passDataToShader(&shader);
	lightsManager.passDataToShader(&shader);

	//drawing stuff
	Renderer::clear({0, 0, 0, 1});
	mesh.draw(&shader);
	cameraPosition1++;
	if (cameraPosition1 >= cameraPositions.size()) {
	  cameraPosition1 = 0;
	}
	cameraPosition2++;
	if (cameraPosition2 >= cameraPositions.size()) {
	  cameraPosition2 = 0;
	}
	glCall(glfwSwapBuffers(app.getWindow()->getGLFWWindow()));
	glfwPollEvents();
  }
  glfwTerminate();
  exit(EXIT_SUCCESS);
}