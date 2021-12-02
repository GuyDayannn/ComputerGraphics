#define _USE_MATH_DEFINES
#include <cmath>
#include <algorithm>

#include "Renderer.h"
#include "InitShader.h"
#include <iostream>

#define INDEX(width,x,y,c) ((x)+(y)*(width))*3+(c)
#define Z_INDEX(width,x,y) ((x)+(y)*(width))

Renderer::Renderer(int viewport_width, int viewport_height) :
	viewport_width(viewport_width),
	viewport_height(viewport_height)
{
	InitOpenglRendering();
	CreateBuffers(viewport_width, viewport_height);
}

Renderer::~Renderer()
{
	delete[] color_buffer;
}

void Renderer::PutPixel(int i, int j, const glm::vec3& color)
{
	if (i < 0) return; if (i >= viewport_width) return;
	if (j < 0) return; if (j >= viewport_height) return;
	
	color_buffer[INDEX(viewport_width, i, j, 0)] = color.x;
	color_buffer[INDEX(viewport_width, i, j, 1)] = color.y;
	color_buffer[INDEX(viewport_width, i, j, 2)] = color.z;
}

void Renderer::DrawLine(const glm::ivec2& p1, const glm::ivec2& p2, const glm::vec3& color)
{
	// TODO: Implement bresenham algorithm
	// https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm

	bool Switch = false;
	bool posSlope = true; // true if positive slope, flase if negative
	int e, x1, y1, x2, y2;
	glm::ivec2 new_p1, new_p2;
	float dp, dq;
	float a;
	int addedVal = 1;

	if (p1 == p2)
	{
		PutPixel(p1.x, p1.y, color);
		return;
	}

	dq = p2.y - p1.y;
	dp = p2.x - p1.x;
	if (dp == 0) //vertical so a should be a > 1 or a < -1 - switching point order y wise
	{
		if (p1.y <= p2.y)
		{
			new_p1 = p1;
			new_p2 = p2;
		}
		else //switching between points so p1 comes before p2 (y wise)
		{
			new_p1 = p2;
			new_p2 = p1;
		}
		a = 3;
	}
	else // not vertical
	{
		a = dq / dp;
		if (p1.x <= p2.x)
		{
			new_p1 = p1;
			new_p2 = p2;
		}
		else //switching between points so p1 comes before p2 (x wise)
		{
			new_p1 = p2;
			new_p2 = p1;
		}
	}
	dq = new_p2.y - new_p1.y;
	dp = new_p2.x - new_p1.x;


	if (a >= 0 && a <= 1)// normal way
	{
		Switch = false;
		posSlope = true;
		x1 = new_p1.x;
		y1 = new_p1.y;
		x2 = new_p2.x;
		y2 = new_p2.y;
		e = -dp;
	}
	else if (a > 1) //switching roles between x and y
	{
		Switch = true;
		posSlope = true;
		//switching
		dq = new_p2.x - new_p1.x;
		dp = new_p2.y - new_p1.y;
		x1 = new_p1.y;
		y1 = new_p1.x;
		x2 = new_p2.y;
		y2 = new_p2.x;
		e = -dp;
		//end of switching
	}
	else if (a >= -1 && a < 0) //reflecting
	{
		Switch = false;
		posSlope = false;
		x1 = new_p1.x;
		y1 = new_p1.y;
		x2 = new_p2.x;
		y2 = new_p2.y;
		e = dp; //reflect
	}
	else if (a < -1) //switching + reflecting
	{
		Switch = true;
		posSlope = false;
		//switching - we will loop on y so switching so y1 will be high and y2 will bo low
		dp = new_p1.y - new_p2.y;
		dq = new_p1.x - new_p2.x;
		x1 = new_p2.y;
		y1 = new_p2.x;
		x2 = new_p1.y;
		y2 = new_p1.x;
		//end of switching
		e = dp; //reflect
	}


	while (x1 <= x2)
	{
		//e = (2 * dq * x1) + (2 * dp * c) - (2 * dp * y1) - 1;
		if (posSlope)
		{
			if (e >= 0)
			{
				y1 = y1 + 1;
				e = e - 2 * dp;
			}
		}
		else
		{
			if (e <= 0)
			{
				y1 = y1 - 1;
				e = e + 2 * dp;
			}
		}
		if (!Switch)
		{
			PutPixel(x1, y1, color);
		}
		else
		{
			PutPixel(y1, x1, color);
		}
		x1 = x1 + 1;
		e = e + 2 * dq;
	}
	
}

