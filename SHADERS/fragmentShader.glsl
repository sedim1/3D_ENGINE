#version 330 core
in vec4 aColor;
out vec4 FragColor;
  
uniform Material material;

void main()
{
	FragColor=aColor;
}
