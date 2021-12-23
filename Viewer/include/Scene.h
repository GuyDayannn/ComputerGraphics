#pragma once

#include <vector>
#include <memory>

#include "Camera.h"
#include "MeshModel.h"
#include "LightSource.h"

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

	void AddLight(const shared_ptr<LightSource>& light_source);
	int GetLightCount() const;
	LightSource& GetLight(int index) const;
	LightSource& GetActiveLight() const;

	void SetActiveCameraIndex(int index);
	int GetActiveCameraIndex() const;

	void SetActiveModelIndex(int index);
	int GetActiveModelIndex() const;

	void SetActiveLightIndex(int index);
	int GetActiveLightIndex() const;

	void ShowLights();
	void HideLights();
	const bool GetShowLights() const;

	const bool IsFog() const;
	void UpdateFogStatus(bool s);
	void UpdateFogDistance(float d);
	const float GetFogDistance() const;
	void UpdateFogDensity(float de);
	const float GetFogDensity() const;

	const bool IsBlur() const;
	void UpdateBlur(bool b);

	const int GetNumOfBlurs() const;
	void UpdateNumOfBlurs(int n);
	
private:
	vector<shared_ptr<MeshModel>> mesh_models;
	vector<shared_ptr<Camera>> cameras;
	vector<shared_ptr<LightSource>> lights;

	int active_camera_index;
	int active_model_index;
	int active_light_index;
	bool showLights;
	bool fog;
	float fogDistance;
	bool blur;
	int numOfBlurs;
	float fogDensity;
	bool bloom;
};