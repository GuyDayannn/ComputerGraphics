#pragma once
#include "Scene.h"
#include <glad/glad.h>
#include <glm/glm.hpp>

class Renderer
{
public:
	Renderer(int viewportWidth, int viewportHeight);
	virtual ~Renderer();
	void Render(const Scene& scene);
	void SwapBuffers();
	void ClearColorBuffer(const glm::vec3& color);
	int GetViewportWidth() const;
	int GetViewportHeight() const;
	
private:
	void PutPixel(const int i, const int j, const glm::vec3& color);
	void DrawLine(const glm::ivec2& p1, const glm::ivec2& p2, const glm::vec3& color);
	void DrawTriangle(const glm::vec3& pnt0, const glm::vec3& pnt1, const glm::vec3& pnt2, const glm::vec3& color, float zfar);
	void DrawColorMeshModel(const MeshModel& meshModel, const glm::vec3& color, const Camera& camera);
	void DrawMeshModel(const MeshModel& meshModel, const glm::vec3& color);
	void DrawMeshModel(const MeshModel& meshModel, const glm::vec3& color, const Camera& camera);
	void DrawTrainglesBoundings(const MeshModel& meshModel, const glm::vec3& color, const Camera& camera);
	void DrawMeshModelAxisWorld(const MeshModel& meshModel, const glm::vec3& color, const Camera& camera);
	void DrawMeshModelAxisModel(const MeshModel& meshModel, const glm::vec3& color, const Camera& camera);
	void DrawMeshModelVerticesNormals(const MeshModel& meshModel, const glm::vec3& color, const Camera& camera);
	void DrawMeshModelFaceNormals(const MeshModel& meshModel, const glm::vec3& color, const Camera& camera);
	void DrawMeshModelBoundigBox(const MeshModel& meshModel, const glm::vec3& color, const Camera& camera);

	void CreateBuffers(int w, int h);
	void CreateOpenglBuffer();
	void InitOpenglRendering();

	float* color_buffer;
	int viewport_width;
	int viewport_height;
	std::vector<std::vector<float>> zBuffer;
	GLuint gl_screen_tex;
	GLuint gl_screen_vtc;
};
