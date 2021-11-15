#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

class Camera
{
public:
	Camera();
	virtual ~Camera();

	void SetCameraLookAt(const glm::vec3& eye, const glm::vec3& at, const glm::vec3& up);

	const glm::mat4x4& GetProjectionTransformation() const;
	const glm::mat4x4& GetViewTransformation() const;

	void UpdateViewVolume(float up, float down, float left, float right, float near, float far);
	const std::vector<float> GetUpDownVals();
	const std::vector<float> GetLeftRightVals();
	const std::vector<float> GetNearFarVals();
	const std::vector<glm::vec3> GetCameraLookAt() const;

private:
	glm::mat4x4 view_transformation;
	glm::mat4x4 projection_transformation;

	glm::vec3 camPos;
	glm::vec3 atPos;
	glm::vec3 upPos;

	float up;
	float down;
	float left;
	float right;
	float nearZ;
	float farZ;
	//float whereEverYouAre;
};
