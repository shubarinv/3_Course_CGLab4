//
// Created by Vladimir Shubarin on 06.01.2021.
//

#ifndef CGLABS__MESH_HPP_
#define CGLABS__MESH_HPP_
#include <utility>

#include "color_buffer.hpp"
#include "functions.hpp"
#include "index_buffer.hpp"
#include "normals_buffer.hpp"
#include "obj_loader.hpp"
#include "renderer.hpp"
#include "texture.hpp"
#include "texture_buffer.hpp"
#include "vertex_array.hpp"
#include "vertex_buffer.hpp"
class Mesh {
  std::vector<float> coordinates;
  std::vector<Buffer> buffers;
  Texture* texture{nullptr};
  VertexArray* vao{nullptr};
  unsigned int indexBufferSize{0};
  IndexBuffer* indexBuffer{nullptr};
  Mesh() = default;
  explicit Mesh(std::vector<glm::vec3> _coordinates) {
	coordinates = vec3ArrayToFloatArray(std::move(_coordinates));
	vao = new VertexArray;
  }
  Mesh* setColor(const std::vector<glm::vec3>& colorsArray) {
	if (colorsArray.size() * 3 != coordinates.size()) {
	  LOG_S(ERROR) << "Amount of elements in colorsArray(" << colorsArray.size() * 3 << ") doesn't match amount vertices<<coordinates<<. Will still try to set colors but this may cause problems";
	}
	addNewBuffer(ColorBuffer(colorsArray), true);
	return this;
  }
  Mesh* setColor(const std::vector<float>& colorsArray) {
	if (colorsArray.size() != coordinates.size()) {
	  LOG_S(ERROR) << "Amount of elements in colorsArray(" << colorsArray.size() << ") doesn't match amount vertices<<coordinates<<. Will still try to set colors but this may cause problems";
	}
	addNewBuffer(ColorBuffer(colorsArray), true);
	return this;
  }

 public:
  Mesh* draw(Shader* shader) {
	if (texture != nullptr)
	  texture->bind();
	if (indexBuffer != nullptr) {
	  Renderer::draw(indexBuffer, vao, shader,indexBufferSize, GL_TRIANGLES);
	} else {
	  Renderer::draw(vao, shader, coordinates.size() / 3, GL_TRIANGLES);
	}
	return this;
  }

  explicit Mesh(std::vector<float> _coordinates) {
	coordinates = std::move(_coordinates);
	vao = new VertexArray;
  }

  explicit Mesh(const std::string& filepath) {
	ObjLoader objLoader;
	auto mesh = objLoader.loadObj(filepath);
	for (auto& vertex : mesh.attrib.vertices) {
	  coordinates.push_back(vertex);
	}
	std::vector<float> texCoords;
	for (auto& texCoord : mesh.attrib.texcoords) {
	  texCoords.push_back(texCoord);
	}
	setTextureCoords(texCoords);
	std::vector<float> normals;
	for (auto& normal : mesh.attrib.normals) {
	  normals.push_back(normal);
	}
	setNormals(normals);
	std::vector<u_int> indices;
	for (auto& index : mesh.attrib.indices) {
	  indices.push_back(index.vertex_index);
	}
	setIndices(indices);
	//mesh.attrib.indices
	/*for(auto&shape:mesh.shapes){

	}*/
	for (auto& material : mesh.materials) {
	  LOG_S(INFO) << material.name;
	}
	vao = new VertexArray;
  }
  Mesh* compile() {
	if (coordinates.empty()) {
	  LOG_S(ERROR) << "Coordinates were not set!";
	  return this;
	}
	addNewBuffer(VertexBuffer(coordinates));// Setting VBO

	fillVAO();
	return this;
  }
  Mesh* setColor(glm::vec3 color) {
	std::vector<float> colors;
	for (auto& coord : coordinates) {
	  colors.push_back(color.r);
	  colors.push_back(color.g);
	  colors.push_back(color.b);
	}
	addNewBuffer(ColorBuffer(colors), true);
	return this;
  }
  Mesh* setTexture(std::string filePath) {
	texture = new Texture(std::move(filePath));
	auto texCoords = Texture::generateTextureCoords(coordinates.size() / 3);
	addNewBuffer(TextureBuffer(texCoords));
	return this;
  }

  Mesh* setNormals(std::vector<glm::vec3> normals) {
	addNewBuffer(NormalsBuffer(std::move(normals)), true);
	return this;
  }
  Mesh* setNormals(std::vector<float> normals) {
	addNewBuffer(NormalsBuffer(std::move(normals)), true);
	return this;
  }
  Mesh* setTextureCoords(std::vector<float> textureCoords) {
	addNewBuffer(TextureBuffer(std::move(textureCoords)), true);
	return this;
  }

  void setIndices(std::vector<unsigned int> indices) {
	indexBufferSize = indices.size();
	indexBuffer = new IndexBuffer(indices);
  }

 private:
  Mesh* addNewBuffer(Buffer _buffer, bool bReplace = false) {
	bool wasReplaced = false;
	for (auto& buffer : buffers) {
	  if (_buffer.bufferType == buffer.bufferType && buffer.bufferType != Buffer::OTHER) {
		if (bReplace) {
		  buffer = _buffer;
		  wasReplaced = true;
		} else {
		  LOG_S(ERROR) << "Can't add buffer; buffer of same type already defined";
		  return this;
		}
	  }
	}
	if (!wasReplaced)
	  buffers.push_back(_buffer);
	return this;
  }
  Mesh* fillVAO() {
	VertexBufferLayout layout3;
	VertexBufferLayout layout2;
	layout3.push<float>(3);///< number of params for each vertex
	layout2.push<float>(2);///< number of params for each vertex
	for (auto& buffer : buffers) {
	  if (buffer.bufferType != Buffer::type::INDEX && buffer.bufferType != Buffer::type::OTHER) {// We skip indexBuffer
		if (buffer.bufferType == Buffer::type::TEXTURE_COORDS) {
		  vao->addBuffer(buffer, layout2, buffer.attributeLocation);
		} else {
		  vao->addBuffer(buffer, layout3, buffer.attributeLocation);
		}
	  }
	}
	return this;
  }
  bool wasBufferDefined(Buffer::type bufferType) {
	for (auto& buffer : buffers) {
	  if (bufferType == buffer.bufferType) {
		return true;
	  }
	}
	return false;
  }
  Buffer* getBufferOfType(Buffer::type bufferType) {
	for (auto& buffer : buffers) {
	  if (bufferType == buffer.bufferType) {
		return &buffer;
	  }
	}
	return nullptr;
  }
};

#endif//CGLABS__MESH_HPP_
