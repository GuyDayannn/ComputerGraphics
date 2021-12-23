#include "Scene.h"
#include "MeshModel.h"
#include <string>

Scene::Scene() :
	active_camera_index(0),
	active_model_index(0),
	active_light_index(0)
{
	showLights = true;
	fog = false;
	fogDistance = 2.0f;
	fogDensity = 1.0f;
	blur = false;
	numOfBlurs = 1;
}

void Scene::AddModel(const std::shared_ptr<MeshModel>& mesh_model)
{
	mesh_models.push_back(mesh_model);
}

int Scene::GetModelCount() const
{
	return mesh_models.size();
}

MeshModel& Scene::GetModel(int index) const
{
	return *mesh_models[index];
}

MeshModel& Scene::GetActiveModel() const
{
	return *mesh_models[active_model_index];
}

void Scene::AddCamera(const std::shared_ptr<Camera>& camera)
{
	cameras.push_back(camera);
}

int Scene::GetCameraCount() const
{
	return cameras.size();
}

Camera& Scene::GetCamera(int index) const
{
	return *cameras[index];
}

Camera& Scene::GetActiveCamera()
{
	return *cameras[active_camera_index];
}

void Scene::AddLight(const std::shared_ptr<LightSource>& light_source)
{
	lights.push_back(light_source);
}

int Scene::GetLightCount() const
{
	return lights.size();
}

LightSource& Scene::GetLight(int index) const
{
	return *lights[index];
}

LightSource& Scene::GetActiveLight() const
{
	return *lights[active_light_index];
}


void Scene::SetActiveCameraIndex(int index)
{
	active_camera_index = index;
}

int Scene::GetActiveCameraIndex() const
{
	return active_camera_index;
}

void Scene::SetActiveModelIndex(int index)
{
	active_model_index = index;
}

int Scene::GetActiveModelIndex() const
{
	return active_model_index;
}

void Scene::SetActiveLightIndex(int index)
{
	active_light_index = index;
}

int Scene::GetActiveLightIndex() const
{
	return active_light_index;
}

void Scene::ShowLights()
{
	showLights = true;
}

void Scene::HideLights()
{
	showLights = false;
}

const bool Scene::GetShowLights() const
{
	return showLights;
}

const bool Scene::IsFog() const
{
	return fog;
}

void Scene::UpdateFogStatus(bool s)
{
	fog = s;
}

void Scene::UpdateFogDistance(float d)
{
	fogDistance = d;
}

const float Scene::GetFogDistance() const
{
	return fogDistance;
}

void Scene::UpdateFogDensity(float de)
{
	fogDensity = de;
}

const float Scene::GetFogDensity() const
{
	return fogDensity;
}

const bool Scene::IsBlur() const
{
	return blur;
}

void Scene::UpdateBlur(bool b)
{
	blur = b;
}

const int Scene::GetNumOfBlurs() const
{
	return numOfBlurs;
}

void Scene::UpdateNumOfBlurs(int n)
{
	numOfBlurs = n;
}