//
// Created by Vladimir Shubarin on 19.10.2020.
//

#ifndef CGLABS__BUFFER_HPP_
#define CGLABS__BUFFER_HPP_

#include "functions.hpp"
class Buffer {

 public:
  unsigned int rendererID{};
  virtual void bind() const {
	glCall(glBindVertexArray(rendererID));
  }
  [[maybe_unused]] static void unbind() {
	glCall(glBindVertexArray(0));
  }
};

#endif//CGLABS__BUFFER_HPP_