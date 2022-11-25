#pragma once
#include <glm/glm.hpp>
#include <string>
#include "Face.h"

class MeshModel
{
public:
	MeshModel(std::vector<Face> faces, std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals, const std::string& model_name);
	virtual ~MeshModel();
	const Face& GetFace(int index) const;
	int GetFacesCount() const;
	const std::string& GetModelName() const;
	const glm::vec4 GetVertex(int index) const;
	const float getMax() const;
	glm::mat4x4 scale(float x = 1, int y = 1, int z = 1);
	glm::mat4x4 translate(int x = 0, int y = 0, int z = 0);
	glm::mat4x4 scaletofit();
	glm::mat4x4 translatetofit();
	void update(int x);
private:
	std::vector<Face> faces;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::string model_name;
	int scalex = 1;
};
