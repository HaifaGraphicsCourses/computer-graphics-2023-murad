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

	for (int i = 0; i < faces.size(); i++)
	{
		auto face = GetFace(i);
		cout << "face " << i << ": ";
		cout << face.GetVertexIndex(0) << ":(" << GetVertex(face.GetVertexIndex(0) - 1).x << ", " << GetVertex(face.GetVertexIndex(0) - 1).y << ", "
			<< GetVertex(face.GetVertexIndex(0) - 1).z << ") ";
		cout << face.GetVertexIndex(1) << ":(" << GetVertex(face.GetVertexIndex(1) - 1).x << ", " << GetVertex(face.GetVertexIndex(1) - 1).y << ", "
			<< GetVertex(face.GetVertexIndex(1) - 1).z << ") ";;
		cout << face.GetVertexIndex(2) << ":(" << GetVertex(face.GetVertexIndex(2) - 1).x << ", " << GetVertex(face.GetVertexIndex(2) - 1).y << ", "
			<< GetVertex(face.GetVertexIndex(2) - 1).z << ")" << endl;

	}
}

MeshModel::~MeshModel()
{
}

const Face& MeshModel::GetFace(int index) const
{
	return faces[index];
}
const glm::vec4 MeshModel::GetVertex(int index) const {
	return glm::vec4(vertices[index], 1.0f);
}

const float MeshModel::getMax() const
{
	float m = 0.0;
	for (int i = 0; i < vertices.size(); i++)
	{

		m = std::max(m, vertices[i].x);
		m = std::max(m, vertices[i].y);
	}
	return m;
}

glm::mat4x4 MeshModel::scale()
{
	float max = getMax();
	float s = 200.0 / max;
	return glm::mat4x4(scalex * s,0, 0, 0, 0, scaley * s, 0, 0, 0, 0, s , 0, 0, 0, 0, 1);
}



glm::mat4x4 MeshModel::translate()
{
	return glm::translate(glm::mat4(1.0f), glm::vec3(translatex, translatey, 0));
}

glm::mat4x4 MeshModel::localrotate()
{
	float o = -rotate * M_PI / 180;
	return glm::mat4x4(cos(o),-sin(o),0,0,sin(o),cos(o),0,0,0,0,1,0,0,0,0,1);
}

glm::mat4x4 MeshModel::Wscale()
{
	return glm::mat4x4(Wscalex, 0, 0, 0, 0, Wscaley, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
}

glm::mat4x4 MeshModel::Wtranslate()
{
	return glm::translate(glm::mat4(1.0f), glm::vec3(Wtranslatex + 640, Wtranslatey + 340, 0));
}

glm::mat4x4 MeshModel::Wlocalrotate()
{
	float o = -Wrotate * M_PI / 180;
	return glm::mat4x4(cos(o), -sin(o), 0, 0, sin(o), cos(o), 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
}


int MeshModel::GetFacesCount() const
{
	return faces.size();
}

const std::string& MeshModel::GetModelName() const
{
	return model_name;
}