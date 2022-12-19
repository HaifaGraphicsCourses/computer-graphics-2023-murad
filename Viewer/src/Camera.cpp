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
#include <glm/glm.hpp>
#define _USE_MATH_DEFINES
#include <math.h>
#include <cmath>




//glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
//glm::vec3 cameraTarget = glm::vec3(1.0f, 0.0f, -1.0f);
//glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
//glm::mat4 view = glm::lookAt(cameraPos, cameraTarget + cameraPos, up);
glm::vec3 cameraPos = glm::vec3(3.0f, 3.0f, 3.0f);
glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
glm::mat4 view = glm::lookAt(cameraPos, cameraTarget + cameraPos, up);
Camera::Camera()
{
	
}

Camera::~Camera()
{
	
}

void Camera::SetCameraLookAt(const glm::vec3& eye, const glm::vec3& at, const glm::vec3& up)
{
	
}


const glm::mat4x4& Camera::GetProjectionTransformation() const
{
	//return glm::ortho(left, right, down, up, 0.1f, 100.0f);
	return glm::frustum(left, right, down, up, near2, far2);
}

const glm::mat4x4& Camera::GetViewTransformation() const
{
	glm::mat4 translate = glm::translate(glm::mat4(1.0f), glm::vec3(translatex, translatey, translatez));
	float o = -rotate * M_PI / 180;
	glm::mat4x4 rotate(cos(o), -sin(o), 0, 0, sin(o), cos(o), 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);

	glm::mat4 wtranslate = glm::translate(glm::mat4(1.0f), glm::vec3(Wtranslatex, Wtranslatey, Wtranslatez));
	float w = -Wrotate * M_PI / 180;
	glm::mat4x4 wrotate(cos(w), -sin(w), 0, 0, sin(w), cos(w), 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);

	return glm::inverse(wtranslate * wrotate * translate * rotate);
}

