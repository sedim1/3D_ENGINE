#include<SHADER.h>
//ShaderProgram methods
ShaderProgram::ShaderProgram(vector<string>&sources,vector<GLenum>&types)
{
	vector<int>shaders;
	int success;
	char infoLog[512];
	//Compile Shaders
	for(int i=0;i<sources.size();i++)
	{
		char* source = loadShaderFile(sources[i].data());
		if(source)
		{
			//cout<<"SHADER TYPE:"<<types[i]<<" SHADER: "<<sources[i]<<endl;
			int shader = compileShader(source,types[i]);
			shaders.push_back(shader);
			free(source);
		}
	}

	//Create Shader Program
	id=glCreateProgram();
	//Attach Shaders to program
	for(int i=0;i<shaders.size();i++)
	{
		//cout<<shaders[i]<<endl;
		glAttachShader(id,shaders[i]);
	}
	//Link program
	glLinkProgram(id);
	glGetProgramiv(id,GL_LINK_STATUS,&success);
	if(!success)
	{
		glGetProgramInfoLog(id,512,NULL,infoLog);
		cout<<"ERROR::LINKING_SHADER_PROGRAM::"<<endl<<infoLog<<endl;
		id=-1;
		return;
	}
	cout<<"::SHADER PROGRAM WAS CREATED SUCCESFULLY::"<<endl;
	//Delete Sources since we dont need them anymore
	for(int i=0;i<shaders.size();i++)
			glDeleteShader(shaders[i]);
	shaders.clear();
}

void ShaderProgram::useProgram()
{
	glUseProgram(id);
}

void ShaderProgram::deleteProgram()
{
	glDeleteProgram(id);
}

string ShaderProgram::ToString()
{
	return "ID: " + to_string(id);
}

//Uniform update methods
void ShaderProgram::setMatrix4f(string uniform,glm::mat4 matrix){
	useProgram();
	int location = glGetUniformLocation(id,uniform.data());
		glUniformMatrix4fv(location,1,GL_FALSE,glm::value_ptr(matrix));
}

//Functions of compiling and linking shaders
char* loadShaderFile(char* source)
{
	char *shader = NULL;
	//OPEN FILE
	FILE *fp = fopen(source,"r");
	if(fp)
	{
		//Get the size of the shader file
		fseek(fp,0,SEEK_END);
		int size = ftell(fp);
		rewind(fp);
		//printf("FileSize: %d\n",size);
		//Read the shader file
		shader = (char*)malloc(sizeof(char)*size+1);
		if(!shader)
			return NULL;
		fread(shader,1,size,fp);
		shader[size] = '\0';
		//cout<<"ShaderFile Loaded:"<<endl<<shader<<endl;
		fclose(fp);
	}
	return shader;
}

int compileShader(char* source,GLenum type)
{
	int shader;
	int success;
	char infoLog[512];
	//Compile shader
	shader=glCreateShader(type);
	glShaderSource(shader,1,&source,NULL);
	glCompileShader(shader);
	glGetShaderiv(shader,GL_COMPILE_STATUS,&success);
	//check if compilation was succesfull
	if(!success)
	{
		glGetShaderInfoLog(shader,512,NULL,infoLog);
		cout<<"ERROR::SHADER::COMPILATION_FAILED"<<infoLog<<endl;
		return -1;
	}
	cout<<"::SHADER COMPILED SUCCESFULLY::"<<endl;
	return shader;
}


