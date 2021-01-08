//
// Created by Vladimir Shubarin on 06.01.2021.
//

#ifndef CGLABS__MESH_HPP_
#define CGLABS__MESH_HPP_
#include <utility>

#include "color_buffer.hpp"
#include "functions.hpp"
#include "renderer.hpp"
#include "vertex_array.hpp"
#include "vertex_buffer.hpp"
class Mesh {
  std::vector<float> coordinates;
  std::vector<Buffer> buffers;
  VertexArray *vao;
  Mesh() = default;
  explicit Mesh(std::vector<glm::vec3> _coordinates) {
	coordinates = vec3ArrayToFloatArray(std::move(_coordinates));
	vao = new VertexArray;
  }
  void setColor(const std::vector<glm::vec3> &colorsArray) {
	if (colorsArray.size() * 3 != coordinates.size()) {
	  LOG_S(ERROR)<<"Amount of elements in colorsArray("<<colorsArray.size()*3<<") doesn't match amount vertices<<coordinates<<. Will still try to set colors but this may cause problems";
	}
	addNewBuffer(ColorBuffer(colorsArray), true);
  }
  void setColor(const std::vector<float> &colorsArray) {
	if (colorsArray.size() != coordinates.size()) {
	  LOG_S(ERROR)<<"Amount of elements in colorsArray("<<colorsArray.size()<<") doesn't match amount vertices<<coordinates<<. Will still try to set colors but this may cause problems";
	}
	addNewBuffer(ColorBuffer(colorsArray), true);
  }

 public:
  void draw(Shader *shader) {
	Renderer::draw(vao, shader, coordinates.size() / 3, GL_TRIANGLES);
  }

  explicit Mesh(std::vector<float> _coordinates) {
	coordinates = std::move(_coordinates);
	vao = new VertexArray;
  }
  void compile() {
	if (coordinates.empty()) {
	  LOG_S(ERROR) << "Coordinates were not set!";
	  return;
	}
	addNewBuffer(VertexBuffer(coordinates));// Setting VBO
	fillVAO();
  }
  void setColor(glm::vec3 color) {
	std::vector<float> colors;
	for (auto &coord : coordinates) {
	  colors.push_back(color.r);
	  colors.push_back(color.g);
	  colors.push_back(color.b);
	}
	addNewBuffer(ColorBuffer(colors), true);
  }
 private:
  void addNewBuffer(Buffer _buffer, bool bReplace = false) {
	bool wasReplaced = false;
	for (auto &buffer : buffers) {
	  if (_buffer.bufferType == buffer.bufferType && buffer.bufferType != Buffer::OTHER) {
		if (bReplace) {
		  buffer = _buffer;
		  wasReplaced = true;
		} else {
		  LOG_S(ERROR) << "Can't add buffer; buffer of same type already defined";
		  return;
		}
	  }
	}
	if (!wasReplaced)
	  buffers.push_back(_buffer);
  }
  void fillVAO() {
	VertexBufferLayout layout;
	layout.push<float>(3);///< number of params for each vertex
	for (auto &buffer : buffers) {
	  if (buffer.bufferType != Buffer::type::INDEX && buffer.bufferType != Buffer::type::OTHER)
		vao->addBuffer(buffer, layout, buffer.attributeLocation);
	}
  }
  bool wasBufferDefined(Buffer::type bufferType) {
	for (auto &buffer : buffers) {
	  if (bufferType == buffer.bufferType) {
		return true;
	  }
	}
	return false;
  }
};

#endif//CGLABS__MESH_HPP_
