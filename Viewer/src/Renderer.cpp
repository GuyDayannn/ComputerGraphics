#define _USE_MATH_DEFINES
#include <cmath>
#include <algorithm>

#include "Renderer.h"
#include "InitShader.h"

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
	int dp, dq, e, x1, y1, x2, y2;
	glm::ivec2 new_p1, new_p2;
	double a;
	int addedVal = 1;

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

	dq = new_p2.y - new_p1.y;
	dp = new_p2.x - new_p1.x;
	a = double(dq) / dp;

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
		dp = new_p2.y - new_p1.y;
		dq = new_p2.x - new_p1.x;
		x1 = new_p1.y;
		y1 = new_p1.x;
		x2 = new_p2.y;
		y2 = new_p2.x;
		e = -dq;
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
		e = -dp;
		addedVal = -1;
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
		e = -dq;
		//end of switching
		addedVal = -1; //reflecting
	}


	while (x1 <= x2)
	{
		//e = (2 * dq * x1) + (2 * dp * c) - (2 * dp * y1) - 1;
		if (posSlope)
		{
			if (e > 0)
			{
				y1 = y1 + 1;
				e = e - 2 * dp;
			}
		}
		else
		{
			if (e < 0)
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
	// draw circle

	int x0 = half_width;
	int y0 = half_height;

	int r = 300;
	double a;
	//0, 180 - x axis
	a = 0;
	DrawLine(glm::ivec2(half_width, half_height), glm::ivec2(half_width + r * cos(a), half_height + r * sin(a)), glm::vec3(0, 0, 0));
	a = M_PI;
	DrawLine(glm::ivec2(half_width, half_height), glm::ivec2(half_width + r * cos(a), half_height + r * sin(a)), glm::vec3(0, 0, 0));
	//90, 270 - y axis
	a = 1.5708;
	DrawLine(glm::ivec2(half_width, half_height), glm::ivec2(half_width + r * cos(a), half_height + r * sin(a)), glm::vec3(0, 0, 0));
	a = 4.71239;
	DrawLine(glm::ivec2(half_width, half_height), glm::ivec2(half_width + r * cos(a), half_height + r * sin(a)), glm::vec3(0, 0, 0));
	
	//60, -60
	a = 1.0472;
	DrawLine(glm::ivec2(half_width, half_height), glm::ivec2(half_width + r*cos(a), half_height + r*sin(a)), glm::vec3(0, 1, 0));
	a = -1.0472;
	DrawLine(glm::ivec2(half_width, half_height), glm::ivec2(half_width + r * cos(a), half_height + r * sin(a)), glm::vec3(0, 0, 1));
	
	//20, -20
	a = 0.349066;
	DrawLine(glm::ivec2(half_width, half_height), glm::ivec2(half_width + r * cos(a), half_height + r * sin(a)), glm::vec3(1, 0, 0));
	a = -0.349066;
	DrawLine(glm::ivec2(half_width, half_height), glm::ivec2(half_width + r * cos(a), half_height + r * sin(a)), glm::vec3(1, 1, 0));
	
	//120, -120
	a = 2.0944;
	DrawLine(glm::ivec2(half_width, half_height), glm::ivec2(half_width + r * cos(a), half_height + r * sin(a)), glm::vec3(0, 1, 1));
	a = -2.0944;
	DrawLine(glm::ivec2(half_width, half_height), glm::ivec2(half_width + r * cos(a), half_height + r * sin(a)), glm::vec3(0, 1, 0));

	//160, -160
	a = 2.79253;
	DrawLine(glm::ivec2(half_width, half_height), glm::ivec2(half_width + r * cos(a), half_height + r * sin(a)), glm::vec3(0, 0, 1));
	a = -2.79253;
	DrawLine(glm::ivec2(half_width, half_height), glm::ivec2(half_width + r * cos(a), half_height + r * sin(a)), glm::vec3(1, 0, 0));
}

int Renderer::GetViewportWidth() const
{
	return viewport_width;
}

int Renderer::GetViewportHeight() const
{
	return viewport_height;
}