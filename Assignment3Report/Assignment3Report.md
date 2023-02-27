 ## Read the function Renderer::InitOpenGLRendering() and explain in the report how this texture is displayed
Generate a unique identifier for an OpenGL texture using glGenTextures then it generates a unique identifier for a vertex array object (VAO) using glGenVertexArrays after that it binds the VAO using glBindVertexArray then it generates a unique identifier for a buffer using glGenBuffers.
Then define the vertices and texture coordinates of a screen-sized rectangle to be used as the surface to display the texture. This is done with two arrays, vtc and tex.
Bind the buffer using glBindBuffer and allocate memory for the buffer and copy the vertex and texture data to it using glBufferData and glBufferSubData.
Load and compile a vertex and fragment shader program using InitShader and make the shader program the current one using glUseProgram and Specify the format and location of the vertex and texture data to the shader program using glVertexAttribPointer and enable the vertex and texture attribute arrays using glEnableVertexAttribArray and finally Specify the texture unit to be used by the shader program using glUniform1i.

## Update the MeshModel class to load the mesh on the GPU
![image](https://user-images.githubusercontent.com/116717525/221362867-fc75c34a-8554-4fcb-ad41-62d10546a6b8.png)
![image](https://user-images.githubusercontent.com/116717525/221362899-1670717c-dad4-42c7-b843-320a92691777.png)

## vertex shader
#version 330 core

    layout(location = 0) in vec3 pos;
    layout(location = 1) in vec3 normal;
    layout(location = 2) in vec2 texCoords;

 
    uniform mat4 model;
    uniform mat4 view;
    uniform mat4 projection;

    out vec3 color;

    void main()
    {
        gl_Position = projection * view *  model * vec4(pos, 1.0f);
    }

## fragment shader
#version 330 core

    out vec4 frag_color;

    void main()
    {
        frag_color = vec4(0.5,0.5,0.5,1);
    }
