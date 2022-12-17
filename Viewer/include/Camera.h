#pragma once
#include <glm/glm.hpp>

class Camera
{
public:
	Camera();
	virtual ~Camera();

	void SetCameraLookAt(const glm::vec3& eye, const glm::vec3& at, const glm::vec3& up);

	const glm::mat4x4& GetProjectionTransformation() const;
	const glm::mat4x4& GetViewTransformation() const;

	float left = -1280.0f;
	float right = 1280.0f;
	float up = 720.0f;
	float down = -720.0f;

	float translatex = 0.0f;
	float translatey = 0.0f;
	float rotate = 0.0f;

	float Wtranslatex = 0.0f;
	float Wtranslatey = 0.0f;
	float Wrotate = 0.0f;

private:
	glm::mat4x4 view_transformation;
	glm::mat4x4 projection_transformation;
};
