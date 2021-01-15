//
// Created by Vladimir Shubarin on 14.01.2021.
//

#ifndef CGLABS__DIFFUSE_LIGHT_HPP_
#define CGLABS__DIFFUSE_LIGHT_HPP_
#include <utility>

#include "functions.hpp"
#include "light_source.hpp"
class DiffuseLight : public LightSource {
  struct Data {
	glm::vec3 position{};
	glm::vec3 color{};
	float intensity{};
  };
  Data data;

 public:
  explicit DiffuseLight(std::string _name, Data _data) : LightSource(lightType::DIFFUSE, std::move(_name)) {
	data = _data;
  }
  void passDataToShader(Shader* shader) override  {
	shader->setUniform3f("light.position", data.position);
	shader->setUniform3f("light.diffuse", data.color*(bEnabled ? data.intensity : 0));
  }
  void moveTo(glm::vec3 newCoords) override {
	data.position = newCoords;
  }
  void setIntensity(float _intensity) override {
	data.intensity = _intensity;
  }
  void setColor(glm::vec3 _color) override {
	data.color = _color;
  }
};

#endif//CGLABS__DIFFUSE_LIGHT_HPP_
