#pragma once

#include <vector>
#include <memory>

#include "Camera.h"
#include "MeshModel.h"

using namespace std;

class Scene {
public:
	Scene();

	void AddModel(const shared_ptr<MeshModel>& mesh_model);
	int GetModelCount() const;
	MeshModel& GetModel(int index) const;
	MeshModel& GetActiveModel() const;
	
	void AddCamera(const shared_ptr<Camera>& camera);
	int GetCameraCount() const;
	Camera& GetCamera(int index) const;
	Camera& GetActiveCamera();

	void SetActiveCameraIndex(int index);
	int GetActiveCameraIndex() const;

	void SetActiveModelIndex(int index);
	int GetActiveModelIndex() const;

	bool bounding = false;
	bool axis = false;
	bool fillTriangle = false;
	bool grey_scale = false;
	bool normals = false;
	bool ambient = false;

	float lightx = 690.0f;
	float lighty = 360.0f;
	float lightz = 5.0f;
	float intensity = 1.0f;
	glm::vec3 light = glm::vec3(1.0f, 1.0f, 1.0f);

	
private:
	vector<shared_ptr<MeshModel>> mesh_models;
	vector<shared_ptr<Camera>> cameras;

	int active_camera_index;
	int active_model_index;
};