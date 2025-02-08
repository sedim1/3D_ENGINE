#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 fragColor;
out vec4 aColor;
uniform mat4 projection;
uniform mat4 view;

void main()
{
	gl_Position= projection * view * vec4(aPos,1.0f);
	aColor=vec4(fragColor,1.0f);
}
