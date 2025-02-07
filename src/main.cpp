#include <glad/glad.h> //Incluir primero glad antes del manejador de ventanas
#include <GLFW/glfw3.h> //
#include <iostream>
#include <string>
#include<SHADER.h>
#include <vector>

//Pasos para crear ventana
//Cargar librerias
//Configuracion de version
//Creacion de Ventana
//Verificacion de la ejecucion correcta de GLAD

using namespace std;
//Especificar el ancho y altura de la ventana
const unsigned int width = 600;
const unsigned int height = 400;

void InitGLFWVersion();
bool GladLoad();
void Framebuffer_Size_Callback(GLFWwindow* window,int w, int h);//Funcion que se va a llamar cada vez que la ventana cambie de tamano
void UpdateWindow(GLFWwindow* window,ShaderProgram* shader);
void GeneraBuffer();

float vertexData[] ={
	//Posicion       //Color
	0.5f,-0.5f,0.0f, 1.0f,0.0f,0.0f,//bottom-righ
	-0.5f,0.5f,0.0f,  0.0f,1.0f,0.0f,//top left
	-0.5f,-0.5f,0.0f, 0.0f,0.0f,1.0f,//bottom left
	0.5f,0.5f,0.0f,  1.0f,0.0f,0.0f,//top right
};

unsigned int indices[]={
	0,1,2,
	1,3,0,
};

unsigned int VBO, VAO, EBO;

int main()
{
	InitGLFWVersion();

	GLFWwindow* window = glfwCreateWindow(width,height,"OPENGL",NULL,NULL); //Expandir ventana, pantalla completa los ultimos dos parametros
	
	if(window==NULL)
	{
		cout<<"ERROR:COULD NOT OPEN WINDOW!"<<endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if(GladLoad()==false)
		return -1;

	vector<string>sources = {"SHADERS/vertexShader.glsl","SHADERS/fragmentShader.glsl"};
	vector<GLenum>types = {GL_VERTEX_SHADER,GL_FRAGMENT_SHADER};
	ShaderProgram program(sources,types);
	cout<<program.ToString()<<endl;
	glfwSetFramebufferSizeCallback(window,Framebuffer_Size_Callback);
	GeneraBuffer();
	UpdateWindow(window,&program);
	glfwTerminate();
	return 0;
}

//Configuracion de version
void InitGLFWVersion()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
	glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
	
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,GL_TRUE);
#endif //__APPLE - retrocompatiilidad con MacOS
}

bool GladLoad()
{
	if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout<<"COULD NOT LOAD GLAD"<<endl; return false;
	}
	return true;
}


void UpdateWindow(GLFWwindow* window,ShaderProgram* shader)
{
	while(!glfwWindowShouldClose(window))
	{
		glClearColor(0.0f,0.0f,0.0f,1.0f); //Color de la ventana
		glClear(GL_COLOR_BUFFER_BIT);// refrescar el buffer Colorear constantemente
		shader->useProgram();
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES,sizeof(indices),GL_UNSIGNED_INT,0);
		glfwSwapBuffers(window);// Limpiar el buffer del frame anterior
		glfwPollEvents();//llamar eventos que nosotros hayamos configurado
	}
}

void Framebuffer_Size_Callback(GLFWwindow* window,int w, int h)
{
	glViewport(0,0,w,h);
}

void GeneraBuffer()
{
	glGenVertexArrays(1,&VAO);
	glGenBuffers(1,&EBO);
	glGenBuffers(1,&VBO);
	glBindVertexArray(VAO);
	//crea VBO
	glBindBuffer(GL_ARRAY_BUFFER,VBO);
	glBufferData(GL_ARRAY_BUFFER,sizeof(vertexData),vertexData,GL_STATIC_DRAW);
	//crea el EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices),indices,GL_STATIC_DRAW);
	//Vertex position attribute enable
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,6 * sizeof(float),(void*)0);
	glEnableVertexAttribArray(0);
	//Color attribute enable
	glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,6 * sizeof(float),(void*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);
	//Desvincular todo despues de haber generado los buffers
	glBindBuffer(GL_ARRAY_BUFFER,0);
	glBindVertexArray(0);
}

