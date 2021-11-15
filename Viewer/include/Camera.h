#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
public:
	Camera();
	virtual ~Camera();

	void SetCameraLookAt(const glm::vec3& eye, const glm::vec3& at, const glm::vec3& up);

	const glm::mat4x4& GetProjectionTransformation() const;
	const glm::mat4x4& GetViewTransformation() const;

private:
	glm::mat4x4 view_transformation;
	glm::mat4x4 projection_transformation;

	glm::vec3 camPos;
	glm::vec3 atPos;

	float up;
	float down;
	float left;
	float right;
	float nearZ;
	float farZ;
	//float whereEverYouAre;
};
