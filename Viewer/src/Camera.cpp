#include "Camera.h"
#include <cmath>
#include <imgui/imgui.h>
#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <nfd.h>
#include "Renderer.h"
#include "Scene.h"
#include <glm/gtx/transform.hpp>

Camera::Camera()
{
	glm::mat4x4 normalmat = { 1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1 };
	view_transformation = normalmat;
}

Camera::~Camera()
{
	
}
void Camera::SetCameraLookAt(const glm::vec3& eye, const glm::vec3& at, const glm::vec3& up)
{
	
	//view_transformation = glOrtho(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble near, GLdouble far);
}
glm::mat4& Camera::LookAt(const glm::vec3& eye, const glm::vec3& at, const glm::vec3& up)
{
	glm::vec3 z = glm::normalize(at - eye);
	glm::vec4 z2 = glm::vec4(z, 0.0);
	glm::vec3 x = glm::normalize(glm::cross(up,z));
	glm::vec4 x2 = glm::vec4(x, 0.0);
	glm::vec3 y = glm::normalize(glm::cross(z,x));
	glm::vec4 y2 = glm::vec4(y, 0.0);
	glm::vec4 t = glm::vec4(0.0, 0.0, 0.0, 1.0);
	glm::mat4 c = glm::mat4(x2, y2, z2, t);
	return c * glm::translate(-eye);
	

}

const glm::mat4x4& Camera::GetProjectionTransformation() const
{
	return projection_transformation;
}

const glm::mat4x4& Camera::GetViewTransformation() const
{
	return view_transformation;
}