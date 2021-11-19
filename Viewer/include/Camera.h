#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <string>

class Camera
{
public:
	Camera(int window_width = 1280, int window_height = 720, int num=0);
	virtual ~Camera();

	void SetCameraLookAt(const glm::vec3& eye, const glm::vec3& at, const glm::vec3& up);

	const glm::mat4x4& GetProjectionTransformation() const;
	const glm::mat4x4& GetViewTransformation() const;

	void UpdateViewVolume(float up, float down, float left, float right, float near, float far, float fovy);
	void UpdateRotationWorld(float degrees, std::string axis);
	void UpdateRotationModel(float degrees, std::string axis);
	const std::vector<float> GetUpDownVals();
	const std::vector<float> GetLeftRightVals();
	const std::vector<float> GetNearFarVals();
	const std::vector<glm::vec3> GetCameraLookAt() const;
	const std::vector<glm::mat4>& GetCurrentRotations() const;
	void UpdateProjType(bool type);
	const bool GetProjType() const;
	const std::string& GetCamName() const;
	void SetWindowSize(const int gwindow_width, const int gwindow_height);

private:
	glm::mat4x4 view_transformation;
	glm::mat4x4 projection_transformation;
	std::vector<glm::mat4> currentRotationMat; //[0] world - [1] - camera itself

	glm::vec3 camPos;
	glm::vec3 atPos;
	glm::vec3 upPos;

	std::string camName;

	float window_width;
	float window_height;

	bool projType; //false - orth, true - perspec
	float up;
	float down;
	float left;
	float right;
	float nearZ;
	float farZ;
	//float whereEverYouAre;
	float fov;
};
