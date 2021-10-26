#include "MeshModel.h"

MeshModel::MeshModel(std::vector<Face> faces, std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals, const std::string& model_name) :
	faces(faces),
	vertices(vertices),
	normals(normals)
{
	scaling.push_back(glm::vec3(1.0f, 1.0f, 1.0f)); //[0] - world
	scaling.push_back(glm::vec3(1.0f, 1.0f, 1.0f)); //[1] - model
	rotation.push_back(glm::vec3(0.0f, 0.0f, 0.0f)); // [i] = {xrotate, yrotate, zrotate}
	rotation.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
	translation.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
	translation.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
}

MeshModel::~MeshModel()
{
}

const Face& MeshModel::GetFace(int index) const
{
	return faces[index];
}

int MeshModel::GetFacesCount() const
{
	return faces.size();
}

const std::string& MeshModel::GetModelName() const
{
	return model_name;
}

//gets vertex on index
const glm::vec3& MeshModel::GetPureVertex(int index) const
{
	return vertices[index - 1];
}

//converting vec3 to Homogeneous Vector
glm::vec4 MeshModel::Vec3ToHomogeneousVec(const glm::vec3& vec)
{
	return glm::vec4(vec.x, vec.y, vec.z, 1.0f);
}

//converting Homogeneous Vector to vec3
glm::vec3 MeshModel::HomogeneousVecToVec3(const glm::vec4& vec)
{
	if(vec.w == 0)
		return glm::vec3(vec.x, vec.y, vec.z);
	return glm::vec3(vec.x / vec.w, vec.y / vec.w, vec.z / vec.w);
}

void MeshModel::FitToWindow(int viewport_width, int viewport_height)
{
	float maxX = vertices[0].x;
	float maxY = vertices[0].y;
	float minX = vertices[0].x;
	float minY = vertices[0].y;

	for (int i = 0; i < vertices.size(); i++) //finding maxX and maxY on vertices
	{
		if (vertices[i].x > maxX)
			maxX = vertices[i].x;

		if (vertices[i].y > maxY)
			maxY = vertices[i].y;

		if (vertices[i].x < minX)
			minX = vertices[i].x;

		if (vertices[i].y < minY)
			minY = vertices[i].y;
	}

	translation[1] = glm::vec3(viewport_width / 2.0f - (maxX - minX) / 2, viewport_height / 2.0f - (maxY - minY) / 2, translation[1].z);
}

void MeshModel::UpdateWorldTransformations(const glm::vec3& scale, const glm::vec3& rotate, const glm::vec3& translate)
{
	scaling[0] = scale;
	rotation[0] = rotate;
	translation[0] = translate;
}

void MeshModel::UpdateModelTransformations(const glm::vec3& scale, const glm::vec3& rotate, const glm::vec3& translate)
{
	scaling[1] = scale;
	rotation[1] = rotate;
	translation[1] = translate;
}

const std::vector<glm::mat4> MeshModel::GetScalingMatrices() const
{
	std::vector<glm::mat4> scalingMats(2);
	for (int i = 0; i < scalingMats.size(); i++)
	{
		scalingMats[i] = glm::mat4
		(scaling[i].x, 0.0f, 0.0f, 0.0f,
		0.0f, scaling[i].y, 0.0f, 0.0f,
		0.0f, 0.0f, scaling[i].z, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);

	}

	return scalingMats;
}

const std::vector<glm::mat4> MeshModel::GetTranslationMatrices() const
{
	std::vector<glm::mat4> translationMats(2);
	for (int i = 0; i < translationMats.size(); i++)
	{
		translationMats[i] = glm::mat4
		(1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		translation[i].x, translation[i].y, translation[i].z, 1.0f);
	}
	//translationMats seems inverse because when inputed like that f.e first row is actually first collum - mat4 = [vec4,vec4,vec4,vec4]
	//so the 4th vec4 is actually the collum with the translation variables

	return translationMats;
}

const std::vector<glm::mat4> MeshModel::GetRotationMatrices(const std::string axis, const int grid) const
{
	std::vector<glm::mat4> rotationMats(2);

	if (axis == "z")
	{
		float zRad = glm::radians(rotation[grid].z);
		float cosRad = cosf(glm::radians(zRad));
		float sinRad = sinf(glm::radians(zRad));
		rotationMats[grid] = glm::mat4
		(cosRad, -sinRad, 0.0f, 0.0f,
		sinRad, cosRad, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
	}
	else if (axis == "y")
	{
		float yRad = glm::radians(rotation[grid].y);
		float cosRad = cosf(glm::radians(yRad));
		float sinRad = sinf(glm::radians(yRad));
		rotationMats[grid] = glm::mat4
		(cosRad, 0.0f, -sinRad, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		sinRad, 0.0f, cosRad, 1.0f);
	}
	else
	{
		float xRad = glm::radians(rotation[grid].x);
		float cosRad = cosf(glm::radians(xRad));
		float sinRad = sinf(glm::radians(xRad));
		rotationMats[grid] = glm::mat4
		(1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, cosRad, -sinRad, 0.0f,
		0.0f, sinRad, cosRad, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
	}

	if (grid != 1) //second matrix is identity
		rotationMats[1] = glm::mat4(1.0f);
	else
		rotationMats[0] = glm::mat4(1.0f);

	return rotationMats;
}

const glm::vec3& MeshModel::GetTransformedVertex(int index) const
{
	std::vector<glm::mat4> scalingMats = GetScalingMatrices();
	std::vector<glm::mat4> rotationMats = GetRotationMatrices();
	std::vector<glm::mat4> translationMats = GetTranslationMatrices();
	glm::mat4 worldTransformationMat = scalingMats[0] * rotationMats[0] * translationMats[0];
	glm::mat4 modelTransformationMat = scalingMats[1] * rotationMats[1] * translationMats[1];
	
	return HomogeneousVecToVec3(worldTransformationMat * modelTransformationMat * Vec3ToHomogeneousVec(vertices[index - 1]));

}