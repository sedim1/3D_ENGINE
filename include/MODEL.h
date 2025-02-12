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
#include<map>

using namespace Assimp;
using namespace std;

struct BoneInfo{
	int id; //id index in final bone matrix
	glm::mat4 offset;/*Offset matrix transforms vertex from model space to bonespace*/
};

class Model
{
	public:
		Model(const std::string &path);
		void Draw(ShaderProgram &shader);
		void modelInfo();
	private:
		//Model Data
		vector<Mesh> meshes;
		vector<Texture> texturesLoaded;
		std::string directory;
		void loadModel(const std::string &path);
		void processNode(aiNode *node,const aiScene *scene);
		Mesh processMesh(aiMesh *mesh,const aiScene *scene);
		vector<Texture> loadMaterialTextures(aiMaterial *mat,aiTextureType type,std::string typeName);
		//BoneData Stuff
		std::map<string,BoneInfo> m_BoneInfoMap;
		int m_BoneCounter = 0;
		auto& GetBoneInfoMap(){
			return m_BoneInfoMap;
		}
		auto& GetBone() {
			return m_BoneCounter;
		}
		void SetVertexDataToDefault(Vertex& vertex);
		void SetVertexBoneData(Vertex& vertex,int boneID,float weight);
		void ExtractBoneWeightForVertices(std::vector<Vertex>& vertices,aiMesh* mesh,const aiScene* scene);


};

//Function for loading a texture
unsigned int TextureFromFile(const char *path, const string &directory, bool gamma);

//Assimp helper functions
glm::mat4 ConvertMatrixToGLMFormat(const aiMatrix4x4& from);

