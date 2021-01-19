//
// Created by Vladimir Shubarin on 12.10.2020.
//

#ifndef CGLABS__SHADER_HPP_
#define CGLABS__SHADER_HPP_

#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>

#include "functions.hpp"

class Shader {

  /**
   * @brief contains code for vertex and fragment shader
   */
  struct ShaderProgramSource {
	std::string vertexShader{};  ///< @brief program for vertex shader
	std::string fragmentShader{};///< @brief program for fragment shader
  };

 public:
  /**
   * @brief
   * @param _filepath path to file containing shader source code
   */
  [[maybe_unused]] explicit Shader(const std::string &_filepath) {
	LOG_SCOPE_F(INFO, "Shader init");
	filepath = _filepath;
	source = parseShader();
	rendererID = createShader();
	std::cout << "Created shader with id: " << rendererID<<std::endl;
  }
  ~Shader() {
	glCall(glDeleteProgram(rendererID));
	std::cout << "destroyed shader with id: " << rendererID<<std::endl;
  }
  /**
   * @brief Activates shader.
   */
  [[maybe_unused]] void bind() const {
	glCall(glUseProgram(rendererID));
  }
  [[maybe_unused]] static void unbind() {
	glCall(glUseProgram(0));
  }
  /**
   * @brief Sets uniform of 1 value with type int in shader
   * @param name name of the uniform
   * @param value value to set uniform to
   */
  [[maybe_unused]] void setUniform1i(const std::string &name, GLint value) {
	glCall(glUniform1i(getUniformLocation(name), value));
  }
  /**
   * @brief Sets uniform with vec4
   * @param name name of the uniform
   * @param value value to set uniform to
   */
  [[maybe_unused]] void setUniform4f(const std::string &name, glm::vec4 vec4) {
	glCall(glUniform4f(getUniformLocation(name), vec4.x, vec4.y, vec4.z, vec4.w));
  }
  [[maybe_unused]] void setUniform3f(const std::string &name, glm::vec3 vec3) {
	glCall(glUniform3f(getUniformLocation(name), vec3.x, vec3.y, vec3.z));
  }
  /**
  * @brief Sets uniform with mat4
  * @param name name of the uniform
  * @param value value to set uniform to
  */
  [[maybe_unused]] void setUniformMat4f(const std::string &name, const glm::mat4 &matrix) {
	glCall(glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
  }bool doesUniformExist(const std::string &name){
	if(getUniformLocation(name,true)==-1){
	  return false;
	}
	else{
	  return true;
	}
  }

 private:
  ShaderProgramSource source;
  std::unordered_map<std::string, int> uniformLocationCache;///< cache of uniforms locations

  /**
   * @brief gets location of uniform in shader
   * @param name name of uniform
   * @returns location of uniform if successful else -1
   */
  [[nodiscard]] GLint getUniformLocation(const std::string &name,bool allowedToFail=false) {
	if (uniformLocationCache.find(name) != uniformLocationCache.end()) {
	  return uniformLocationCache[name];
	}
	glCall(int location = glGetUniformLocation(rendererID, name.c_str()));
	if (location == -1) {
	  if(!allowedToFail)LOG_S(WARNING) << "Uniform with name: " <<name<< " does not exist"<<std::endl;
	}

	uniformLocationCache[name] = location;
	return location;
  }

  unsigned int rendererID{};
  std::string filepath{};

  /**
   * @brief Parses file that contains the shader
   * @returns source code for vertex and fragment shader.
   */
  ShaderProgramSource parseShader() {
	std::cout << "Parsing shader at: " << filepath.c_str()<<std::endl;
	std::ifstream stream(filepath);
	if (stream.fail()) {
	  LOG_S(FATAL) << "Unable to open shader file at: " << filepath.c_str()<<std::endl;
	  throw std::runtime_error("Unable to open shader file");
	}
	std::string line;
	std::stringstream ss[2];
	enum class shaderType {
	  NONE = -1,
	  VERTEX = 0,
	  FRAGMENT = 1
	};
	shaderType type = shaderType::NONE;
	while (std::getline(stream, line)) {
	  if (line.find("#shader") != std::string::npos) {
		if (line.find("vertex") != std::string::npos) {
		  type = shaderType::VERTEX;
		} else if (line.find("fragment") != std::string::npos) {
		  type = shaderType::FRAGMENT;
		}
	  } else {
		ss[(int)type] << line << "\n";
	  }
	}
	std::cout << "Shader parsed successfully"<<std::endl;
	return {ss[0].str(), ss[1].str()};
  }

  /**
   * @brief Compiles shader program
   * @param type fragment or vertex
   * @param source source code of shader program
   * @return returns reference to compiled shader program
   */
  static unsigned int compileShader(int type, std::string &source) {
	std::cout << "Trying to compile " << (type == GL_VERTEX_SHADER ? "VertexShader " : "FragmentShader ");
	unsigned int id = glCreateShader(type);
	const char *src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE) {
	  int length;
	  glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
	  std::string error;
	  char buf[length];
	  error = "Application::CompileShader() failed to compile ";
	  error += (type == GL_VERTEX_SHADER ? "VertexShader " : "FragmentShader ");
	  glGetShaderInfoLog(id, length, &length, buf);
	  error += buf;
	  LOG_S(FATAL) << error<<std::endl;
	  throw std::runtime_error(error);
	  glDeleteShader(id);
	}
	std::cout << "shader compiled successfully"<<std::endl;
	return id;
  }

  /**
 * @brief Creates shader that can be used
 * @return reference to final shader program
 */
  unsigned int createShader() {
	unsigned int program = glCreateProgram();
	unsigned int vShader = compileShader(GL_VERTEX_SHADER, source.vertexShader);
	unsigned int fShader = compileShader(GL_FRAGMENT_SHADER, source.fragmentShader);
	glCall(glAttachShader(program, vShader));
	glCall(glAttachShader(program, fShader));
	glCall(glLinkProgram(program));
	glCall(glValidateProgram(program));

	glCall(glDeleteShader(vShader));
	glCall(glDeleteShader(fShader));

	return program;
  }
};

#endif//CGLABS__SHADER_HPP_