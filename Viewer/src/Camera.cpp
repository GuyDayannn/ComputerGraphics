#include "Camera.h"

Camera::Camera()
{
	float viewport_width = 1280.0f;
	float viewport_height = 720.0f;
	camPos = glm::vec3(0.0f, 0.0f, 1.0f);
	atPos = glm::vec3(0.0f, 0.0f, 0.0f);
	upPos = glm::vec3(0.0f, 1.0f, 0.0f);
	nearZ = 30.0f;
	farZ = 500.0f;
	left = -(viewport_width / 2.0f);
	right = viewport_width / 2.0f;
	down = -(viewport_height / 2.0f);
	up = viewport_height / 2.0f;
	view_transformation = glm::lookAt(camPos, atPos, upPos);
	projection_transformation = glm::ortho(left, right, down, up, nearZ, farZ);
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
	upPos = up;

	view_transformation = glm::lookAt(eye, at, up);
}

void Camera::UpdateViewVolume(float up, float down, float left, float right, float near, float far)	
{
	this->up = up;
	this->down = down;
	this->left = left;
	this->right = right;
	this->nearZ = near;
	this->farZ = far;
	projection_transformation = glm::ortho(left, right, down, up, nearZ, farZ);
}

const std::vector<float> Camera::GetUpDownVals()
{
	std::vector<float> upDown;
	upDown.push_back(up);
	upDown.push_back(down);
	return upDown;
}

const std::vector<float> Camera::GetLeftRightVals()
{
	std::vector<float> leftRight;
	leftRight.push_back(left);
	leftRight.push_back(right);
	return leftRight;
}

const std::vector<float> Camera::GetNearFarVals()
{
	std::vector<float> nearFar;
	nearFar.push_back(nearZ);
	nearFar.push_back(farZ);
	return nearFar;
}

const std::vector<glm::vec3> Camera::GetCameraLookAt() const
{
	std::vector<glm::vec3> cameraLookAt;
	cameraLookAt.push_back(camPos);
	cameraLookAt.push_back(atPos);
	cameraLookAt.push_back(upPos);
	return cameraLookAt;

}