#include "MeshModel.h"
#include <iostream>
using namespace std;

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
const glm::vec3 MeshModel::GetVertex(int index) const {
	return vertices[index];
}

int MeshModel::GetFacesCount() const
{
	return faces.size();
}

const std::string& MeshModel::GetModelName() const
{
	return model_name;
}