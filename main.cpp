
#include "application.hpp"
#include "camera.hpp"
#include "diffuse_light.hpp"
#include "lights_manager.hpp"
#include "mesh.hpp"
#include "renderer.hpp"
#include "shader.hpp"

LightsManager *lightsManager;
int selectedTask{0};
int useTextures{0};
int currentLightPosition{0};
int lightMovementSpeed{1};
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
	  selectedTask--;
	}
	if (key == GLFW_KEY_KP_3) {
	  selectedTask++;
	}
	LOG_S(INFO) << "SelectedTask: " << selectedTask;
	switch (selectedTask) {
	  case 0:
		lightsManager->getLightByNameDir("1_1")->setColor({0.8, 0, 0.8});
		lightsManager->getLightByNameDir("1_1")->moveTo({0, 0, 10});
		break;

	  case 1:
		lightsManager->getLightByNameDir("1_1")->setColor({0.8, 0.8, 0.8});
		lightsManager->getLightByNameDir("1_1")->moveTo({0, 0, 10});
		break;
	  default:
		break;
	}
  }
}
void drawWithTextures([[maybe_unused]] int key, [[maybe_unused]] int action, [[maybe_unused]] Application *app) {
  if (action == GLFW_RELEASE) {
	if (key == GLFW_KEY_KP_0) {
	  useTextures = 0;
	}
	if (key == GLFW_KEY_KP_2) {
	  useTextures = 1;
	}
	LOG_S(INFO) << "Drawing with Textures: " << useTextures;
  }
}
void wasdKeyPress([[maybe_unused]] int key, [[maybe_unused]] int action, [[maybe_unused]] Application *app) {
  if ((key == GLFW_KEY_W) && (action == GLFW_PRESS || action == GLFW_REPEAT))
	camera->ProcessKeyboard(FORWARD, deltaTime);
  if ((key == GLFW_KEY_S) && (action == GLFW_PRESS || action == GLFW_REPEAT))
	camera->ProcessKeyboard(BACKWARD, deltaTime);
  if ((key == GLFW_KEY_A) && (action == GLFW_PRESS || action == GLFW_REPEAT))
	camera->ProcessKeyboard(LEFT, deltaTime);
  if ((key == GLFW_KEY_D) && (action == GLFW_PRESS || action == GLFW_REPEAT))
	camera->ProcessKeyboard(RIGHT, deltaTime);
}
void adjustLightMovementSpeed([[maybe_unused]] int key, [[maybe_unused]] int action, [[maybe_unused]] Application *app) {
  if (action == GLFW_RELEASE) {
	if (selectedTask == 3) {
	  if (key == GLFW_KEY_KP_ADD) {
		lightMovementSpeed++;
	  }
	  if (key == GLFW_KEY_KP_SUBTRACT) {
		lightMovementSpeed--;
	  }
	}
  }
}