void swapPoints(glm::vec3& p1, glm::vec3& p2)
{
	glm::vec3 temp = p1;
	p1 = p2;
	p2 = temp;
}

std::vector<float> interpolate(float y0, float x0, float y1, float x1)
{
	std::vector<float> xCords;
	if (x1 != x0)
	{
		float m = (y1 - y0) / (x1 - x0);
		float c = y0 - m * x0;

		for (float y = y0; y < y1; y++)
		{
			float xCord = (y - c) / m;
			xCords.push_back(xCord);
		}

	}
	else
	{
		for (float y = y0; y < y1; y++)
		{
			xCords.push_back(x0);
		}

	}
	return xCords;

}

void Renderer::DrawTriangle(const glm::vec3& pnt0, const glm::vec3& pnt1, const glm::vec3& pnt2, const glm::vec3& color)
{
	glm::vec3 p0 = pnt0;
	glm::vec3 p1 = pnt1;
	glm::vec3 p2 = pnt2;
	int i = 0, j = 0, k = 0;

	if (p1.y < p0.y) swapPoints(p1, p0);
	if (p2.y < p0.y) swapPoints(p2, p0);
	if (p2.y < p1.y) swapPoints(p2, p1);
	//now yo<=y1<=y2

	std::vector<float> x01 = interpolate(p0.y, p0.x, p1.y, p1.x);
	std::vector<float> x12 = interpolate(p1.y, p1.x, p2.y, p2.x);
	std::vector<float> x02 = interpolate(p0.y, p0.x, p2.y, p2.x);

	if (p0.y == p1.y) //draw between 02 to 12
	{
		//std::cout << "p0.y == p1.y" << endl;
		float y = p0.y;
		for (y = p0.y; y < p2.y; y++, i++, k++) //loop from p0 to p2
		{
			DrawLine(glm::vec2(x02[i], y), glm::vec2(x12[k], y), color);
		}

	}
	else if (p1.y == p2.y) //draw between 02 to 01
	{
		//std::cout << "p1.y == p2.y" << endl;
		float y = p0.y;
		for (y = p0.y; y < p1.y; y++, i++, j++) //loop from p0 to p1
		{
			DrawLine(glm::vec2(x02[i], y), glm::vec2(x01[j], y), color);
		}
	}
	else
	{
		//std::cout << "else" << endl;
		float y = p0.y;
		for (y = p0.y; y < p1.y; y++, i++, j++) //loop from p0 to p1
		{
			DrawLine(glm::vec2(x02[i], y), glm::vec2(x01[j], y), color);
		}

		for (y = y; y < p2.y; y++, i++, k++) //loop from p0 to p2
		{
			DrawLine(glm::vec2(x02[i], y), glm::vec2(x12[k], y), color);
		}

	}
}

void Renderer::DrawColorMeshModel(const MeshModel& meshModel, const glm::vec3& color, const Camera& camera) //drawing all triangles
{
	std::vector<std::vector<glm::vec3>> triangles = meshModel.GetTriangles();
	glm::mat4x4 view_transformation = camera.GetViewTransformation();
	glm::mat4x4 projection_transformation = camera.GetProjectionTransformation();
	std::vector<glm::mat4> rotations = camera.GetCurrentRotations();
	glm::mat4 invertedRotationMats = glm::inverse(rotations[0] * rotations[1]);
	std::vector<glm::vec3> colors = meshModel.GetFaceColors();

	for (int i = 0; i < triangles.size(); i++)
	{
		std::vector<glm::vec3> triangle = triangles[i];
		glm::vec3 v1 = camera.GetTransformedVertex(triangle[0]);
		glm::vec3 v2 = camera.GetTransformedVertex(triangle[1]);
		glm::vec3 v3 = camera.GetTransformedVertex(triangle[2]);

		DrawTriangle(v1, v2, v3, colors[i]);
	}

}

