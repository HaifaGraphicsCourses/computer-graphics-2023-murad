 ## Read the function Renderer::InitOpenGLRendering() and explain in the report how this texture is displayed
Generate a unique identifier for an OpenGL texture using glGenTextures then it generates a unique identifier for a vertex array object (VAO) using glGenVertexArrays after that it binds the VAO using glBindVertexArray then it generates a unique identifier for a buffer using glGenBuffers.
Then define the vertices and texture coordinates of a screen-sized rectangle to be used as the surface to display the texture. This is done with two arrays, vtc and tex.
Bind the buffer using glBindBuffer and allocate memory for the buffer and copy the vertex and texture data to it using glBufferData and glBufferSubData.
Load and compile a vertex and fragment shader program using InitShader and make the shader program the current one using glUseProgram and Specify the format and location of the vertex and texture data to the shader program using glVertexAttribPointer and enable the vertex and texture attribute arrays using glEnableVertexAttribArray and finally Specify the texture unit to be used by the shader program using glUniform1i.

## Update the MeshModel class to load the mesh on the GPU
![MeshViewer - Microsoft Visual Studio 2_21_2023 12_03_36 AM](https://user-images.githubusercontent.com/115185916/220205097-90a27761-6d22-42f4-8957-8d69f9f02e96.png)
