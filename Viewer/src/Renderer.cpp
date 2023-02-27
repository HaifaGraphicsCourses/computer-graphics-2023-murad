#define _USE_MATH_DEFINES
#include <cmath>
#include <algorithm>
#include <iostream>
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
	delete[] z_buffer;
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
	z_buffer = new float[3 * w * h];
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
			z_buffer[Z_INDEX(viewport_width, i, j)] = std::numeric_limits<float>::max();
		}
	}
}
void Renderer::LoadShaders()
{
	colorShader.loadShaders("vshader.glsl", "fshader.glsl");
}

//void Renderer::Render(const Scene& scene)
//{
//	// TODO: Replace this code with real scene rendering code
//	int half_width = viewport_width / 2;
//	int half_height = viewport_height / 2;
//	// draw circle
//	auto count = scene.GetModelCount();
//	auto proj = scene.GetCamera(0).GetProjectionTransformation();
//	auto view = scene.GetCamera(0).GetViewTransformation();
//
//
//	if (scene.axis)
//	{
//		glm::vec4 xm(viewport_width, 0, 1, 1);
//		glm::vec4 x(-viewport_width, 0, 1, 1);
//		glm::vec4 ym(0, viewport_height, 1, 1);
//		glm::vec4 y(0, -viewport_height, 1, 1);
//
//		xm = proj * view * xm;
//		x = proj * view * x;
//		ym = proj * view * ym;
//		y = proj * view * y;
//
//		xm.x = (xm.x + 1) * half_width; xm.y = (xm.y + 1) * half_height;
//		x.x = (x.x + 1) * half_width; x.y = (x.y + 1) * half_height;
//		ym.x = (ym.x + 1) * half_width; ym.y = (ym.y + 1) * half_height;
//		y.x = (y.x + 1) * half_width; y.y = (y.y + 1) * half_height;
//
//		DrawLine(glm::vec2(xm.x, xm.y), glm::vec2(x.x, x.y), glm::vec3(1, 0, 0));
//		DrawLine(glm::vec2(ym.x, ym.y), glm::vec2(y.x, y.y), glm::vec3(1, 0, 0));
//	}
//
//	for (int active = 0; active < count; active++)
//	{
//		auto mesh = scene.GetModel(active);
//		auto faceCount = mesh.GetFacesCount();
//		mesh.screen = viewport_height;
//
//		glm::mat4 modelMatrix = mesh.GetTransformation();
//		glm::mat4 world = mesh.GetWorld();
//
//		float maxX = 0; float minX = 0;
//		float maxY = 0; float minY = 0;
//
//		for (int i = 0; i < faceCount; i++)
//		{
//			auto face = mesh.GetFace(i);
//			auto v1 = mesh.GetVertex((face.GetVertexIndex(0) - 1));
//			auto v2 = mesh.GetVertex((face.GetVertexIndex(1) - 1));
//			auto v3 = mesh.GetVertex((face.GetVertexIndex(2) - 1));
//
//			auto n1 = mesh.getNormal((face.GetNormalIndex(0) - 1));
//			auto n2 = mesh.getNormal((face.GetNormalIndex(1) - 1));
//			auto n3 = mesh.getNormal((face.GetNormalIndex(2) - 1));
//
//			float maxy = mesh.getMax();
//			float s = 300 / maxy;
//			v1.x *= s;
//			v1.y *= s;
//			v2.x *= s;
//			v2.y *= s;
//			v3.x *= s;
//			v3.y *= s;
//
//			float maxv = mesh.getMin();
//			int z1 = (v1.z / maxv + 1) * 10;
//			int z2 = (v2.z / maxv + 1) * 10;
//			int z3 = (v3.z / maxv + 1) * 10;
//
//			glm::vec3 color;
//
//			int depth = (z1 + z2 + z3) / 3;
//			if (depth == 0)
//				color = glm::vec3(0, 0.5, 0);
//			if (depth == 1)
//				color = glm::vec3(1, 0, 0);
//			if (depth == 2)
//				color = glm::vec3(0, 1, 0);
//			if (depth == 3)
//				color = glm::vec3(1, 1, 0);
//			if (depth == 4)
//				color = glm::vec3(0, 0, 1);
//			if (depth == 5)
//				color = glm::vec3(1, 0, 1);
//			if (depth == 6)
//				color = glm::vec3(0, 1, 1);
//			if (depth == 7)
//				color = glm::vec3(1, 1, 1);
//			if (depth == 8)
//				color = glm::vec3(1, 0.5, 0);
//			if (depth == 9)
//				color = glm::vec3(1, 0, 0.5);
//			if (depth == 10)
//				color = glm::vec3(1, 0.5, 0.5);
//			if (depth == 11)
//				color = glm::vec3(0.5, 0, 0);
//			if (depth == 12)
//				color = glm::vec3(0.5, 0.5, 0.5);
//			if (depth == 13)
//				color = glm::vec3(0, 0.5, 0);
//			if (depth == 14)
//				color = glm::vec3(0, 0, 0.5);
//			if (depth == 15)
//				color = glm::vec3(0.5, 0, 0.5);
//			if (depth == 16)
//				color = glm::vec3(0, 1, 0.5);
//			if (depth == 17)
//				color = glm::vec3(1, 1, 0.5);
//			if (depth == 18)
//				color = glm::vec3(1, 0, 0.5);
//			if (depth == 19)
//				color = glm::vec3(0.5, 0.5, 1);
//			if (depth == 20)
//				color = glm::vec3(0.5, 0, 1);
//
//			//cout << depth << endl;
//
//			v1 = proj * view * world * modelMatrix * v1;
//			v2 = proj * view * world * modelMatrix * v2;
//			v3 = proj * view * world * modelMatrix * v3;
//
//			auto normal1 = v1 + n1 * 0.1f;
//			auto normal2 = v2 + n2 * 0.1f;
//			auto normal3 = v3 + n3 * 0.1f;
//
//			v1.x = (v1.x + 1) * half_width; v1.y = (v1.y + 1) * half_height;
//			v2.x = (v2.x + 1) * half_width; v2.y = (v2.y + 1) * half_height;
//			v3.x = (v3.x + 1) * half_width; v3.y = (v3.y + 1) * half_height;
//
//
//
//			normal1.x = (normal1.x + 1) * half_width; normal1.y = (normal1.y + 1) * half_height;
//			normal2.x = (normal2.x + 1) * half_width; normal2.y = (normal2.y + 1) * half_height;
//			normal3.x = (normal3.x + 1) * half_width; normal3.y = (normal3.y + 1) * half_height;
//
//			glm::vec3 fn1(v1.x, v1.y, v1.z);
//			glm::vec3 fn2(v2.x, v2.y, v2.z);
//			glm::vec3 fn3(v3.x, v3.y, v3.z);
//
//			// Compute the face normal
//			glm::vec3 edge1 = fn2 - fn1;
//			glm::vec3 edge2 = fn3 - fn1;
//			glm::vec3 normal = glm::normalize(glm::cross(edge2, edge1));
//
//			// Compute the midpoint of the face
//			glm::vec3 midpoint = (fn1 + fn2 + fn3) / 3.0f;
//
//			// Compute the end point of the normal
//			glm::vec3 end_point = midpoint + normal * 0.1f;
//
//			//midpoint.x = (midpoint.x + 1) * half_width; midpoint.y = (midpoint.y + 1) * half_height;
//			//end_point.x = (end_point.x + 1) * half_width; end_point.y = (end_point.y + 1) * half_height;
//
//			if (!scene.fillTriangle && !scene.grey_scale && !scene.ambient)
//			{
//				DrawLine(glm::vec2(v1.x, v1.y), glm::vec2(v2.x, v2.y), glm::vec3(0, 0, 0));
//				DrawLine(glm::vec2(v1.x, v1.y), glm::vec2(v3.x, v3.y), glm::vec3(0, 0, 0));
//				DrawLine(glm::vec2(v2.x, v2.y), glm::vec2(v3.x, v3.y), glm::vec3(0, 0, 0));
//			}
//
//			if (scene.normals)
//			{
//			DrawLine(glm::vec2(midpoint.x, midpoint.y), glm::vec2(end_point.x, end_point.y), glm::vec3(1, 0, 0));
//			//DrawLine(glm::vec2(v1.x, v1.y), glm::vec2(normal1.x, normal1.y), glm::vec3(1, 0, 0));
//			//DrawLine(glm::vec2(v2.x, v2.y), glm::vec2(normal2.x, normal2.y), glm::vec3(1, 0, 0));
//			//DrawLine(glm::vec2(v3.x, v3.y), glm::vec2(normal3.x, normal3.y), glm::vec3(1, 0, 0));
//			}
//			
//
//				maxX = max(max(v3.x, v2.x), v1.x);
//				minX = min(min(v3.x, v2.x), v1.x);
//				maxY = max(max(v3.y, v2.y), v1.y);
//				minY = min(min(v3.y, v2.y), v1.y);
//
//			if (scene.bounding)
//			{
//				DrawLine(glm::vec2(minX, maxY), glm::vec2(maxX, maxY), color);
//				DrawLine(glm::vec2(minX, maxY), glm::vec2(minX, minY), color);
//				DrawLine(glm::vec2(maxX, maxY), glm::vec2(maxX, minY), color);
//				DrawLine(glm::vec2(minX, minY), glm::vec2(maxX, minY), color);
//			}
//
//			if (scene.fillTriangle)
//			{
//				for (int y = minY; y <= maxY; y++) {
//					for (int x = minX; x <= maxX; x++) {
//						// Compute the barycentric coordinates of the pixel
//						glm::vec4 p(x, y, 0, 0);
//						float u, v, w;
//						glm::vec4 v2v1 = v2 - v1;
//						glm::vec4 v3v1 = v3 - v1;
//						glm::vec4 pv1 = p - v1;
//						float d00 = dot(v2v1, v2v1);
//						float d01 = dot(v2v1, v3v1);
//						float d11 = dot(v3v1, v3v1);
//						float d20 = dot(pv1, v2v1);
//						float d21 = dot(pv1, v3v1);
//						float denom = d00 * d11 - d01 * d01;
//						v = (d11 * d20 - d01 * d21) / denom;
//						w = (d00 * d21 - d01 * d20) / denom;
//						u = 1.0f - v - w;
//
//						// Check if the pixel is inside the triangle
//						if (u >= 0 && v >= 0 && w >= 0) {
//							// Mark the pixel as inside
//							PutPixel(x, y, mesh.color);
//						}
//					}
//				}
//			}
//
//			if (scene.grey_scale)
//			{
//				for (int y = minY; y <= maxY; y++) {
//					for (int x = minX; x <= maxX; x++) {
//						// Compute the barycentric coordinates of the pixel
//						glm::vec4 p(x, y, 0, 0);
//						float u, v, w;
//						glm::vec4 v2v1 = v2 - v1;
//						glm::vec4 v3v1 = v3 - v1;
//						glm::vec4 pv1 = p - v1;
//						float d00 = dot(v2v1, v2v1);
//						float d01 = dot(v2v1, v3v1);
//						float d11 = dot(v3v1, v3v1);
//						float d20 = dot(pv1, v2v1);
//						float d21 = dot(pv1, v3v1);
//						float denom = d00 * d11 - d01 * d01;
//						v = (d11 * d20 - d01 * d21) / denom;
//						w = (d00 * d21 - d01 * d20) / denom;
//						u = 1.0f - v - w;
//
//						// Check if the pixel is inside the triangle
//						if (u >= 0 && v >= 0 && w >= 0) {
//							// Mark the pixel as inside
//							float depth2 = depth / 10.0;
//							z_buffer[Z_INDEX(viewport_width, x, y)] = min(depth2, z_buffer[Z_INDEX(viewport_width, x, y)]);
//							float alpha = z_buffer[Z_INDEX(viewport_width, x, y)];
//							alpha = 1 - alpha;
//							color = glm::vec3(alpha, alpha, alpha);
//							PutPixel(x, y, color);
//						}
//					}
//				}
//			}
//
//			if (scene.ambient)
//			{
//				//normal = (n1 + n2 + n3) / 3.0f;
//				glm::vec3 ambient = scene.light * 1.0f;
//				glm::vec3 lightDirection = glm::normalize(glm::vec3(scene.lightx, scene.lighty, scene.lightz) - midpoint);
//				float intensity = dot(lightDirection, normal);
//				glm::vec3 diffuse = glm::vec3(1,1,1) * max(intensity, 0.0f);
//				glm::vec3 reflectDir = glm::reflect(-lightDirection, normal);
//				float spec = pow(max(dot(lightDirection, reflectDir), 0.0f), 32);
//				glm::vec3 specular = spec * glm::vec3(1,1,1);
//				glm::vec3 finalColor = ( ambient + diffuse + specular) * mesh.color;
//				//cout << glm::to_string(normal) << endl;
//				//finalColor += mesh.color;
//				for (int y = minY; y <= maxY; y++) {
//					for (int x = minX; x <= maxX; x++) {
//						// Compute the barycentric coordinates of the pixel
//						glm::vec4 p(x, y, 0, 0);
//						float u, v, w;
//						glm::vec4 v2v1 = v2 - v1;
//						glm::vec4 v3v1 = v3 - v1;
//						glm::vec4 pv1 = p - v1;
//						float d00 = dot(v2v1, v2v1);
//						float d01 = dot(v2v1, v3v1);
//						float d11 = dot(v3v1, v3v1);
//						float d20 = dot(pv1, v2v1);
//						float d21 = dot(pv1, v3v1);
//						float denom = d00 * d11 - d01 * d01;
//						v = (d11 * d20 - d01 * d21) / denom;
//						w = (d00 * d21 - d01 * d20) / denom;
//						u = 1.0f - v - w;
//
//						// Check if the pixel is inside the triangle
//						if (u >= 0 && v >= 0 && w >= 0) {
//							// Mark the pixel as inside
//							/*float depth2 = depth / 10.0;
//							z_buffer[Z_INDEX(viewport_width, x, y)] = min(depth2, z_buffer[Z_INDEX(viewport_width, x, y)]);*/
//							PutPixel(x, y, finalColor);
//						}
//					}
//				}
//			}
//
//	}
//
//		if (scene.axis)
//		{
//			glm::vec4 xm(viewport_width, 0, 1, 1);
//			glm::vec4 x(-viewport_width, 0, 1, 1);
//			glm::vec4 ym(0, viewport_height, 1, 1);
//			glm::vec4 y(0, -viewport_height, 1, 1);
//			xm = proj * view * world * xm;
//			x = proj * view * world * x;
//			ym = proj * view * world * ym;
//			y = proj * view * world * y;
//			xm.x = (xm.x + 1) * half_width; xm.y = (xm.y + 1) * half_height;
//			x.x = (x.x + 1) * half_width; x.y = (x.y + 1) * half_height;
//			ym.x = (ym.x + 1) * half_width; ym.y = (ym.y + 1) * half_height;
//			y.x = (y.x + 1) * half_width; y.y = (y.y + 1) * half_height;
//			cout << glm::to_string(xm) << endl;
//			cout << glm::to_string(x) << endl;
//			DrawLine(glm::vec2(xm.x, xm.y), glm::vec2(x.x, x.y), glm::vec3(0, 0, 1));
//			DrawLine(glm::vec2(ym.x, ym.y), glm::vec2(y.x, y.y), glm::vec3(0, 0, 1));
//		}
//
//	}
//}
void Renderer::Render(const std::shared_ptr<Scene>& scene) {
	int half_width = viewport_width / 2;

	int half_height = viewport_height / 2;

	int camera_count = scene->GetCameraCount();

	if (camera_count > 0)

	{

		int model_count = scene->GetModelCount();

		if (model_count > 0)

		{

			const Camera& active = scene->GetActiveCamera();

			for (int index = 0; index < model_count; index++)

			{

				std::shared_ptr<MeshModel> current = scene->GetModel(index);

				colorShader.use();

				colorShader.setUniform("model", current->GetWorldTransformation() * current->GetModelTransformation());

				colorShader.setUniform("view", active.GetViewTransformation());

				colorShader.setUniform("projection", active.GetProjectionTransformation());

				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

				glBindVertexArray(current->GetVAO());

				glDrawArrays(GL_TRIANGLES, 0, current->GetModelVertices().size());

				glBindVertexArray(0);

				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

				glBindVertexArray(current->GetVAO());

				glDrawArrays(GL_TRIANGLES, 0, current->GetModelVertices().size());

				glBindVertexArray(0);

			}
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