//drawing mesh model by drawing triangles received
void Renderer::DrawMeshModel(const MeshModel& meshModel, const glm::vec3& color)
{
	std::vector<std::vector<glm::vec3>> triangles = meshModel.GetTriangles();
	
	for (int i = 0; i < triangles.size(); i++)
	{
		std::vector<glm::vec3> triangle = triangles[i];
		glm::vec3 v1 = triangle[0];
		glm::vec3 v2 = triangle[1];
		glm::vec3 v3 = triangle[2];

		DrawLine(glm::ivec2(v1.x,v1.y), glm::ivec2(v2.x,v2.y), color);
		DrawLine(glm::ivec2(v1.x, v1.y), glm::ivec2(v3.x, v3.y), color);
		DrawLine(glm::ivec2(v2.x, v2.y), glm::ivec2(v3.x, v3.y), color);
	}

}

void Renderer::DrawMeshModel(const MeshModel& meshModel, const glm::vec3& color, const Camera& camera) //drawing all triangles
{
	std::vector<std::vector<glm::vec3>> triangles = meshModel.GetTriangles();
	glm::mat4x4 view_transformation = camera.GetViewTransformation();
	glm::mat4x4 projection_transformation = camera.GetProjectionTransformation();
	std::vector<glm::mat4> rotations = camera.GetCurrentRotations();
	glm::mat4 invertedRotationMats = glm::inverse(rotations[0] * rotations[1]);
	
	for (int i = 0; i < triangles.size(); i++)
	{
		std::vector<glm::vec3> triangle = triangles[i];
		glm::vec3 v1 = camera.GetTransformedVertex(triangle[0]);
		glm::vec3 v2 = camera.GetTransformedVertex(triangle[1]);
		glm::vec3 v3 = camera.GetTransformedVertex(triangle[2]);



		DrawLine(glm::ivec2(v1.x, v1.y), glm::ivec2(v2.x, v2.y), color);
		DrawLine(glm::ivec2(v1.x, v1.y), glm::ivec2(v3.x, v3.y), color);
		DrawLine(glm::ivec2(v2.x, v2.y), glm::ivec2(v3.x, v3.y), color);
	}

}

void Renderer::DrawTrainglesBoundings(const MeshModel& meshModel, const glm::vec3& color, const Camera& camera) //drawing all triangles
{
	std::vector<std::vector<glm::vec3>> triangles = meshModel.GetTriangles();
	glm::mat4x4 view_transformation = camera.GetViewTransformation();
	glm::mat4x4 projection_transformation = camera.GetProjectionTransformation();
	std::vector<glm::mat4> rotations = camera.GetCurrentRotations();
	glm::mat4 invertedRotationMats = glm::inverse(rotations[0] * rotations[1]);
	float minX, maxX, minY, maxY;
	std::vector<glm::vec3> colors = meshModel.GetFaceColors();

	for (int i = 0; i < triangles.size(); i++)
	{
		std::vector<glm::vec3> triangle = triangles[i];
		glm::vec3 v1 = camera.GetTransformedVertex(triangle[0]);
		glm::vec3 v2 = camera.GetTransformedVertex(triangle[1]);
		glm::vec3 v3 = camera.GetTransformedVertex(triangle[2]);

		minX = std::min(std::min(v1.x, v2.x), v3.x);
		maxX = std::max(std::max(v1.x, v2.x), v3.x);
		minY = std::min(std::min(v1.y, v2.y), v3.y);
		maxY = std::max(std::max(v1.y, v2.y), v3.y);

		glm::vec2 minMin = glm::vec2(minX, minY);
		glm::vec2 minMax = glm::vec2(minX, maxY);
		glm::vec2 maxMin = glm::vec2(maxX, minY);
		glm::vec2 maxMax = glm::vec2(maxX, maxY);

		DrawLine(minMin, minMax, colors[i]);
		DrawLine(minMin, maxMin, colors[i]);
		DrawLine(maxMin, maxMax, colors[i]);
		DrawLine(minMax, maxMax, colors[i]);

	}

}



