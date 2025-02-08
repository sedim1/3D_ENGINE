#include"MESH.h"

Mesh::Mesh(vector<Vertex>&data,vector<unsigned int>&indices,vector<Texture>&textures){
	this->vertices = data;
	this->indices = indices;
	this->textures = textures;
	std::cout<<"This mesh has"<<this->textures.size()<<"Textures"<<std::endl;
	setupMesh();
}


void Mesh::setupMesh()
{
	glGenVertexArrays(1,&VAO);
	glGenBuffers(1,&VBO);
	glGenBuffers(1,&EBO);
	//Passing data to the VBO and EBO
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER,VBO);
	glBufferData(GL_ARRAY_BUFFER,vertices.size() * sizeof(Vertex),&vertices[0],GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,indices.size() * sizeof(unsigned int),&indices[0],GL_STATIC_DRAW);
	//Enabling attributes
	//Position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(Vertex),(void*)0);
	//Normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,sizeof(Vertex),(void*)offsetof(Vertex,Normal));
	//Texture
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,sizeof(Vertex),(void*)offsetof(Vertex,TexCoords));
	glBindVertexArray(0);
}

void Mesh::Draw(ShaderProgram &shader)
{
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	for(unsigned int i = 0; i < textures.size();i++)
	{
		//Types of texture we can have anything we only need to justify it with a naming convention with a number to have n numbers of any type
		//Ex: uniform sampler 2D texture_typeN; Keep in mind that we add the texture in ascending form
		glActiveTexture(GL_TEXTURE0+i);//Activate respective texture unit before binding
		string number;
		string name = textures[i].type;
		if(name == "texture_diffuse") //Diffuse properties te texture has
			number = to_string(diffuseNr++);
		if(name == "texture_specular") //Specular propertie the texture has
			number = to_string(diffuseNr++);
		shader.setInt((name+number),i);
		glBindTexture(GL_TEXTURE_2D,textures[i].id);
	}
	glActiveTexture(GL_TEXTURE0);
	//Draw Mesh
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(),GL_UNSIGNED_INT,0);
	glBindVertexArray(0);
}
