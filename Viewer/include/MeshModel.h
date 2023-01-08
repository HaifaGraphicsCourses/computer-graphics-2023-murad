#pragma once
#include <glm/glm.hpp>
#include <string>
#include "Face.h"
#include "Camera.h"

class MeshModel
{
public:
	MeshModel(std::vector<Face> faces, std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals, const std::string& model_name);
	virtual ~MeshModel();
	const Face& GetFace(int index) const;
	int GetFacesCount() const;
	const std::string& GetModelName() const;
	const glm::mat4 GetTransformation() const;
	const glm::mat4 GetWorld() const;
	const glm::vec4 GetVertex(int index) const;
	const float getMax() const;
	const float getMin() const;
	const glm::vec4 getNormal(int i) const;
	const float getMaxNormal() const;


	float scalex = 1.0f;
	float scaley = 1.0f;
	float translatex = 0.0f;
	float translatey = 0.0f;
	float rotate = 0.0f;
	float Wscalex = 1.0f;
	float Wscaley = 1.0f;
	float Wtranslatex = 0.0f;
	float Wtranslatey = 0.0f;
	float Wtranslatez = 0.0f;
	float Wrotate = 0.0f;

	int screen;
private:
	std::vector<Face> faces;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::string model_name;
};