void Renderer::DrawMeshModelAxisWorld(const MeshModel& meshModel, const glm::vec3& color, const Camera& camera)
{
	std::vector<std::vector<glm::vec3>> axises = meshModel.GetTransformedModelAxisWorld();
	glm::mat4x4 view_transformation = camera.GetViewTransformation();
	glm::mat4x4 projection_transformation = camera.GetProjectionTransformation();
	std::vector<glm::mat4> rotations = camera.GetCurrentRotations();
	glm::mat4 invertedRotationMats = glm::inverse(rotations[0] * rotations[1]);

	for (int i = 0; i < axises.size(); i++)
	{
		std::vector<glm::vec3> axis = axises[i];
		glm::vec3 v1 = camera.GetTransformedVertex(axis[0]);
		glm::vec3 v2 = camera.GetTransformedVertex(axis[1]);

		if (i == 0) DrawLine(glm::ivec2(v1.x, v1.y), glm::ivec2(v2.x, v2.y), glm::vec3(1.0f, 0.0f, 0.0f));
		if (i == 1) DrawLine(glm::ivec2(v1.x, v1.y), glm::ivec2(v2.x, v2.y), glm::vec3(0.0f, 1.0f, 0.0f));
		if (i == 2) DrawLine(glm::ivec2(v1.x, v1.y), glm::ivec2(v2.x, v2.y), glm::vec3(0.0f, 0.0f, 1.0f));

	}

}

void Renderer::DrawMeshModelAxisModel(const MeshModel& meshModel, const glm::vec3& color, const Camera& camera)
{
	std::vector<std::vector<glm::vec3>> axises = meshModel.GetTransformedModelAxisModel();
	glm::mat4x4 view_transformation = camera.GetViewTransformation();
	glm::mat4x4 projection_transformation = camera.GetProjectionTransformation();
	std::vector<glm::mat4> rotations = camera.GetCurrentRotations();
	glm::mat4 invertedRotationMats = glm::inverse(rotations[0] * rotations[1]);

	for (int i = 0; i < axises.size(); i++)
	{
		std::vector<glm::vec3> axis = axises[i];
		glm::vec3 v1 = camera.GetTransformedVertex(axis[0]);
		glm::vec3 v2 = camera.GetTransformedVertex(axis[1]);

		if (i == 0) DrawLine(glm::ivec2(v1.x, v1.y), glm::ivec2(v2.x, v2.y), glm::vec3(1.0f, 0.0f, 0.0f));
		if (i == 1) DrawLine(glm::ivec2(v1.x, v1.y), glm::ivec2(v2.x, v2.y), glm::vec3(0.0f, 1.0f, 0.0f));
		if (i == 2) DrawLine(glm::ivec2(v1.x, v1.y), glm::ivec2(v2.x, v2.y), glm::vec3(0.0f, 0.0f, 1.0f));

	}

}

void Renderer::DrawMeshModelVerticesNormals(const MeshModel& meshModel, const glm::vec3& color, const Camera& camera)
{
	std::vector<std::vector<std::vector<glm::vec3>>> normals = meshModel.GetVerticesNormals();

	std::vector<glm::mat4> modelTranslation = meshModel.GetTranslationMatrices();
	std::vector<glm::mat4> modelRotations = meshModel.GetCurrentRotation();
	std::vector<glm::mat4> modelScale = meshModel.GetScalingMatrices();

	glm::mat4 modelTransformation = modelTranslation[0] * modelRotations[0] * modelScale[0] * modelTranslation[1] * modelRotations[1] * modelScale[1];

	glm::mat4x4 view_transformation = camera.GetViewTransformation();
	glm::mat4x4 projection_transformation = camera.GetProjectionTransformation();
	std::vector<glm::mat4> rotations = camera.GetCurrentRotations();
	glm::mat4 invertedRotationMats = glm::inverse(rotations[0] * rotations[1]);

	for (int i = 0; i < normals.size(); i++)
	{
		// <{<glm::vec3, glm::vec3>, <glm::vec3, glm::vec3>, <glm::vec3, glm::vec3>}>
		std::vector<std::vector<glm::vec3>> faceNormals = normals[i];

		for (int j = 0; j < faceNormals.size(); j++)
		{
			//<glm::vec3, glm::vec3>
			std::vector<glm::vec3> vertexNormal = faceNormals[j];
			glm::vec3 normalPoint1 = vertexNormal[0];
			glm::vec3 normalPoint2 = vertexNormal[1];
			glm::vec3 v1 = camera.GetTransformedVertex(MeshModel::HomogeneousVecToVec3(modelTransformation * MeshModel::Vec3ToHomogeneousVec(normalPoint1)));
			glm::vec3 v2 = camera.GetTransformedVertex(MeshModel::HomogeneousVecToVec3(modelTransformation * MeshModel::Vec3ToHomogeneousVec(normalPoint2)));

			DrawLine(v1, v2, color);

		}	
	}

}


