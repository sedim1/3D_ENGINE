#pragma once

#include<glad/glad.h>
#include"SHADER.h"
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include <string>
#include <vector>

#define MAX_BONE_INFLUENCE 4

using namespace std;

struct Vertex{
	//Vertex Data
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
	//Bone Data
	int boneIDs[MAX_BONE_INFLUENCE]; //bone index that will influence this vertex ivec4
	float boneWeights[MAX_BONE_INFLUENCE];//Weights of each bone vec4

};

struct Texture{
	unsigned int id;
	string type;
	string path;
};

class Mesh{
	public:
		//Mesh data
		vector<Vertex> vertices;
		vector<unsigned int> indices;
		vector<Texture> textures;
		Mesh(vector<Vertex>&data,vector<unsigned int>&indices,vector<Texture>&textures);
		void Draw(ShaderProgram &shader);
	private:
		//render data
		unsigned int VAO, VBO, EBO;
		void setupMesh();
};

