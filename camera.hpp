//
// Created by Vladimir Shubarin on 18.12.2020.
//

#ifndef CGLABS__CAMERA_HPP_
#define CGLABS__CAMERA_HPP_
#include "functions.hpp"
class Camera {
  glm::vec3 position{};
  glm::vec3 lookingAt{};
  glm::mat4 projection{};
  glm::mat4 model{};
  glm::mat4 view;

 public:
  Camera() {
	projection = glm::perspective(
		glm::radians(90.f),// Вертикальное поле зрения в радианах. Обычно между 90&deg; (очень широкое) и 30&deg; (узкое)
		4.0f / 3.0f,       // Отношение сторон. Зависит от размеров вашего окна. Заметьте, что 4/3 == 800/600 == 1280/960
		0.1f,              // Ближняя плоскость отсечения. Должна быть больше 0.
		100.0f             // Дальняя плоскость отсечения.
	);
	model = glm::mat4(1.0f);
  }

  explicit Camera(glm::vec2 windowSize) {
	projection = glm::perspective(
		glm::radians(90.f),         // Вертикальное поле зрения в радианах. Обычно между 90&deg; (очень широкое) и 30&deg; (узкое)
		windowSize.x / windowSize.y,// Отношение сторон. Зависит от размеров вашего окна. Заметьте, что 4/3 == 800/600 == 1280/960
		0.1f,                       // Ближняя плоскость отсечения. Должна быть больше 0.
		100.0f                      // Дальняя плоскость отсечения.
	);
	model = glm::mat4(1.0f);
  }
  void lookAt(glm::vec3 lookAt) {
	lookingAt = lookAt;
	updateView();
  }
  void moveTo(glm::vec3 moveTo) {
	position = moveTo;
	updateView();
  }
  void setModel(glm::mat4 _model) {
	model = _model;
  }
  glm::mat4 getModel() {
	return model;
  }
  glm::mat4 getProjection() {
	return projection;
  }
  glm::mat4 getView() {
	view = glm::lookAt(
		position,
		lookingAt,
		glm::vec3(0, 1, 0));
	return view;
  }
  glm::mat4 getMVP() {
	return projection * view * model;
  }
  void updateView() {
	view = glm::lookAt(
		position,
		lookingAt,
		glm::vec3(0, 1, 0));
  }
};

#endif//CGLABS__CAMERA_HPP_