void Renderer::DrawMeshModelFaceNormals(const MeshModel& meshModel, const glm::vec3& color, const Camera& camera)
{
	std::vector<std::vector<glm::vec3>> normals = meshModel.GetFacesNormals();

	std::vector<glm::mat4> modelTranslation = meshModel.GetTranslationMatrices();
	std::vector<glm::mat4> modelRotations = meshModel.GetCurrentRotation();
	std::vector<glm::mat4> modelScale = meshModel.GetScalingMatricesChangeable();
	
	glm::mat4 modelTransformation = modelTranslation[0] * modelRotations[0] * modelScale[0] * modelTranslation[1] * modelRotations[1] * modelScale[1];

	glm::mat4x4 view_transformation = camera.GetViewTransformation();
	glm::mat4x4 projection_transformation = camera.GetProjectionTransformation();
	std::vector<glm::mat4> rotations = camera.GetCurrentRotations();
	glm::mat4 invertedRotationMats = glm::inverse(rotations[0] * rotations[1]);

	for (int i = 0; i < normals.size(); i++)
	{
		// <{x,y,z}, {x,y,z}}>
		std::vector<glm::vec3> faceNormals = normals[i];

	
		//<glm::vec3, glm::vec3>
		glm::vec3 normalPoint1 = faceNormals[0];
		glm::vec3 normalPoint2 = faceNormals[1];
		glm::vec3 v1 = camera.GetTransformedVertex(MeshModel::HomogeneousVecToVec3(modelTransformation * MeshModel::Vec3ToHomogeneousVec(normalPoint1)));
		glm::vec3 v2 = camera.GetTransformedVertex(MeshModel::HomogeneousVecToVec3(modelTransformation * MeshModel::Vec3ToHomogeneousVec(normalPoint2)));

		DrawLine(v1, v2, color);

		
	}

}

