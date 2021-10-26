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

const glm::vec3& MeshModel::GetTransformedVertex(int index) const
{
	return vertices[index - 1];
}