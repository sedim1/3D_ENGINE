#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 fragColor;
out vec4 aColor;

void main()
{
	gl_Position=vec4(aPos,1.0f);
	aColor=vec4(fragColor,1.0f);
}
