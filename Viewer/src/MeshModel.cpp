#include "MeshModel.h"
#include <iostream>
using namespace std;
#include <glm/gtx/transform.hpp>

MeshModel::MeshModel(std::vector<Face> faces, std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals, const std::string& model_name) :
	faces(faces),
	vertices(vertices),
	normals(normals)
{

		/*auto faceCount = GetFacesCount();

		for (int i = 0; i < faceCount; i++)
		{
			auto face = GetFace(i);
			cout << "face " << i << ": ";
			cout << face.GetVertexIndex(0) << ":(" << GetVertex(face.GetVertexIndex(0)-1).x << ", " << GetVertex(face.GetVertexIndex(0)-1).y << ", "
				<< GetVertex(face.GetVertexIndex(0)-1).z << ") ";
			cout << face.GetVertexIndex(1) << ":(" << GetVertex(face.GetVertexIndex(1)-1).x << ", " << GetVertex(face.GetVertexIndex(1)-1).y << ", "
				<< GetVertex(face.GetVertexIndex(1)-1).z << ") ";;
			cout << face.GetVertexIndex(2) << ":(" << GetVertex(face.GetVertexIndex(2)-1).x << ", " << GetVertex(face.GetVertexIndex(2)-1).y << ", "
				<< GetVertex(face.GetVertexIndex(2)-1).z << ")" << endl;
			//DrawLine();
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

glm::mat4x4 MeshModel::scale(float x, int y, int z)
{
	float max = getMax();
	float s = 300.0 / max;
	return glm::mat4x4(x * s,0, 0, 0, 0, y * s, 0, 0, 0, 0, z * s , 0, 0, 0, 0, 1);
}



glm::mat4x4 MeshModel::translate(int x, int y, int z)
{
	return glm::translate(glm::mat4(1.0f), glm::vec3(x + 640, y + 340, z + 0));
}

glm::mat4x4 MeshModel::scaletofit()
{
	float max = getMax();
	float s = 300.0 / max;
	return glm::mat4x4( s, 0, 0, 0, 0, s, 0, 0, 0, 0, s, 0, 0, 0, 0, 1);
}

glm::mat4x4 MeshModel::translatetofit()
{
	return glm::translate(glm::mat4(1.0f), glm::vec3(640, 340, 0));
}

void MeshModel::update(int x)
{
	scalex = x;
}

int MeshModel::GetFacesCount() const
{
	return faces.size();
}

const std::string& MeshModel::GetModelName() const
{
	return model_name;
}