void Renderer::DrawMeshModelBoundigBox(const MeshModel& meshModel, const glm::vec3& color, const Camera& camera)
{

	std::vector<glm::mat4> modelTranslation = meshModel.GetTranslationMatrices();
	std::vector<glm::mat4> modelRotations = meshModel.GetCurrentRotation();
	std::vector<glm::mat4> modelScale = meshModel.GetScalingMatricesChangeable();
	glm::mat4 modelTransformation = modelTranslation[0] * modelRotations[0] * modelScale[0] * modelTranslation[1] * modelRotations[1] * modelScale[1];

	glm::mat4x4 view_transformation = camera.GetViewTransformation();
	glm::mat4x4 projection_transformation = camera.GetProjectionTransformation();
	std::vector<glm::mat4> rotations = camera.GetCurrentRotations();
	glm::mat4 invertedRotationMats = glm::inverse(rotations[0] * rotations[1]);

	glm::mat4 modelCamMat = projection_transformation * invertedRotationMats * view_transformation * modelTransformation;


	int verticesCount = meshModel.GetVerticesCount();
	float maxX = meshModel.GetPureVertex(1).x;
	float minX = maxX;
	float maxY =  meshModel.GetPureVertex(1).y;
	float minY = maxY;
	float maxZ = meshModel.GetPureVertex(1).z;
	float minZ = maxZ;

	for (int i = 1; i <= verticesCount; i++)
	{
		glm::vec3 vec = meshModel.GetPureVertex(i);
		if (vec.x >= maxX)
			maxX = vec.x;
		if (vec.x <= minX)
			minX = vec.x;

		if (vec.y >= maxY)
			maxY = vec.y;
		if (vec.y <= minY)
			minY = vec.y;

		if (vec.z >= maxZ)
			maxZ = vec.z;
		if (vec.z <= minZ)
			minZ = vec.z;
	}

	glm::vec3 minMinMin(minX, minY, minZ);
	glm::vec3 maxMinMin(maxX, minY, minZ);
	glm::vec3 minMaxMin(minX, maxY, minZ);
	glm::vec3 maxMaxMin(maxX, maxY, minZ);
	glm::vec3 maxMinMax(maxX, minY, maxZ);
	glm::vec3 maxMaxMax(maxX, maxY, maxZ);
	glm::vec3 minMinMax(minX, minY, maxZ);
	glm::vec3 minMaxMax(minX, maxY, maxZ);

	minMinMin = camera.GetTransformedVertex(MeshModel::HomogeneousVecToVec3(modelTransformation * MeshModel::Vec3ToHomogeneousVec(minMinMin)));
	maxMinMin = camera.GetTransformedVertex(MeshModel::HomogeneousVecToVec3(modelTransformation * MeshModel::Vec3ToHomogeneousVec(maxMinMin)));
	minMaxMin = camera.GetTransformedVertex(MeshModel::HomogeneousVecToVec3(modelTransformation * MeshModel::Vec3ToHomogeneousVec(minMaxMin)));
	maxMaxMin = camera.GetTransformedVertex(MeshModel::HomogeneousVecToVec3(modelTransformation * MeshModel::Vec3ToHomogeneousVec(maxMaxMin)));
	maxMinMax = camera.GetTransformedVertex(MeshModel::HomogeneousVecToVec3(modelTransformation * MeshModel::Vec3ToHomogeneousVec(maxMinMax)));
	maxMaxMax = camera.GetTransformedVertex(MeshModel::HomogeneousVecToVec3(modelTransformation * MeshModel::Vec3ToHomogeneousVec(maxMaxMax)));
	minMinMax = camera.GetTransformedVertex(MeshModel::HomogeneousVecToVec3(modelTransformation * MeshModel::Vec3ToHomogeneousVec(minMinMax)));
	minMaxMax = camera.GetTransformedVertex(MeshModel::HomogeneousVecToVec3(modelTransformation * MeshModel::Vec3ToHomogeneousVec(minMaxMax)));


	DrawLine(minMinMin, maxMinMin, color);
	DrawLine(minMinMin, minMaxMin, color);
	DrawLine(minMinMin, minMinMax, color);
	DrawLine(maxMinMin, maxMaxMin, color);
	DrawLine(maxMinMin, maxMinMax, color);
	DrawLine(maxMinMax, maxMaxMax, color);
	DrawLine(maxMinMax, minMinMax, color);
	DrawLine(minMaxMax, maxMaxMax, color);
	DrawLine(minMaxMax, minMinMax, color);
	DrawLine(minMaxMax, minMaxMin, color);
	DrawLine(maxMaxMin, minMaxMin, color);
	DrawLine(maxMaxMin, maxMaxMax, color);

}

void Renderer::CreateBuffers(int w, int h)
{
	CreateOpenglBuffer(); //Do not remove this line.
	color_buffer = new float[3 * w * h];
	ClearColorBuffer(glm::vec3(0.0f, 0.0f, 0.0f));
}

//##############################
//##OpenGL stuff. Don't touch.##
//##############################

