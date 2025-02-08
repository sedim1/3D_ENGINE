#ifndef SHADER_H
#define SHADER_H

#include<iostream>
#include<stdlib.h>
#include<vector>
#include<string>
#include<glad/glad.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

using namespace std;

class ShaderProgram
{
	public:
		unsigned int id;
		ShaderProgram(vector<string>&sources,vector<GLenum>&types);
		void deleteProgram();
		string ToString();
		void useProgram();
		void setMatrix4f(string uniform,glm::mat4 matrix);
};

//Functions for loading shaders sources
char* loadShaderFile(char* source);
int compileShader(char* source,GLenum type);
bool linkPrograms(vector<int>&shaders);

#endif
