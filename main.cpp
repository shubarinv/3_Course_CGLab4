
#include "application.hpp"
#include "camera.hpp"
#include "diffuse_light.hpp"
#include "lights_manager.hpp"
#include "mesh.hpp"
#include "renderer.hpp"
#include "shader.hpp"

int selectedOptionX{0};
int selectedOptionY{0};
float lastX = 0;
float lastY = 0;
bool firstMouse = true;
// timing
double deltaTime = 0.0f;// time between current frame and last frame
double lastFrame = 0.0f;
Camera *camera;

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
void wasdKeyPress([[maybe_unused]] int key, [[maybe_unused]] int action, [[maybe_unused]] Application *app){
  if ((key == GLFW_KEY_W) &&(action== GLFW_PRESS||action==GLFW_REPEAT))
	camera->ProcessKeyboard(FORWARD, deltaTime);
  if ((key == GLFW_KEY_S) &&(action== GLFW_PRESS||action==GLFW_REPEAT))
	camera->ProcessKeyboard(BACKWARD, deltaTime);
  if ((key == GLFW_KEY_A) &&(action== GLFW_PRESS||action==GLFW_REPEAT))
	camera->ProcessKeyboard(LEFT, deltaTime);
  if ((key == GLFW_KEY_D) &&(action== GLFW_PRESS||action==GLFW_REPEAT))
	camera->ProcessKeyboard(RIGHT, deltaTime);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow *window, double xpos, double ypos) {
  if (firstMouse) {
	lastX = xpos;
	lastY = ypos;
	firstMouse = false;
  }

  double xoffset = xpos - lastX;
  double yoffset = lastY - ypos;// reversed since y-coordinates go from bottom to top

  lastX = xpos;
  lastY = ypos;

  camera->ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
  camera->ProcessMouseScroll(yoffset);
}

int main(int argc, char *argv[]) {
  Application app({640, 480}, argc, argv);
  Application::setOpenGLFlags();
  app.registerKeyCallback(GLFW_KEY_ESCAPE, programQuit);
  app.registerKeyCallback(GLFW_KEY_KP_1, changeTask);
  app.registerKeyCallback(GLFW_KEY_KP_3, changeTask);
  app.registerKeyCallback(GLFW_KEY_KP_2, drawWithTextures);
  app.registerKeyCallback(GLFW_KEY_KP_0, drawWithTextures);
  app.registerKeyCallback(GLFW_KEY_W, wasdKeyPress);
  app.registerKeyCallback(GLFW_KEY_A, wasdKeyPress);
  app.registerKeyCallback(GLFW_KEY_S, wasdKeyPress);
  app.registerKeyCallback(GLFW_KEY_D, wasdKeyPress);
  lastX = app.getWindow()->getWindowSize().x / 2.0f;
  lastY = app.getWindow()->getWindowSize().y / 2.0f;

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

  std::vector<glm::vec3> lightsPositions = getCoordsForVertices(0, 0, 2, 500);/// координаты для точек гиперболойды
  int lightPosition1{0};
  int lightPosition2 = lightsPositions.size() / 2;

  LightsManager lightsManager;
  lightsManager.addLight(DiffuseLight("1_1", {{0, 0, 10}, {0.8, 0.8, 0.8}, 1}));
  lightsManager.addLight(DiffuseLight("3_1", {{10, 0, 0}, {0.8, 0, 0.8}, 1}));
  lightsManager.addLight(DiffuseLight("3_2", {{-10, 0, 10}, {0.8, 0.8, 0}, 1}));
  lightsManager.addLight(DiffuseLight("4_1", {{-10, 0, 10}, {0.8, 0.8, 0}, 1}));

  // camera
  camera = new Camera(glm::vec3(0.0f, 0.0f, 6.0f));
  camera->setWindowSize(app.getWindow()->getWindowSize());


  glfwSetCursorPosCallback(app.getWindow()->getGLFWWindow(), mouse_callback);
  glfwSetScrollCallback(app.getWindow()->getGLFWWindow(), scroll_callback);

  while (!app.getShouldClose()) {
	auto currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
	//updating data for shader
	shader_tex.reload();
	shader_color.reload();
	Renderer::clear({0, 0, 0, 1});
	switch (selectedOptionY) {
	  case 0:
		camera->passDataToShader(&shader_color);
		lightsManager.passDataToShader(&shader_color);
		break;
	  case 1:
		camera->passDataToShader(&shader_tex);
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
		lightsManager.getLightByNameDir("4_1")->moveTo(lightsPositions[lightPosition1]);
		if (selectedOptionY == 0) Cube.draw(&shader_color);
		if (selectedOptionY == 1) Cube.draw(&shader_tex);

		//cube
		break;
	  default:
		break;
	}

	lightPosition1++;
	if (lightPosition1 >= lightsPositions.size()) {
	  lightPosition1 = 0;
	}
	lightPosition2++;
	if (lightPosition2 >= lightsPositions.size()) {
	  lightPosition2 = 0;
	}
	glCall(glfwSwapBuffers(app.getWindow()->getGLFWWindow()));
	glfwPollEvents();
  }
  glfwTerminate();
  exit(EXIT_SUCCESS);
}