void arrowKeyPress([[maybe_unused]] int key, [[maybe_unused]] int action, [[maybe_unused]] Application *app) {
  if ((key == GLFW_KEY_UP) && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
	if (selectedTask == 1) {
	  auto currLightPos = lightsManager->getLightByNameDir("1_1")->getPosition();
	  lightsManager->getLightByNameDir("1_1")->moveTo({currLightPos.x, currLightPos.y, currLightPos.z - 0.5f});
	}
  }
  if ((key == GLFW_KEY_DOWN) && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
	if (selectedTask == 1) {
	  auto currLightPos = lightsManager->getLightByNameDir("1_1")->getPosition();
	  lightsManager->getLightByNameDir("1_1")->moveTo({currLightPos.x, currLightPos.y, currLightPos.z + 0.5f});
	}
  }
  if ((key == GLFW_KEY_LEFT) && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
	if (selectedTask == 0) {
	  auto currColor = lightsManager->getLightByNameDir("1_1")->getColor();
	  if (currColor.b - 0.05 <= 0) {
		lightsManager->getLightByNameDir("1_1")->setColor({currColor.r, currColor.g, 0});
	  } else {
		lightsManager->getLightByNameDir("1_1")->setColor({currColor.r, currColor.g, currColor.b - 0.05f});
	  }
	}
	if (selectedTask == 1) {
	  auto currLightPos = lightsManager->getLightByNameDir("1_1")->getPosition();
	  lightsManager->getLightByNameDir("1_1")->moveTo({currLightPos.x - 0.5, currLightPos.y, currLightPos.z});
	}
  }
  if ((key == GLFW_KEY_RIGHT) && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
	if (selectedTask == 0) {
	  auto currColor = lightsManager->getLightByNameDir("1_1")->getColor();
	  if (currColor.b + 0.05 >= 1) {
		lightsManager->getLightByNameDir("1_1")->setColor({currColor.r, currColor.g, 1});
	  } else {
		lightsManager->getLightByNameDir("1_1")->setColor({currColor.r, currColor.g, currColor.b + 0.05f});
	  }
	}
	if (selectedTask == 1) {
	  auto currLightPos = lightsManager->getLightByNameDir("1_1")->getPosition();
	  lightsManager->getLightByNameDir("1_1")->moveTo({currLightPos.x + 0.5, currLightPos.y, currLightPos.z});
	}
  }
}
void changeIntensity([[maybe_unused]] int key, [[maybe_unused]] int action, [[maybe_unused]] Application *app) {
  if (selectedTask == 2) {
	if ((key == GLFW_KEY_P) && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
	  lightsManager->getLightByNameDir("3_1")->setIntensity(lightsManager->getLightByNameDir("3_1")->getIntensity() + 0.05f);
	}
	if ((key == GLFW_KEY_Y) && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
	  lightsManager->getLightByNameDir("3_1")->setIntensity(lightsManager->getLightByNameDir("3_1")->getIntensity() - 0.05f);
	}
	if(lightsManager->getLightByNameDir("3_1")->getIntensity()>1){
	  lightsManager->getLightByNameDir("3_1")->setIntensity(0.1);
	}
	if(lightsManager->getLightByNameDir("3_1")->getIntensity()<0){
	  lightsManager->getLightByNameDir("3_1")->setIntensity(0);
	}
  }
}

