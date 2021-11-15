#include "Camera.h"

Camera::Camera()
{
	camPos = glm::vec3(0.0f, 0.0f, 1.0f);
	atPos = glm::vec3(0.0f, 0.0f, 0.0f);
	nearZ = 30.0f;
	farZ = 500.0f;
	left = 0.0f;
	right = 1280.0f;
	down = 0.0f;
	up = 720.0f;
	view_transformation = glm::lookAt(camPos, atPos, glm::vec3(0.0f, 1.0f, 0.0f));
	projection_transformation = glm::ortho(-(1280.0f / 2.0f), 1280.0f / 2.0f, -(720.0f / 2.0f), 720.0f / 2.0f, nearZ, farZ);
}

Camera::~Camera()
{
	
	
}

const glm::mat4x4& Camera::GetProjectionTransformation() const
{
	return projection_transformation;
}

const glm::mat4x4& Camera::GetViewTransformation() const
{
	return view_transformation;
}

void Camera::SetCameraLookAt(const glm::vec3& eye, const glm::vec3& at, const glm::vec3& up)
{
	camPos = eye;
	atPos = at;

	view_transformation = glm::lookAt(eye, at, up);
	projection_transformation = glm::ortho(-(1280.0f / 2.0f), 1280.0f / 2.0f, -(720.0f / 2.0f), 720.0f / 2.0f, nearZ, farZ);
}