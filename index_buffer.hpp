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
  explicit IndexBuffer(std::vector<float> points) : Buffer(std::move(points)) {
	bufferType = Buffer::type::INDEX;
	attributeLocation=-1;
  }
  explicit IndexBuffer(std::vector<glm::vec3> coordinates) : Buffer(std::move(vec3ArrayToFloatArray(std::move(coordinates)))) {
	bufferType = Buffer::type::INDEX;
	attributeLocation=-1;
  }
};
#endif//CGLABS__INDEX_BUFFER_HPP_
