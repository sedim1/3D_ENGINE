#ifndef CAMERA_H
#define CAMERA_H

#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<math.h>
#include<radians.h>
#include<SHADER.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

using namespace glm;

class Camera
{
	public:
		Camera();
		Camera(vec3 eye,vec3 center,vec3 UP,float fov);
		vec3 front;
		vec3 position;
		vec3 up;
		mat4 view;
		mat4 projection;
		float fov;
		float pitch;
		float yaw;
		float roll;
		void spectatorMode(GLFWwindow* window);
		void updateViewMatrix();
		void updatePerspectiveProjection(int w, int h, float zNear, float zFar);
};
#endif
