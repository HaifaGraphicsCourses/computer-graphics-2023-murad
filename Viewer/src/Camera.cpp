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


glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraTarget = glm::vec3(1.0f, 0.0f, -1.0f);
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
	return glm::ortho(left, right, down, up, 0.1f, 100.0f);
}

const glm::mat4x4& Camera::GetViewTransformation() const
{
	return view;
}
