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
	auto proj = scene.GetCamera(0).GetProjectionTransformation();
	auto view = scene.GetCamera(0).GetViewTransformation();


	/*glm::vec4 xm(viewport_width, 0, 1, 1);
	glm::vec4 x(-viewport_width, 0, 1, 1);
	glm::vec4 ym(0, viewport_height, 1, 1);
	glm::vec4 y(0, -viewport_height, 1, 1);

	xm = proj * view * xm;
	x = proj * view * x;
	ym = proj * view * ym;
	y = proj * view * y;

	xm.x = (xm.x + 1) * half_width; xm.y = (xm.y + 1) * half_height;
	x.x = (x.x + 1) * half_width; x.y = (x.y + 1) * half_height;
	ym.x = (ym.x + 1) * half_width; ym.y = (ym.y + 1) * half_height;
	y.x = (y.x + 1) * half_width; y.y = (y.y + 1) * half_height;

	DrawLine(glm::vec2(xm.x, xm.y), glm::vec2(x.x, x.y), glm::vec3(1, 0, 0));
	DrawLine(glm::vec2(ym.x, ym.y), glm::vec2(y.x, y.y), glm::vec3(1, 0, 0));*/

	for(int active = 0; active < count; active++)
	{
		auto mesh = scene.GetModel(active);
		auto faceCount = mesh.GetFacesCount();
		mesh.screen = viewport_height;

		glm::mat4 modelMatrix = mesh.GetTransformation();
		glm::mat4 world = mesh.GetWorld();

		float maxX = 0; float minX = 1000;
		float maxY = 0; float minY = 1000;
		float lmaxX = -100; float lminX = 1000;
		float lmaxY = -100; float lminY = 1000;
		glm::vec4 maxmax;
		glm::vec4 minmax;
		glm::vec4 maxmin;
		glm::vec4 minmin;
		
		for (int i = 0; i < faceCount; i++)
		{
			auto face = mesh.GetFace(i);
			auto v1 =  mesh.GetVertex((face.GetVertexIndex(0) - 1));
			auto v2 =  mesh.GetVertex((face.GetVertexIndex(1) - 1));
			auto v3 =  mesh.GetVertex((face.GetVertexIndex(2) - 1));

			lmaxX = max(lmaxX, v1.x); lmaxX = max(lmaxX, v2.x); lmaxX = max(lmaxX, v3.x);
			lminX = min(lminX, v1.x); lminX = min(lminX, v1.x); lminX = min(lminX, v1.x);
			lmaxY = max(lmaxY, v1.y); lmaxY = max(lmaxY, v2.y); lmaxY = max(lmaxY, v3.y);
			lminY = min(lminY, v1.y); lminY = min(lminY, v1.y); lminY = min(lminY, v1.y);
			
			v1 = proj * view * world * modelMatrix * v1;
			v2 = proj * view * world * modelMatrix * v2;
			v3 = proj * view * world * modelMatrix * v3;

			v1.x = (v1.x + 1) * half_width; v1.y = (v1.y + 1) * half_height;
			v2.x = (v2.x + 1) * half_width; v2.y = (v2.y + 1) * half_height;
			v3.x = (v3.x + 1) * half_width; v3.y = (v3.y + 1) * half_height;

			DrawLine(glm::vec2(v1.x, v1.y),glm::vec2(v2.x, v2.y),glm::vec3(0, 0, 0));
			DrawLine(glm::vec2(v1.x, v1.y),glm::vec2(v3.x, v3.y),glm::vec3(0, 0, 0));
			DrawLine(glm::vec2(v2.x, v2.y),glm::vec2(v3.x, v3.y),glm::vec3(0, 0, 0));

			maxX = max(maxX, v1.x); maxX = max(maxX, v2.x); maxX = max(maxX, v3.x);
			minX = min(minX, v1.x); minX = min(minX, v1.x); minX = min(minX, v1.x);
			maxY = max(maxY, v1.y); maxY = max(maxY, v2.y); maxY = max(maxY, v3.y);
			minY = min(minY, v1.y); minY = min(minY, v1.y); minY = min(minY, v1.y);
			maxmax = glm::vec4(lmaxX, lmaxY, 0, 1);
			minmax = glm::vec4(lminX, lmaxY, 0, 1);
			maxmin = glm::vec4(lmaxX, lminY, 0, 1);
			minmin = glm::vec4(lminX, lminY, 0, 1);
			maxmax = proj * view * world * modelMatrix * maxmax;
			minmax = proj * view * world * modelMatrix * minmax;
			maxmin = proj * view * world * modelMatrix * maxmin;
			minmin = proj * view * world * modelMatrix * minmin;
			maxmax.x = (maxmax.x + 1) * half_width; maxmax.y = (maxmax.y + 1) * half_height;
			minmax.x = (minmax.x + 1) * half_width; minmax.y = (minmax.y + 1) * half_height;
			maxmin.x = (maxmin.x + 1) * half_width; maxmin.y = (maxmin.y + 1) * half_height;
			minmin.x = (minmin.x + 1) * half_width; minmin.y = (minmin.y + 1) * half_height;

		}

		DrawLine(glm::vec2(minmax.x, minmax.y), glm::vec2(maxmax.x, maxmax.y), glm::vec3(0, 1, 1));
		DrawLine(glm::vec2(minmax.x, minmax.y), glm::vec2(minmin.x, minmin.y), glm::vec3(0, 1, 1));
		DrawLine(glm::vec2(maxmax.x, maxmax.y), glm::vec2(maxmin.x, maxmin.y), glm::vec3(0, 1, 1));
		DrawLine(glm::vec2(minmin.x, minmin.y), glm::vec2(maxmin.x, maxmin.y), glm::vec3(0, 1, 1));
		DrawLine(glm::vec2(minX, maxY), glm::vec2(maxX, maxY), glm::vec3(0, 1, 1));
		DrawLine(glm::vec2(minX, maxY), glm::vec2(minX, minY), glm::vec3(0, 1, 1));
		DrawLine(glm::vec2(maxX, maxY), glm::vec2(maxX, minY), glm::vec3(0, 1, 1));
		DrawLine(glm::vec2(minX, minY), glm::vec2(maxX, minY), glm::vec3(0, 1, 1));

		/*glm::vec4 xm(viewport_width, 0, 1, 1);
		glm::vec4 x(-viewport_width, 0, 1, 1);
		glm::vec4 ym(0, viewport_height, 1, 1);
		glm::vec4 y(0, -viewport_height, 1, 1);
		xm = proj * view * world * xm;
		x = proj * view * world * x;
		ym = proj * view * world * ym;
		y = proj * view * world * y;
		xm.x = (xm.x + 1) * half_width; xm.y = (xm.y + 1) * half_height;
		x.x = (x.x + 1) * half_width; x.y = (x.y + 1) * half_height;
		ym.x = (ym.x + 1) * half_width; ym.y = (ym.y + 1) * half_height;
		y.x = (y.x + 1) * half_width; y.y = (y.y + 1) * half_height;
		cout << glm::to_string(xm) << endl;
		cout << glm::to_string(x) << endl;
		DrawLine(glm::vec2(xm.x, xm.y), glm::vec2(x.x, x.y), glm::vec3(0, 0, 1));
		DrawLine(glm::vec2(ym.x, ym.y), glm::vec2(y.x, y.y), glm::vec3(0, 0, 1));*/

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