
#include "application.hpp"
#include "camera.hpp"
#include "diffuse_light.hpp"
#include "lights_manager.hpp"
#include "mesh.hpp"
#include "renderer.hpp"
#include "shader.hpp"

int selectedOptionX{0};
int selectedOptionY{0};

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

void programQuit([[maybe_unused]] int key, [[maybe_unused]] int action, Application *app) {
  app->close();
  LOG_S(INFO) << "Quiting...";
}

void changeTask([[maybe_unused]] int key, [[maybe_unused]] int action, [[maybe_unused]] Application *app) {
  if (action == GLFW_RELEASE) {
	if (key == GLFW_KEY_KP_1) {
	  selectedOptionX--;
	}
	if (key == GLFW_KEY_KP_3) {
	  selectedOptionX++;
	}
	LOG_S(INFO) << "SelectedTask: " << selectedOptionX;
  }
}
void drawWithTextures([[maybe_unused]] int key, [[maybe_unused]] int action, [[maybe_unused]] Application *app) {
  if (action == GLFW_RELEASE) {
	if (key == GLFW_KEY_KP_0) {
	  selectedOptionY = 0;
	}
	if (key == GLFW_KEY_KP_2) {
	  selectedOptionY = 1;
	}
	LOG_S(INFO) << "Drawing with Textures: " << selectedOptionY;
  }
}

int main(int argc, char *argv[]) {
  Application app({640, 480}, argc, argv);
  Application::setOpenGLFlags();
  app.registerKeyCallback(GLFW_KEY_ESCAPE, programQuit);
  app.registerKeyCallback(GLFW_KEY_KP_1, changeTask);
  app.registerKeyCallback(GLFW_KEY_KP_3, changeTask);
  app.registerKeyCallback(GLFW_KEY_KP_2, drawWithTextures);
  app.registerKeyCallback(GLFW_KEY_KP_0, drawWithTextures);

  Shader shader_tex("../shaders/multiple_diffuse_shader_tex.glsl", false);
  shader_tex.bind();
  shader_tex.setUniform1i("u_Texture", 0);
  shader_tex.setUniform1i("numDiffLights", 1);

  Shader shader_color("../shaders/multiple_diffuse_shader_color.glsl", false);
  shader_color.bind();
  shader_color.setUniform1i("numDiffLights", 1);

  Mesh Cube("../resources/models/Crate1.obj");
  Cube.addTexture("../textures/crate.png");
  Cube.setColor(glm::vec3{1, 1, 1});
  Cube.compile();

  Mesh Sphere("../resources/models/sphere.obj");
  Sphere.addTexture("../textures/crate.png");
  Sphere.setColor(glm::vec3{1, 1, 1});
  Sphere.compile();

  Mesh Pyramid("../resources/models/Pyramid.obj");
  Pyramid.addTexture("../textures/crate.png");
  Pyramid.setColor(glm::vec3{1, 1, 1});
  Pyramid.compile();

  Camera camera(app.getWindow()->getWindowSize());
  camera.moveTo({0, 0, 4});
  camera.lookAt({0, 0, 0.1});

  std::vector<glm::vec3> cameraPositions = getCoordsForVertices(0, 0, 2, 500);/// координаты для точек гиперболойды
  int cameraPosition1{0};
  int cameraPosition2 = cameraPositions.size() / 2;

  LightsManager lightsManager;
  lightsManager.addLight(DiffuseLight("1_1", {{0, 0, 10}, {0.8, 0.8, 0.8}, 1}));
  lightsManager.addLight(DiffuseLight("3_1", {{10, 0, 0}, {0.8, 0, 0.8}, 1}));
  lightsManager.addLight(DiffuseLight("3_2", {{-10, 0, 10}, {0.8, 0.8, 0}, 1}));
  lightsManager.addLight(DiffuseLight("4_1", {{-10, 0, 10}, {0.8, 0.8, 0}, 1}));
  while (!app.getShouldClose()) {
	//updating data for shader
	shader_tex.reload();
	shader_color.reload();
	Renderer::clear({0, 0, 0, 1});
	switch (selectedOptionY) {
	  case 0:
		camera.passDataToShader(&shader_color);
		lightsManager.passDataToShader(&shader_color);
		break;
	  case 1:
		camera.passDataToShader(&shader_tex);
		lightsManager.passDataToShader(&shader_tex);
	  default:
		break;
	}
	//drawing stuff

	switch (selectedOptionX) {
	  case 0:
		lightsManager.getLightByNameDir("1_1")->enable();
		lightsManager.getLightByNameDir("3_1")->disable();
		lightsManager.getLightByNameDir("3_2")->disable();
		lightsManager.getLightByNameDir("4_1")->disable();
		if (selectedOptionY == 0) Sphere.draw(&shader_color);
		if (selectedOptionY == 1) Sphere.draw(&shader_tex);
		//lightsManager.getLightByNameDir("1_1")->moveTo(cameraPositions[cameraPosition1]);
		break;
	  case 1:
		lightsManager.getLightByNameDir("1_1")->enable();
		lightsManager.getLightByNameDir("1_1")->setColor({0.8f, 0.8f, 0.8f});
		lightsManager.getLightByNameDir("3_1")->disable();
		lightsManager.getLightByNameDir("3_2")->disable();
		lightsManager.getLightByNameDir("4_1")->disable();

		if (selectedOptionY == 0) Sphere.draw(&shader_color);
		if (selectedOptionY == 1) Sphere.draw(&shader_tex);
		//lightsManager.getLightByNameDir("1_1")->moveTo(cameraPositions[cameraPosition1]);
		break;
	  case 2:
		lightsManager.getLightByNameDir("1_1")->disable();
		lightsManager.getLightByNameDir("3_1")->enable();
		lightsManager.getLightByNameDir("3_2")->enable();
		lightsManager.getLightByNameDir("4_1")->disable();
		//Pyramid
		if (selectedOptionY == 0) Pyramid.draw(&shader_color);
		if (selectedOptionY == 1) Pyramid.draw(&shader_tex);
		break;
	  case 3:
		lightsManager.getLightByNameDir("1_1")->disable();
		lightsManager.getLightByNameDir("3_1")->disable();
		lightsManager.getLightByNameDir("3_2")->disable();
		lightsManager.getLightByNameDir("4_1")->enable();
		lightsManager.getLightByNameDir("4_1")->moveTo(cameraPositions[cameraPosition1]);
		if (selectedOptionY == 0) Cube.draw(&shader_color);
		if (selectedOptionY == 1) Cube.draw(&shader_tex);

		//cube
		break;
	  default:
		break;
	}

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