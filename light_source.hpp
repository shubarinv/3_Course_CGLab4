//
// Created by Vladimir Shubarin on 14.01.2021.
//

#ifndef CGLABS__LIGHT_SOURCE_HPP_
#define CGLABS__LIGHT_SOURCE_HPP_
#include <utility>

#include "shader.hpp"
class LightSource {
  enum lightType {
	DIFFUSE,
	AMBIENT,
	DIRECTIONAL,
	POINT
  };
  explicit LightSource(lightType _type,std::string _name) {
	type = _type;
	name = std::move(_name);
  }
  lightType type;
  std::string name;
  virtual void passDataToShader(Shader* shader) = 0;

};

#endif//CGLABS__LIGHT_SOURCE_HPP_
