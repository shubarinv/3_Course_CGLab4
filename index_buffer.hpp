//
// Created by Vladimir Shubarin on 08.01.2021.
//

#ifndef CGLABS__INDEX_BUFFER_HPP_
#define CGLABS__INDEX_BUFFER_HPP_

#include "buffer.hpp"
#include "functions.hpp"
class IndexBuffer : public Buffer {

 public:
  /**
   * @brief Holds data about vertices
   * @param points std::vector<float>
   */
  explicit IndexBuffer(const std::vector<unsigned int>& indices) : Buffer({}) {
	bufferType = Buffer::type::INDEX;
	attributeLocation=-1;
	glCall(glGenBuffers(1, &rendererID));
	glCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererID));
	glCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW));
  }
  void bind() {
	glCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererID));
  }
  [[maybe_unused]] static void unbind() {
	glCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
  }

};
#endif//CGLABS__INDEX_BUFFER_HPP_