// Basic tutorial on how opengl works:
// http://www.opengl-tutorial.org/beginners-tutorials/tutorial-2-the-first-triangle/
// don't linger here for now, we will have a few tutorials about opengl later.
void Renderer::InitOpenglRendering()
{
	// Creates a unique identifier for an opengl texture.
	glGenTextures(1, &gl_screen_tex);

	// Same for vertex array object (VAO). VAO is a set of buffers that describe a renderable object.
	glGenVertexArrays(1, &gl_screen_vtc);

	GLuint buffer;

	// Makes this VAO the current one.
	glBindVertexArray(gl_screen_vtc);

	// Creates a unique identifier for a buffer.
	glGenBuffers(1, &buffer);

	// (-1, 1)____(1, 1)
	//	     |\  |
	//	     | \ | <--- The exture is drawn over two triangles that stretch over the screen.
	//	     |__\|
	// (-1,-1)    (1,-1)
	const GLfloat vtc[]={
		-1, -1,
		 1, -1,
		-1,  1,
		-1,  1,
		 1, -1,
		 1,  1
	};

	const GLfloat tex[]={
		0,0,
		1,0,
		0,1,
		0,1,
		1,0,
		1,1};

	// Makes this buffer the current one.
	glBindBuffer(GL_ARRAY_BUFFER, buffer);

	// This is the opengl way for doing malloc on the gpu. 
	glBufferData(GL_ARRAY_BUFFER, sizeof(vtc)+sizeof(tex), NULL, GL_STATIC_DRAW);

	// memcopy vtc to buffer[0,sizeof(vtc)-1]
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vtc), vtc);

	// memcopy tex to buffer[sizeof(vtc),sizeof(vtc)+sizeof(tex)]
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vtc), sizeof(tex), tex);

	// Loads and compiles a sheder.
	GLuint program = InitShader( "vshader.glsl", "fshader.glsl" );

	// Make this program the current one.
	glUseProgram(program);

	// Tells the shader where to look for the vertex position data, and the data dimensions.
	GLint  vPosition = glGetAttribLocation( program, "vPosition" );
	glEnableVertexAttribArray( vPosition );
	glVertexAttribPointer( vPosition,2,GL_FLOAT,GL_FALSE,0,0 );

	// Same for texture coordinates data.
	GLint  vTexCoord = glGetAttribLocation( program, "vTexCoord" );
	glEnableVertexAttribArray( vTexCoord );
	glVertexAttribPointer( vTexCoord,2,GL_FLOAT,GL_FALSE,0,(GLvoid *)sizeof(vtc) );

	//glProgramUniform1i( program, glGetUniformLocation(program, "texture"), 0 );

	// Tells the shader to use GL_TEXTURE0 as the texture id.
	glUniform1i(glGetUniformLocation(program, "texture"),0);
}

void Renderer::CreateOpenglBuffer()
{
	// Makes GL_TEXTURE0 the current active texture unit
	glActiveTexture(GL_TEXTURE0);

	// Makes glScreenTex (which was allocated earlier) the current texture.
	glBindTexture(GL_TEXTURE_2D, gl_screen_tex);

	// malloc for a texture on the gpu.
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, viewport_width, viewport_height, 0, GL_RGB, GL_FLOAT, NULL);
	glViewport(0, 0, viewport_width, viewport_height);
}

void Renderer::SwapBuffers()
{
	// Makes GL_TEXTURE0 the current active texture unit
	glActiveTexture(GL_TEXTURE0);

	// Makes glScreenTex (which was allocated earlier) the current texture.
	glBindTexture(GL_TEXTURE_2D, gl_screen_tex);

	// memcopy's colorBuffer into the gpu.
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, viewport_width, viewport_height, GL_RGB, GL_FLOAT, color_buffer);

	// Tells opengl to use mipmapping
	glGenerateMipmap(GL_TEXTURE_2D);

	// Make glScreenVtc current VAO
	glBindVertexArray(gl_screen_vtc);

	// Finally renders the data.
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Renderer::ClearColorBuffer(const glm::vec3& color)
{
	for (int i = 0; i < viewport_width; i++)
	{
		for (int j = 0; j < viewport_height; j++)
		{
			PutPixel(i, j, color);
		}
	}
}

