#include"MODEL.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Model::Model(const std::string &path){
	loadModel(path);
}

void Model::modelInfo(){
	int vertexMemory=0;
	int indexMemory=0;
	float totalMemory=0.0f;
	for(int i = 0; i < meshes.size(); i++)
	{
		cout<<"Mesh: "<<i+1<<endl;
		cout<<"Textures Assinged:"<<endl;
		for(int j = 0;j<meshes[i].textures.size();j++)
		{
			cout<<"Texture "<<i+1<<endl;
			cout<<"Type: "<<meshes[i].textures[j].type<<" ";
			cout<<"Path: "<<meshes[i].textures[j].path<<endl;
		}
		vertexMemory += meshes[i].vertices.size();
		indexMemory += meshes[i].indices.size();
	}
	totalMemory = (((vertexMemory)*sizeof(Vertex)/sizeof(Vertex)) + ((indexMemory)*sizeof(unsigned int)/sizeof(unsigned int))) * 0.000006;
	cout<<"Size: "<<totalMemory<<"MB"<<endl;
}

void Model::Draw(ShaderProgram &shader){
	for(unsigned int i = 0;i<meshes.size();i++)
		meshes[i].Draw(shader);
}

void Model::loadModel(const std::string &path)
{
	//Importing the model to read the file and save it in a assimp scene
	Importer importer;
	const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs); //path and post processing arguments options
	//Check if everything was loaded correctly
	if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout<<"ERROR:ASSIMP::"<<importer.GetErrorString()<<std::endl;
		return;
	}
	directory = path.substr(0,path.find_last_of('/'));
	processNode(scene->mRootNode,scene); //Start processing the node
	
	std::cout<<"LOADED::"<<directory<<std::endl;
}

void Model::processNode(aiNode *node,const aiScene *scene)
{
	//process all the nodes Meshes
	for(unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];//Acces to current data of a mesh
		meshes.push_back(processMesh(mesh,scene)); //Add mesh to the model
	}

	//Do the same for the childrens node
	for(unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i],scene);
	}
}

Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene){
	vector<Vertex> vertexData;
	vector<unsigned int> indices;
	vector<Texture> textures;
	//Process vertexData (vertices)
	for(int i = 0; i < mesh->mNumVertices;i++)
	{
		Vertex data;
		data.Position = glm::vec3(mesh->mVertices[i].x,mesh->mVertices[i].y,mesh->mVertices[i].z);
		data.Normal = glm::vec3(mesh->mNormals[i].x,mesh->mNormals[i].y,mesh->mNormals[i].z);
		if(mesh->mTextureCoords[0])//Check if mesh contains texture coordinates
			data.TexCoords = glm::vec2(mesh->mTextureCoords[0][i].x,mesh->mTextureCoords[0][i].y);
		else
			data.TexCoords = glm::vec2(0.0f,0.0f);
		vertexData.push_back(data);
	}
	//Process indices
	for(unsigned int i = 0;i<mesh->mNumFaces;i++)
	{
		aiFace face = mesh->mFaces[i]; //Accces the current face
		for(unsigned int j = 0;j<face.mNumIndices;j++)//Access the indices of the face
			indices.push_back(face.mIndices[j]);
	}
	//Process material(textures)
	if(mesh->mMaterialIndex >= 0)
	{
		aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
		vector<Texture> diffuseMaps = loadMaterialTextures(material,aiTextureType_DIFFUSE,"texture_diffuse");//Regular base texture
		textures.insert(textures.end(),diffuseMaps.begin(),diffuseMaps.end());
		vector<Texture> specularMaps = loadMaterialTextures(material,aiTextureType_SPECULAR,"specular_diffuse");//
		textures.insert(textures.end(),specularMaps.begin(),specularMaps.end());
		vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
        	textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
        	std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
        	textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
	}
	else{
		cout<<"No Materials are on file"<<endl;
	}
	return Mesh(vertexData,indices,textures);
}

vector<Texture> Model::loadMaterialTextures(aiMaterial *mat,aiTextureType type,std::string typeName)
{
	vector<Texture> textures;
	for(unsigned int i = 0; i < mat->GetTextureCount(type);i++)
	{
		aiString str;
		mat->GetTexture(type, i,&str);
		bool skip = false;
		//Check if x texture has been loaded to not load it again
		for(unsigned int j = 0; j < texturesLoaded.size(); j++)
		{
			if(std::strcmp(texturesLoaded[j].path.data(),str.C_Str())==0)
			{
				textures.push_back(texturesLoaded[j]);
				skip=true;
				//cout<<texturesLoaded[j].path<<" has been loaded"<<endl;
				break;
			}
		}
		if(!skip){
			Texture texture;
			texture.id = TextureFromFile(str.C_Str(),directory,false);//Load texture with stb_image
			texture.type = typeName;
			texture.path = str.C_Str();
			textures.push_back(texture);
			texturesLoaded.push_back(texture);//Add to loaded texture
		}
	}
	return textures;
}


unsigned int TextureFromFile(const char *path, const string &directory, bool gamma)
{
	string filename = string(path);
   	filename = directory + '/' + filename;
	
	unsigned int textureID;
   	glGenTextures(1, &textureID);

    	int width, height, nrComponents;

	std::cout<<"Loading Texture: "<<filename<<std::endl;
    	unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    	if (data)
    	{
		//std::cout<<"Texture found!! nrComponents:"<<nrComponents<<std::endl;
        	GLenum format;
        	if (nrComponents == 1)
        	    format = GL_RED;
        	else if (nrComponents == 3)
        	    format = GL_RGB;
        	else if (nrComponents == 4)
        	    format = GL_RGBA;

        	glBindTexture(GL_TEXTURE_2D, textureID);
        	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        	glGenerateMipmap(GL_TEXTURE_2D);

        	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        	stbi_image_free(data);
    	}	
    	else
    	{
    	    std::cout << "Texture failed to load at path: " << path << std::endl;
    	    stbi_image_free(data);
    	}

    return textureID;
}


