#pragma once
#include"MESH.h"
#include<vector>
#include<string>
#include"SHADER.h"
#include<iostream>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

using namespace Assimp;
using namespace std;

class Model
{
	public:
		Model(const std::string &path);
		void Draw(ShaderProgram &shader);
	private:
		//Model Data
		vector<Mesh> meshes;
		vector<Texture> texturesLoaded;
		std::string directory;
		void loadModel(const std::string &path);
		void processNode(aiNode *node,const aiScene *scene);
		Mesh processMesh(aiMesh *mesh,const aiScene *scene);
		vector<Texture> loadMaterialTextures(aiMaterial *mat,aiTextureType type,std::string typeName);
};

//Function for loading a texture

unsigned int TextureFromFile(const char *path, const string &directory, bool gamma);