void Renderer::Render(const Scene& scene)
{
	// TODO: Replace this code with real scene rendering code
	int half_width = viewport_width / 2;
	int half_height = viewport_height / 2;
	
	int modelCount = scene.GetModelCount();
	
	int camCount = scene.GetCameraCount();
	if (camCount != 0)
	{
		Camera cam = scene.GetCamera(scene.GetActiveCameraIndex());
		for (int i = 0; i < modelCount; i++)
		{
			DrawColorMeshModel(scene.GetModel(i), glm::vec3(0, 0, 0), cam);
			DrawMeshModel(scene.GetModel(i), glm::vec3(0, 0, 0), cam);
			if(scene.GetModel(i).displayBoundingRec) DrawTrainglesBoundings(scene.GetModel(i), glm::vec3(0.5, 0.5, 0.5), cam);
			if (scene.GetModel(i).GetWorldAxisShowState()) DrawMeshModelAxisWorld(scene.GetModel(i), glm::vec3(0, 0, 1), cam);
			if (scene.GetModel(i).GetModelAxisShowState()) DrawMeshModelAxisModel(scene.GetModel(i), glm::vec3(0, 0, 1), cam);
			if (scene.GetModel(i).GetFaceNormalsShowState()) DrawMeshModelFaceNormals(scene.GetModel(i), glm::vec3(0, 1, 1), cam);
			if (scene.GetModel(i).GetVertexNormalsShowState()) DrawMeshModelVerticesNormals(scene.GetModel(i), glm::vec3(1, 0, 1), cam);
			if (scene.GetModel(i).displayBoundingBox) DrawMeshModelBoundigBox(scene.GetModel(i), glm::vec3(1, 0.5, 0), cam);
		}
	}


	/*
	for (int i = 0; i < modelCount; i++)
	{
		DrawMeshModel(scene.GetModel(i), glm::vec3(0, 0, 0));	
	}
	*/
	
	/*
	MeshModel myModel = scene.GetModel(0);
	DrawMeshModel(myModel, glm::vec3(0, 0, 0));
	*/
	/*
	// draw circle
	int r = 150;
	int s = 1;
	
	
	for (int deg = 0; deg < 360; deg++)
	{
		//rand() % 2
		int x0 = half_width + 250; int y0 = half_height + 180;
		double red = 1;
		double green = 1;
		double blue = 0.3;
		int x = round(x0 + r * cos(s * deg * M_PI / 180));
		int y = round(y0 + r * sin(s * deg * M_PI / 180));
		DrawLine(glm::ivec2(x0, y0), glm::ivec2(x, y), glm::vec3(red, green, blue));
	}
	
	
	//Drawing
	
	//left side something - GREEN
	double a = 123;
	double x1 = 220; double y1 = 79; int rr = 185;
	double x2 = x1 + rr * cos(a * M_PI / 180);
	double y2 = y1 + rr * sin(a * M_PI / 180);
	double stepsize = 0.2;
	//DrawLine(glm::ivec2(x1, y1), glm::ivec2(x2, y2), glm::vec3(0, 0, 0));
	for (double x = x1; x < 416; x += stepsize)
	{

		DrawLine(glm::ivec2(x, y1), glm::ivec2(x2++, y2), glm::vec3(0, 0.7, 0));
		y1+=2*stepsize;
		x2 = round(x + rr * cos(a * M_PI / 180));
		y2 = round(y1 + rr * sin(a * M_PI / 180));
	}

	x1 = 220; y1 = 79;
	x2 = x1 + rr * cos(a * M_PI / 180);
	y2 = y1 + rr * sin(a * M_PI / 180);
	stepsize = 0.1;
	for (double x = x1; x < 416; x += stepsize)
	{

		DrawLine(glm::ivec2(x, y1), glm::ivec2(x2++, y2), glm::vec3(0, 0.7, 0));
		y1 += 2 * stepsize;
		x2 = round(x + rr * cos(a * M_PI / 180));
		y2 = round(y1 + rr * sin(a * M_PI / 180));
	}

	//lower something - BLUE
	x1 = 220; y1 = 79; rr = 190;
	x2 = x1 + rr * cos(63 * M_PI / 180);
	y2 = y1 + rr * sin(63 * M_PI / 180);
	for(int x=x1; x<680; x++)
		DrawLine(glm::ivec2(x, y1), glm::ivec2(x2++, y2), glm::vec3(0, 0.4, 1));

	//right side something - YELLOW
	x1 = 765; y1 = 201; rr = 491;
	x2 = x1 + rr * cos(120 * M_PI / 180);
	y2 = y1 + rr * sin(120 * M_PI / 180);
	//DrawLine(glm::ivec2(x1, y1), glm::ivec2(x2, y2), glm::vec3(1, 1, 0));
	for (int x = x1; x > 559; x--)
	{
		DrawLine(glm::ivec2(x, 249), glm::ivec2(x2--, y2), glm::vec3(0.8, 0.7, 0));
	}
	*/
}

int Renderer::GetViewportWidth() const
{
	return viewport_width;
}

int Renderer::GetViewportHeight() const
{
	return viewport_height;
}