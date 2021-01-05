//
// Created by Vladimir Shubarin on 05.01.2021.
//

#ifndef CGLABS__VERTEX_BUFFER_HPP_
#define CGLABS__VERTEX_BUFFER_HPP_
#include <vector>
#include "functions.hpp"
#include "buffer.hpp"
class VertexBuffer : public Buffer {
 public:

  /**
   * @brief Holds data about vertices
   * @param points std::vector<float>
   */
  explicit VertexBuffer(std::vector<float> points) {
	glGenBuffers(1, &rendererID);
	glBindBuffer(GL_ARRAY_BUFFER, rendererID);
	glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(float), points.data(), GL_STATIC_DRAW);
  }
  void bind() const override {
	glBindBuffer(GL_ARRAY_BUFFER, rendererID);
  }
  static void unbind() {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
  }
  GLuint getID() const {
	return rendererID;
  }
};

#endif //CGLABS__VERTEX_BUFFER_HPP_
