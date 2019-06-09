#pragma once
#include "Shader.h"
#include<vector>
#include "Mesh.h"
#include <assimp/scene.h>

// TODO: support model withour material
class Model
{
public:
	Model()=default;
	~Model()=default;

	Model(char *path);
	void Draw(Shader shader);

private:
	std::vector<Texture> textures_loaded;
	std::vector<Mesh> meshes;
	std::string directory;

	void loadModel(std::string path);
	void processNode(aiNode *node, const aiScene *scene);
	Mesh processMesh(aiMesh *mesh, const aiScene *scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
	unsigned int TextureFromFile(const char *path, const std::string &directory, bool gamma = false);
};

