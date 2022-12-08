#include "Camera.h"
#include <cmath>
#include <imgui/imgui.h>
#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <nfd.h>
#include "Renderer.h"
#include "Scene.h"

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
void  Camera::SetViewTransformation(const  glm::mat4x4&  view_transformation)
{
 this->projection_transformation= projection_transformation;
}
const glm::mat4x4& Camera::GetProjectionTransformation() const
{
	return projection_transformation;
}

const glm::mat4x4& Camera::GetViewTransformation() const
{
	return view_transformation;
}