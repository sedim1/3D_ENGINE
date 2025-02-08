#pragma once

#include<glad/glad.h>
#include"SHADER.h"
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include <string>
#include <vector>

using namespace std;

struct Vertex{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
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

