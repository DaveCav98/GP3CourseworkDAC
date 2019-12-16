#pragma once
#include "Mesh.h"
#include "Vertex.h"
#include <assimp/assimp/Importer.hpp>
#include <assimp/assimp/scene.h>
#include <assimp/assimp/postprocess.h>

class Model
{
private:
	std::vector<Mesh*> m_meshes;
	std::string m_directory;

	void Load();
	void processNode(aiNode* node, const aiScene* scene);
	Mesh* processMesh(aiMesh* mesh, const aiScene* scene);

public:
	Model(std::string path);
	std::vector<Mesh*> GetMeshes() { return m_meshes; }
};