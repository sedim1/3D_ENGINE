#include<CAMERA.h>

Camera::Camera()
{
	position = vec3(0.0f,0.0f,0.0f);	
	front = vec3(0.0f,0.0f,-1.0f);	
	up = vec3(0.0f,1.0f,0.0f);
	view = mat4(1.0f);
	projection = mat4(1.0f);
	fov = 45.0f;
	pitch = roll = yaw = 0.0f;

}

Camera::Camera(vec3 eye,vec3 center,vec3 UP,float fov)
{
	position = eye;	
	front = center;	
	up = UP;
	view = mat4(1.0f);
	projection = mat4(1.0f);
	this->fov = fov;
	pitch = roll = yaw = 0.0f;
}

void Camera::spectatorMode(GLFWwindow* window)
{
	float cameraSpeed = 0.2f;
	float offsett = 1.0;
	//Update Front Vector
	if(glfwGetKey(window,GLFW_KEY_RIGHT)==GLFW_PRESS) //TURN RIGHT the yaw
		yaw+=offsett;
	if(glfwGetKey(window,GLFW_KEY_LEFT)==GLFW_PRESS) //TURN LEFT the yaw
		yaw-=offsett;
	if(glfwGetKey(window,GLFW_KEY_UP)==GLFW_PRESS)//Look Up
		pitch+=offsett;
	if(glfwGetKey(window,GLFW_KEY_DOWN)==GLFW_PRESS)//Look Up
		pitch-=offsett;
	if(pitch>89.0f)
		pitch = 89.0f;
	if(pitch < -89.0f)
		pitch = -89.0f;
	//Update the camera direction
	front.x = cos(degToRad(&(yaw))) * cos(degToRad(&(pitch)));	
	front.y = sin(degToRad(&(pitch)));	
	front.z = sin(degToRad(&(yaw))) * cos(degToRad(&(pitch)));
	front=normalize(front);//glm_vec3_normalize(cam->cameraFront);
	
	//Move camera towards dirdectrior or eft rigth
	if(glfwGetKey(window,GLFW_KEY_E)==GLFW_PRESS) //Up
		position.y+=cameraSpeed;

	if(glfwGetKey(window,GLFW_KEY_Q)==GLFW_PRESS) //Down
		position.y-=cameraSpeed;
	
	if(glfwGetKey(window,GLFW_KEY_W) == GLFW_PRESS)//MOVE FORWARD
	{
		//cameraPos += cameraSpeed * cameraFront;
		position += cameraSpeed * front;
	}
	if(glfwGetKey(window,GLFW_KEY_S) == GLFW_PRESS)//MOVE BACKWARD
	{
		//cameraPos -= cameraSpeed * cameraFront;
		position-= cameraSpeed * front;
	}
	if(glfwGetKey(window,GLFW_KEY_A) == GLFW_PRESS)//MOVE LEFT
	{
		//Operation doin here
		//cameraPos -= normalized(cross(cameraFront,cameraUp)) * cameraSpeed;
		/*vec3 res, aux;
		glm_vec3_crossn(cam->cameraFront,cam->cameraUp,aux);
		glm_vec3_scale(aux,cameraSpeed,aux);
		glm_vec3_sub(cam->cameraPos,aux,res);
		glm_vec3_copy(res,cam->cameraPos);*/
		position -= normalize(cross(front,up)) * cameraSpeed;
	}
	if(glfwGetKey(window,GLFW_KEY_D) == GLFW_PRESS)//MOVE RIGHT
	{
		//Operation doin here
		//cameraPos += normalized(cross(cameraFront,cameraUp)) * cameraFront;
		/*vec3 res, aux;
		glm_vec3_crossn(cam->cameraFront,cam->cameraUp,aux);
		glm_vec3_scale(aux,cameraSpeed,aux);
		glm_vec3_add(cam->cameraPos,aux,res);
		glm_vec3_copy(res,cam->cameraPos);*/
		position += normalize(cross(front,up)) * cameraSpeed;
	}
}

void Camera::updateViewMatrix(){
	view = lookAt(position,position+front,up);
}

void Camera::updatePerspectiveProjection(int w, int h, float zNear, float zFar){
	projection = perspective(fov,(float)w/(float)h,zNear,zFar);
}
