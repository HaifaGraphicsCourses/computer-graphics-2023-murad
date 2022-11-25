#define _USE_MATH_DEFINES
#include <cmath>
#include <algorithm>

#include "Renderer.h"
#include "InitShader.h"
#include <iostream>
#include<glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

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
	//calculating range for line between start and end point
	float dx, dy, p, x, y;
	float x0 = p1.x, y0 = p1.y, x1 = p2.x, y1 = p2.y; // for easier use
	if (p1.x > p2.x) // if x0 > x1 wh swap points
	{
		swap(x0, x1);
		swap(y0, y1);
	}

	dx = x1 - x0;
	dy = y1 - y0;

	x = x0;
	y = y0;

	if (dx == 0) // for straight lines
	{
		while (y != y1)
		{
			PutPixel(x, y, color);
			if (dy > 0)
				y++;
			else
				y--;
		}
	}

	else if (abs(dy) > abs(dx)) // if y difference is greater than x difference we use this breshman algo for slopes greater than 1
	{
		p = 2 * dx - dy;
		while (y != y1)
		{
			if (p >= 0)
			{
				PutPixel(x, y, color);
				{
					x = x + 1;
					p = p + 2 * dx - 2 * abs(dy);
				}
			}
			else
			{
				PutPixel(x, y, color);
				p = p + 2 * dx;
			}
			if (dy > 0) // we move up if slope is positive and down if negative
				y = y + 1;
			else
				y = y - 1;
		}
	}

	else { // else we use this algo for slopes smaller than one
		p = 2 * abs(dy) - dx;
		while (x <= x1)
		{
			if (p >= 0)
			{
				PutPixel(x, y, color);
				if (dy < 0) // we check if slope is negative to move up or down
					y = y - 1;
				else
					y = y + 1;

				p = p + 2 * abs(dy) - 2 * dx;
			}
			else
			{
				PutPixel(x, y, color);

				p = p + 2 * abs(dy);
			}
			x = x + 1;
		}
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
	const GLfloat vtc[] = {
		-1, -1,
		 1, -1,
		-1,  1,
		-1,  1,
		 1, -1,
		 1,  1
	};

	const GLfloat tex[] = {
		0,0,
		1,0,
		0,1,
		0,1,
		1,0,
		1,1 };

	// Makes this buffer the current one.
	glBindBuffer(GL_ARRAY_BUFFER, buffer);

	// This is the opengl way for doing malloc on the gpu. 
	glBufferData(GL_ARRAY_BUFFER, sizeof(vtc) + sizeof(tex), NULL, GL_STATIC_DRAW);

	// memcopy vtc to buffer[0,sizeof(vtc)-1]
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vtc), vtc);

	// memcopy tex to buffer[sizeof(vtc),sizeof(vtc)+sizeof(tex)]
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vtc), sizeof(tex), tex);

	// Loads and compiles a sheder.
	GLuint program = InitShader("vshader.glsl", "fshader.glsl");

	// Make this program the current one.
	glUseProgram(program);

	// Tells the shader where to look for the vertex position data, and the data dimensions.
	GLint  vPosition = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 2, GL_FLOAT, GL_FALSE, 0, 0);

	// Same for texture coordinates data.
	GLint  vTexCoord = glGetAttribLocation(program, "vTexCoord");
	glEnableVertexAttribArray(vTexCoord);
	glVertexAttribPointer(vTexCoord, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)sizeof(vtc));

	//glProgramUniform1i( program, glGetUniformLocation(program, "texture"), 0 );

	// Tells the shader to use GL_TEXTURE0 as the texture id.
	glUniform1i(glGetUniformLocation(program, "texture"), 0);
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
	auto count = scene.GetModelCount();
	if (count > 0)
	{
		auto mesh = scene.GetModel(0);
		auto faceCount = mesh.GetFacesCount();
		for (int i = 0; i < faceCount; i++)
		{
			auto face = mesh.GetFace(i);
			auto v1 =  mesh.GetVertex((face.GetVertexIndex(0) - 1));
			auto v2 =  mesh.GetVertex((face.GetVertexIndex(1) - 1));
			auto v3 =  mesh.GetVertex((face.GetVertexIndex(2) - 1));
			v1 = mesh.translate() * mesh.scale(scene.scalex) * v1;
			v2 = mesh.translate() * mesh.scale(scene.scalex) * v2;
			v3 = mesh.translate() * mesh.scale(scene.scalex) * v3;
			DrawLine(glm::vec2(v1.x, v1.y),glm::vec2(v2.x, v2.y),glm::vec3(0, 0, 0));
			DrawLine(glm::vec2(v1.x, v1.y),glm::vec2(v3.x, v3.y),glm::vec3(0, 0, 0));
			DrawLine(glm::vec2(v2.x, v2.y),glm::vec2(v3.x, v3.y),glm::vec3(0, 0, 0));
		}
	}
}


int Renderer::GetViewportWidth() const
{
	return viewport_width;
}

int Renderer::GetViewportHeight() const
{
	return viewport_height;
}