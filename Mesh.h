#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include "Shader.h"

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
	glm::vec3 Tangent;
	glm::vec3 Bitangent;
};
struct Texture
{
	signed int id;
	std::string type;
	std::string path;
};
class Mesh
{
public:
	Mesh() = default;
	~Mesh() = default;
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
	void Draw(Shader shader);

private:
	// VAO会存储之后所有顶点属性调用的设置如glEnableVertexAttribArray,glVertexAttribPointer及其VBO,EBO
	// vbo vertex buffer object
	unsigned int VAO, VBO, EBO;
	void setupMesh();
};