void changeDrawMode([[maybe_unused]] int key, [[maybe_unused]] int action, [[maybe_unused]] Application *app) {
  if (action == GLFW_RELEASE) {
	if(key==GLFW_KEY_KP_7){
	  glDepthFunc(GL_LESS);
	  glCall(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
	}
	if(key==GLFW_KEY_KP_8){
	  glCall(glPointSize(10));
	  glCall(glPolygonMode(GL_FRONT_AND_BACK, GL_POINT));
	}
	if(key==GLFW_KEY_KP_9){
	  glCall(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));
	}
  }
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
  Application app({1280, 720}, argc, argv);
  Application::setOpenGLFlags();
  app.registerKeyCallback(GLFW_KEY_ESCAPE, programQuit);
  app.registerKeyCallback(GLFW_KEY_KP_1, changeTask);
  app.registerKeyCallback(GLFW_KEY_KP_3, changeTask);

  app.registerKeyCallback(GLFW_KEY_KP_2, drawWithTextures);
  app.registerKeyCallback(GLFW_KEY_KP_0, drawWithTextures);

  app.registerKeyCallback(GLFW_KEY_KP_ADD, adjustLightMovementSpeed);
  app.registerKeyCallback(GLFW_KEY_KP_SUBTRACT, adjustLightMovementSpeed);

  app.registerKeyCallback(GLFW_KEY_P, changeIntensity);
  app.registerKeyCallback(GLFW_KEY_Y, changeIntensity);

  app.registerKeyCallback(GLFW_KEY_KP_7, changeDrawMode);
  app.registerKeyCallback(GLFW_KEY_KP_8, changeDrawMode);
  app.registerKeyCallback(GLFW_KEY_KP_9, changeDrawMode);

  app.registerKeyCallback(GLFW_KEY_W, wasdKeyPress);
  app.registerKeyCallback(GLFW_KEY_A, wasdKeyPress);
  app.registerKeyCallback(GLFW_KEY_S, wasdKeyPress);
  app.registerKeyCallback(GLFW_KEY_D, wasdKeyPress);

  app.registerKeyCallback(GLFW_KEY_UP, arrowKeyPress);
  app.registerKeyCallback(GLFW_KEY_DOWN, arrowKeyPress);
  app.registerKeyCallback(GLFW_KEY_LEFT, arrowKeyPress);
  app.registerKeyCallback(GLFW_KEY_RIGHT, arrowKeyPress);

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

  lightsManager = new LightsManager;
  lightsManager->addLight(DiffuseLight("1_1", {{0, 0, 10}, {0.8, 0, 0.8}, 0.8}));
  lightsManager->addLight(DiffuseLight("3_1", {{10, 0, 10}, {0.8, 0, 0.8}, 0.8}));
  lightsManager->addLight(DiffuseLight("3_2", {{-10, 0, 10}, {0.8, 0.8, 0}, 0.8}));
  lightsManager->addLight(DiffuseLight("4_1", {{-10, 0, 10}, {0.8, 0.8, 0}, 1}));

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
	switch (useTextures) {
	  case 0:
		camera->passDataToShader(&shader_color);
		lightsManager->passDataToShader(&shader_color);
		break;
	  case 1:
		camera->passDataToShader(&shader_tex);
		lightsManager->passDataToShader(&shader_tex);
	  default:
		break;
	}
	//drawing stuff

	switch (selectedTask) {
	  case 0:
		lightsManager->getLightByNameDir("1_1")->enable();
		lightsManager->getLightByNameDir("3_1")->disable();
		lightsManager->getLightByNameDir("3_2")->disable();
		lightsManager->getLightByNameDir("4_1")->disable();
		if (useTextures == 0) Sphere.draw(&shader_color);
		if (useTextures == 1) Sphere.draw(&shader_tex);
		//lightsManager.getLightByNameDir("1_1")->moveTo(cameraPositions[cameraPosition1]);
		break;
	  case 1:
		lightsManager->getLightByNameDir("1_1")->enable();
		lightsManager->getLightByNameDir("1_1")->setColor({0.8f, 0.8f, 0.8f});
		lightsManager->getLightByNameDir("3_1")->disable();
		lightsManager->getLightByNameDir("3_2")->disable();
		lightsManager->getLightByNameDir("4_1")->disable();

		if (useTextures == 0) Sphere.draw(&shader_color);
		if (useTextures == 1) Sphere.draw(&shader_tex);
		//lightsManager.getLightByNameDir("1_1")->moveTo(cameraPositions[cameraPosition1]);
		break;
	  case 2:
		lightsManager->getLightByNameDir("1_1")->disable();
		lightsManager->getLightByNameDir("3_1")->enable();
		lightsManager->getLightByNameDir("3_2")->enable();
		lightsManager->getLightByNameDir("4_1")->disable();
		//Pyramid
		if (useTextures == 0) Pyramid.draw(&shader_color);
		if (useTextures == 1) Pyramid.draw(&shader_tex);
		break;
	  case 3:
		lightsManager->getLightByNameDir("1_1")->disable();
		lightsManager->getLightByNameDir("3_1")->disable();
		lightsManager->getLightByNameDir("3_2")->disable();
		lightsManager->getLightByNameDir("4_1")->enable();
		lightsManager->getLightByNameDir("4_1")->moveTo(lightsPositions[currentLightPosition]);
		if (useTextures == 0) Cube.draw(&shader_color);
		if (useTextures == 1) Cube.draw(&shader_tex);

		//cube
		break;
	  default:
		break;
	}

	if (currentLightPosition >= (int)lightsPositions.size()) {
	  currentLightPosition = 0;
	}
	if (currentLightPosition < 0) {
	  currentLightPosition = (int)lightsPositions.size() - currentLightPosition - abs(lightMovementSpeed);
	}
	currentLightPosition += lightMovementSpeed;
	glCall(glfwSwapBuffers(app.getWindow()->getGLFWWindow()));
	glfwPollEvents();
  }
  glfwTerminate();
  exit(EXIT_SUCCESS);
}
