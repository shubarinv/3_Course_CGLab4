//
// Created by Vladimir Shubarin on 07.01.2021.
//

#ifndef CGLABS__VERTEX_HPP_
#define CGLABS__VERTEX_HPP_
#include "functions.hpp"

struct Vertex {
  // position
  glm::vec3 Position;
  // normal
  glm::vec3 Normal;
  // texCoords
  glm::vec2 TexCoords;
  // tangent
  glm::vec3 Tangent;
  // bitangent
  glm::vec3 Bitangent;
};
#endif//CGLABS__VERTEX_HPP_
