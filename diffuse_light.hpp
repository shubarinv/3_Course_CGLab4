//
// Created by Vladimir Shubarin on 14.01.2021.
//

#ifndef CGLABS__DIFFUSE_LIGHT_HPP_
#define CGLABS__DIFFUSE_LIGHT_HPP_
#include <utility>

#include "light_source.hpp"
#include "functions.hpp"
class DiffuseLight:public LightSource {
  struct data{
	glm::vec3 position{};
	glm::vec3 color{};
	float intensity{};
  };
  explicit DiffuseLight(std::string _name,data _data):LightSource(lightType::DIFFUSE,std::move(_name)){
	data=_data;
  }

  void passDataToShader(Shader* shader) override{
	shader.setUniform3f("lightPos", position);
	shader.setUniform3f("lightColor", color);
	shader.setUniform1f("intensity", intensity);
  }
};

#endif//CGLABS__DIFFUSE_LIGHT_HPP_
