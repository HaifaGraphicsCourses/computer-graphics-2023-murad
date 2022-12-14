#include "MeshModel.h"
#include <iostream>
using namespace std;
#include <glm/gtx/transform.hpp>
#define _USE_MATH_DEFINES
#include <cmath>
#include <math.h>



MeshModel::MeshModel(std::vector<Face> faces, std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals, const std::string& model_name) :
	faces(faces),
	vertices(vertices),
	normals(normals)
{

	/*for (int i = 0; i < faces.size(); i++)
	{
		auto face = GetFace(i);
		cout << "face " << i << ": ";
		cout << face.GetVertexIndex(0) << ":(" << GetVertex(face.GetVertexIndex(0) - 1).x << ", " << GetVertex(face.GetVertexIndex(0) - 1).y << ", "
			<< GetVertex(face.GetVertexIndex(0) - 1).z << ") ";
		cout << face.GetVertexIndex(1) << ":(" << GetVertex(face.GetVertexIndex(1) - 1).x << ", " << GetVertex(face.GetVertexIndex(1) - 1).y << ", "
			<< GetVertex(face.GetVertexIndex(1) - 1).z << ") ";;
		cout << face.GetVertexIndex(2) << ":(" << GetVertex(face.GetVertexIndex(2) - 1).x << ", " << GetVertex(face.GetVertexIndex(2) - 1).y << ", "
			<< GetVertex(face.GetVertexIndex(2) - 1).z << ")" << endl;

	}*/
}

MeshModel::~MeshModel()
{
}

const Face& MeshModel::GetFace(int index) const
{
	return faces[index];
}
const glm::vec4 MeshModel::GetVertex(int index) const {
	float max = getMax();
	float s = screen / 2 / max;
	glm::vec3 ret;
	for (int i = 0; i < 3; i++)
	{
		ret.x = (vertices[index].x) * s;
		ret.y = (vertices[index].y) * s;
		ret.z = (vertices[index].z) * s;
	}
	return glm::vec4(ret, 1.0f);
}

const float MeshModel::getMax() const
{
	float m = 0.0f;
	for (int i = 0; i < vertices.size(); i++)
	{

		m = std::max(m, vertices[i].x);
		m = std::max(m, vertices[i].y);
	}
	return m;
}

const float MeshModel::getMin() const
{
	float m = 1000.0f;
	for (int i = 0; i < vertices.size(); i++)
	{

		m = std::min(m, vertices[i].x);
		m = std::min(m, vertices[i].y);
	}
	return m;
}

int MeshModel::GetFacesCount() const
{
	return faces.size();
}

const std::string& MeshModel::GetModelName() const
{
	return model_name;
}

const glm::mat4 MeshModel::GetTransformation() const
{

	glm::mat4x4 scale(scalex, 0, 0, 0, 0, scaley, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
    glm::mat4 translate = glm::translate(glm::mat4(1.0f), glm::vec3(translatex, translatey, 0));
	float o = -rotate * M_PI / 180;
	glm::mat4x4 rotate(cos(o), -sin(o), 0, 0, sin(o), cos(o), 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);

	glm::mat4x4 wscale(Wscalex, 0, 0, 0, 0, Wscaley, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
    glm::mat4 wtranslate = glm::translate(glm::mat4(1.0f), glm::vec3(Wtranslatex, Wtranslatey, 0));
	float w = -Wrotate * M_PI / 180;
	glm::mat4x4 wrotate(cos(w), -sin(w), 0, 0, sin(w), cos(w), 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);

	return (wtranslate * wrotate * wscale * translate * rotate * scale);
}
