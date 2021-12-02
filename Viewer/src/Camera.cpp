#include "Camera.h"

Camera::Camera(int window_width, int window_height, int num): window_width(window_width), window_height(window_height)
{
	camPos = glm::vec3(0.0f, 0.0f, 3.0f);
	atPos = glm::vec3(0.0f, 0.0f, 0.0f);
	upPos = glm::vec3(0.0f, 1.0f, 0.0f);
	nearZ = 0.1f;
	farZ = 100.0f;
	left = -(window_width / 2.0f);
	right = window_width / 2.0f;
	down = -(window_height / 2.0f);
	up = window_height / 2.0f;
	view_transformation = glm::lookAt(camPos, atPos, upPos);
	projType = true;
	fov = 100.0f;
	UpdateViewVolume(up, down, left, right, nearZ, farZ, fov);
	currentRotationMat.push_back(glm::mat4(1.0f));
	currentRotationMat.push_back(glm::mat4(1.0f));
	currentTranslationMat.push_back(glm::mat4(1.0f));
	currentTranslationMat.push_back(glm::mat4(1.0f));
	currentScalingMat.push_back(glm::mat4(1.0f));
	currentScalingMat.push_back(glm::mat4(1.0f));
	camName = "Camera " + std::to_string(num);
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

void Camera::UpdateViewVolume(float up, float down, float left, float right, float near, float far, float fovy)	
{
	this->up = up;
	this->down = down;
	this->left = left;
	this->right = right;
	this->nearZ = near;
	this->farZ = far;
	
	if(projType == false)
		projection_transformation = glm::ortho(left, right, down, up, nearZ, farZ);
	else
	{
		fov = fovy;
		float rFov = glm::radians(fov);
		projection_transformation = glm::perspective(rFov, (right - left) / (up - down), nearZ, farZ);
	}
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

const std::vector<glm::mat4>& Camera::GetCurrentRotations() const
{
	return currentRotationMat;
}

void Camera::UpdateRotationWorld(float degrees, std::string axis)
{
	glm::vec3 rotateAround;
	if (axis == "z") rotateAround = glm::vec3(0.0f, 0.0f, 1.0f);
	else if (axis == "y") rotateAround = glm::vec3(0.0f, 1.0f, 0.0f);
	else rotateAround = glm::vec3(1.0f, 0.0f, 0.0f);

	currentRotationMat[0] = glm::rotate(currentRotationMat[0], glm::radians(degrees), rotateAround);
}

void Camera::UpdateRotationModel(float degrees, std::string axis)
{
	glm::vec3 rotateAround;
	if (axis == "z") rotateAround = glm::vec3(0.0f, 0.0f, 1.0f);
	else if (axis == "y") rotateAround = glm::vec3(0.0f, 1.0f, 0.0f);
	else rotateAround = glm::vec3(1.0f, 0.0f, 0.0f);

	currentRotationMat[1] = glm::rotate(currentRotationMat[1], glm::radians(degrees), rotateAround);
}

void Camera::UpdateTranslationWorld(const glm::vec3& vec)
{
	currentTranslationMat[0] = glm::translate(glm::mat4(1.0f), vec);
}

void Camera::UpdateTranslationModel(const glm::vec3& vec)
{
	currentTranslationMat[1] = glm::translate(glm::mat4(1.0f), vec);
}

void Camera::UpdateScalingWorld(const glm::vec3& vec)
{
	currentScalingMat[0] = glm::scale(glm::mat4(1.0f), vec);
}

void Camera::UpdateScalingModel(const glm::vec3& vec)
{
	currentScalingMat[1] = glm::scale(glm::mat4(1.0f), vec);
}


void Camera::UpdateProjType(bool type)
{
	projType = type;

	if (projType == false)
		projection_transformation = glm::ortho(left, right, down, up, nearZ, farZ);
	else
	{
		float rFov = glm::radians(fov);
		projection_transformation = glm::perspective(rFov, (right - left) / (up - down), nearZ, farZ);
	}
}

const bool Camera::GetProjType() const
{
	return projType;
}
const std::string& Camera::GetCamName() const
{
	return camName;
}

void Camera::SetWindowSize(const int gwindow_width, const int gwindow_height)
{
	if (projType == true) //Perspective
	{
		window_width = gwindow_width;
		window_height = gwindow_height;
		projection_transformation = glm::perspective(fov, window_width / window_height, nearZ, farZ);
	}
}

glm::vec4 Camera::Vec3ToHomogeneousVec(const glm::vec3& vec)
{
	return glm::vec4(vec.x, vec.y, vec.z, 1.0f);
}

//converting Homogeneous Vector to vec3
glm::vec3 Camera::HomogeneousVecToVec3(const glm::vec4& vec)
{
	if (vec.w == 0)
		return glm::vec3(vec.x, vec.y, vec.z);
	return glm::vec3(vec.x / vec.w, vec.y / vec.w, vec.z / vec.w);
}

glm::vec3 Camera::GetTransformedVertex(const glm::vec3 vec) const
{
	glm::vec3 tVec = HomogeneousVecToVec3(projection_transformation * glm::inverse(currentTranslationMat[0] * currentRotationMat[0] * currentScalingMat[0] * currentTranslationMat[1] * currentRotationMat[1] * currentScalingMat[1]) * view_transformation * Vec3ToHomogeneousVec(vec));
	tVec.x = (1.0f + tVec.x) * (window_width / 2.0f);
	tVec.y = (1.0f + tVec.y) * (window_height / 2.0f);
	tVec.z = (1.0f + tVec.z) * (abs(farZ - nearZ));
	return tVec;
	//return HomogeneousVecToVec3(projection_transformation * glm::inverse(currentTranslationMat[0] * currentRotationMat[0] * currentScalingMat[0] * currentTranslationMat[1] * currentRotationMat[1] * currentScalingMat[1]) * view_transformation * Vec3ToHomogeneousVec(vec));
}