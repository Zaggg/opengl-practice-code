#pragma once

#include <glad/glad.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

class Shader
{
public:
	Shader()=default;
	~Shader()=default;

	unsigned int ID;
	Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
	void use();
	// uniform tools function
	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
	void setVec3(const std::string& name, float value1, float value2, float value3) const;
	void setVec3(const std::string &name, const glm::vec3 &value) const;
	void setMat4(const std::string& name, glm::mat4 value) const;
};

