#include "MeshModel.h"

MeshModel::MeshModel(std::vector<Face> faces, std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals, const std::string& model_name) :
	faces(faces),
	vertices(vertices),
	normals(normals),
	model_name(model_name)
{
	scaling.push_back(glm::vec3(1.0f, 1.0f, 1.0f)); //[0] - world
	scaling.push_back(glm::vec3(150.0f, 150.0f, 150.0f)); //[1] - model
	rotation.push_back(glm::vec3(0.0f, 0.0f, 0.0f)); // [i] = {xrotate, yrotate, zrotate}
	rotation.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
	translation.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
	translation.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
	currentRotationMat.push_back(glm::mat4(1.0f));
	currentRotationMat.push_back(glm::mat4(1.0f));
	
	std::vector<glm::vec3> xAxis;
	xAxis.push_back(glm::vec3(-200.0f, 0.0f, 0.0f));
	xAxis.push_back(glm::vec3(200.0f, 0.0f, 0.0f));
	std::vector<glm::vec3> yAxis;
	yAxis.push_back(glm::vec3(0.0f, -200.0f, 0.0f));
	yAxis.push_back(glm::vec3(0.0f, 200.0f, 0.0f));
	std::vector<glm::vec3> zAxis;
	zAxis.push_back(glm::vec3(0.0f, 0.0f, -200.0f));
	zAxis.push_back(glm::vec3(0.0f, 0.0f, 200.0f));
	modelAxis.push_back(xAxis);
	modelAxis.push_back(yAxis);
	modelAxis.push_back(zAxis);
	modelAxisModel = modelAxis;
	for (int i = 0; i < modelAxisModel.size(); i++)
	{
		modelAxisModel[i][0] /= 200.0f;
		modelAxisModel[i][1] /= 200.0f;

	}
	showAxisWorld = false;
	showAxisModel = false;
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

/**
* just moving so model will be on middle by that formula:
* (biggest_x_vertex - smallest_x_vertex) / 2 is the object "middle". kind of anywat...
* same for y
* then add to the translation matrix the 'command' to move it to the middle:
*				viewport_width / 2.0f - (maxX - minX) / 2
*				same for viewport_height
*/
/*
const std::vector<glm::vec3> MeshModel::FitToWindow(int viewport_width, int viewport_height) const
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
	float embigen = 1.0f / (maxY - minY) * (viewport_height / 2.0f);
	glm::vec3 fittingSizes(viewport_width / 2.0f - (embigen*(maxX + minX) / 2), viewport_height / 2.5f - (embigen*(maxY + minY) / 2), translation[1].z);
	glm::vec3 fittingScale(embigen, embigen, embigen);
	std::vector<glm::vec3> fittingMeasures;
	fittingMeasures.push_back(fittingScale);
	fittingMeasures.push_back(fittingSizes);

	return fittingMeasures;
	//translation[1] = glm::vec3(viewport_width / 2.0f - (maxX - minX) / 2, viewport_height / 2.0f - (maxY - minY) / 2, translation[1].z);
}
*/

//recieving worldTransformation request and updating scaling, roatation and translation
void MeshModel::UpdateWorldTransformations(const glm::vec3& scale, const glm::vec3& rotate, std::string axis, const glm::vec3& translate)
{
	scaling[0] = scale;
	rotation[0] = rotate;
	translation[0] = translate;

	currentRotationMat[0] = GetRotationMatrices(axis, 0)[0] * currentRotationMat[0]; //incrementally rotation
}

//same just for model
void MeshModel::UpdateModelTransformations(const glm::vec3& scale, const glm::vec3& rotate, std::string axis, const glm::vec3& translate)
{
	scaling[1] = scale;
	rotation[1] = rotate;
	translation[1] = translate;

	currentRotationMat[1] = GetRotationMatrices(axis, 1)[1] * currentRotationMat[1]; //incrementally rotation
}

//returning scaliing matrices for world and model with information in rotation vector
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


//same just for translation
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

//same for rotation
const std::vector<glm::mat4> MeshModel::GetRotationMatrices(const std::string axis, const int grid) const
{
	std::vector<glm::mat4> rotationMats(2);

	if (axis == "z")
	{
		float zRad = glm::radians(rotation[grid].z);
		float cosRad = cosf(zRad);
		float sinRad = sinf(zRad);
		rotationMats[grid] = glm::mat4
		(cosRad, sinRad, 0.0f, 0.0f,
		-sinRad, cosRad, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
	}
	else if (axis == "y")
	{
		float yRad = glm::radians(rotation[grid].y);
		float cosRad = cosf(yRad);
		float sinRad = sinf(yRad);
		rotationMats[grid] = glm::mat4
		(cosRad, 0.0f, -sinRad, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		sinRad, 0.0f, cosRad, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
	}
	else
	{
		float xRad = glm::radians(rotation[grid].x);
		float cosRad = cosf(xRad);
		float sinRad = sinf(xRad);
		rotationMats[grid] = glm::mat4
		(1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, cosRad, sinRad, 0.0f,
		0.0f, -sinRad, cosRad, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
	}

	if (grid != 1) //second matrix is identity - for now anyway
		rotationMats[1] = glm::mat4(1.0f);
	else
		rotationMats[0] = glm::mat4(1.0f);

	return rotationMats;
}

//returning vertex after being transformed by transformation matrices which includes world and model matrices
const glm::vec3& MeshModel::GetTransformedVertex(int index) const
{
	std::vector<glm::mat4> scalingMats = GetScalingMatrices();
	std::vector<glm::mat4> rotationMats = GetRotationMatrices();
	std::vector<glm::mat4> translationMats = GetTranslationMatrices();
	glm::mat4 worldTransformationMat = translationMats[0] * currentRotationMat[0] * scalingMats[0];
	glm::mat4 modelTransformationMat = translationMats[1] * currentRotationMat[1] * scalingMats[1];
	
	return HomogeneousVecToVec3(worldTransformationMat * modelTransformationMat * Vec3ToHomogeneousVec(vertices[index - 1]));
}


//returning triangles (faces)
const std::vector<std::vector<glm::vec3>> MeshModel::GetTriangles() const
{
	std::vector<std::vector<glm::vec3>> triangles;
	int faceCounts = GetFacesCount();
	for (int i = 0; i < faceCounts; i++)
	{
		Face temp_face(GetFace(i)); //gets ith Face
		int firstVIn = temp_face.GetVertexIndex(0);	//Gets first vertex's index in this face
		int secondVIn = temp_face.GetVertexIndex(1);
		int thirdVIn = temp_face.GetVertexIndex(2);
		glm::vec3 v1 = GetTransformedVertex(firstVIn);
		glm::vec3 v2 = GetTransformedVertex(secondVIn);
		glm::vec3 v3 = GetTransformedVertex(thirdVIn);
		std::vector<glm::vec3> triangle;
		triangle.push_back(v1);
		triangle.push_back(v2);
		triangle.push_back(v3);
		triangles.push_back(triangle);

	}

	return triangles;
}

const std::vector<std::vector<glm::vec3>> MeshModel::GetModelAxis() const
{
	return modelAxis;
}

const std::vector<std::vector<glm::vec3>> MeshModel::GetTransformedModelAxisWorld() const
{
	std::vector<std::vector<glm::vec3>> tranformdAxis;
	std::vector<glm::mat4> scalingMats = GetScalingMatrices();
	std::vector<glm::mat4> rotationMats = GetRotationMatrices();
	std::vector<glm::mat4> translationMats = GetTranslationMatrices();

	glm::mat4 worldTransformationMat = translationMats[0] * currentRotationMat[0] * scalingMats[0];

	//xAXis
	std::vector<glm::vec3> xAxis;
	xAxis.push_back(HomogeneousVecToVec3(worldTransformationMat * Vec3ToHomogeneousVec(modelAxis[0][0]))); //left
	xAxis.push_back(HomogeneousVecToVec3(worldTransformationMat * Vec3ToHomogeneousVec(modelAxis[0][1]))); //right
	//yAxis
	std::vector<glm::vec3> yAxis;
	yAxis.push_back(HomogeneousVecToVec3(worldTransformationMat * Vec3ToHomogeneousVec(modelAxis[1][0]))); //left
	yAxis.push_back(HomogeneousVecToVec3(worldTransformationMat * Vec3ToHomogeneousVec(modelAxis[1][1]))); //right
	//zAxis
	std::vector<glm::vec3> zAxis;
	zAxis.push_back(HomogeneousVecToVec3(worldTransformationMat * Vec3ToHomogeneousVec(modelAxis[2][0]))); //left
	zAxis.push_back(HomogeneousVecToVec3(worldTransformationMat * Vec3ToHomogeneousVec(modelAxis[2][1]))); //right
	
	tranformdAxis.push_back(xAxis);
	tranformdAxis.push_back(yAxis);
	tranformdAxis.push_back(zAxis);

	return tranformdAxis;
}

const std::vector<std::vector<glm::vec3>> MeshModel::GetTransformedModelAxisModel() const
{
	std::vector<std::vector<glm::vec3>> tranformdAxis;
	std::vector<glm::mat4> scalingMats = GetScalingMatrices();
	std::vector<glm::mat4> rotationMats = GetRotationMatrices();
	std::vector<glm::mat4> translationMats = GetTranslationMatrices();

	glm::mat4 worldTransformationMat = translationMats[0] * currentRotationMat[0] * scalingMats[0];
	glm::mat4 modelTransformationMat = translationMats[1] * currentRotationMat[1] * scalingMats[1];

	//xAXis
	std::vector<glm::vec3> xAxis;
	xAxis.push_back(HomogeneousVecToVec3(worldTransformationMat * modelTransformationMat * Vec3ToHomogeneousVec(modelAxisModel[0][0]))); //left
	xAxis.push_back(HomogeneousVecToVec3(worldTransformationMat * modelTransformationMat * Vec3ToHomogeneousVec(modelAxisModel[0][1]))); //right
	//yAxis
	std::vector<glm::vec3> yAxis;
	yAxis.push_back(HomogeneousVecToVec3(worldTransformationMat * modelTransformationMat * Vec3ToHomogeneousVec(modelAxisModel[1][0]))); //left
	yAxis.push_back(HomogeneousVecToVec3(worldTransformationMat * modelTransformationMat * Vec3ToHomogeneousVec(modelAxisModel[1][1]))); //right
	//zAxis
	std::vector<glm::vec3> zAxis;
	zAxis.push_back(HomogeneousVecToVec3(worldTransformationMat * modelTransformationMat * Vec3ToHomogeneousVec(modelAxisModel[2][0]))); //left
	zAxis.push_back(HomogeneousVecToVec3(worldTransformationMat * modelTransformationMat * Vec3ToHomogeneousVec(modelAxisModel[2][1]))); //right

	tranformdAxis.push_back(xAxis);
	tranformdAxis.push_back(yAxis);
	tranformdAxis.push_back(zAxis);

	return tranformdAxis;
}


void MeshModel::ShowWorldAxis()
{
	showAxisWorld = true;
}

void MeshModel::HideWorldAxis()
{
	showAxisWorld = false;
}

const bool MeshModel::GetWorldAxisShowState() const
{
	return showAxisWorld;
}


void MeshModel::ShowModelAxis()
{
	showAxisModel = true;
}

void MeshModel::HideModelAxis()
{
	showAxisModel = false;
}

const bool MeshModel::GetModelAxisShowState() const
{
	return showAxisModel;
}