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
	void UpdateTranslationWorld(const glm::vec3& vec);
	void UpdateTranslationModel(const glm::vec3& vec);
	void UpdateScalingWorld(const glm::vec3& vec);
	void UpdateScalingModel(const glm::vec3& vec);
	const std::vector<float> GetUpDownVals();
	const std::vector<float> GetLeftRightVals();
	const std::vector<float> GetNearFarVals() const;
	const std::vector<glm::vec3> GetCameraLookAt() const;
	const std::vector<glm::mat4>& GetCurrentRotations() const;
	void UpdateProjType(bool type);
	const bool GetProjType() const;
	const std::string& GetCamName() const;
	void SetWindowSize(const int gwindow_width, const int gwindow_height);
	static glm::vec4 Vec3ToHomogeneousVec(const glm::vec3& vec);
	static glm::vec3 HomogeneousVecToVec3(const glm::vec4& vec);
	glm::vec3 GetTransformedVertex(const glm::vec3 vec) const;
	glm::vec3 GetTransformedLight(const glm::vec3 vec) const;
	float GetTransformedZ(const float z) const;

	bool gray;

private:
	glm::mat4x4 view_transformation;
	glm::mat4x4 projection_transformation;
	std::vector<glm::mat4> currentTranslationMat;
	std::vector<glm::mat4> currentRotationMat; //[0] world - [1] - camera itself
	std::vector<glm::mat4> currentScalingMat;

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
