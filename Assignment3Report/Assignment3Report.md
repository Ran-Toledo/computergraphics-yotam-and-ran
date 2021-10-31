# Assignment 3

## Renderer::InitOpenGLRendering() Function 
This function's general purpose is to allocate space on the GPU and prepare for actual data (from our color_buffer_ generally) to be 
passed to the shaders.
First we create a unique identifier for the texture like any texture in OpenGL, we do the same for the vertex array, we allocate the 
space needed for our vertex array
and texture coordinates on the GPU. We then use our glsl files to compile a shader composed of fragment and vertex shader. Then we pass 
the shaders information about 
how exactly should it access the data we allocated eariler that is the vertex array and the texture coordinates (we pass a pointer to 
the start of the buffer along 
with an integer informing the shader how much data we have in each vertex for example 2 in x,y coordinates), we finally specify the 
texture ID for the shaders to know 
which one are we currently using.

## Vertex Shader Code (Initial, for Solid Color)
```c
#version 330 core
layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 fragPos;
out vec3 fragNormal;

void main()
{
	fragPos = vec3(model * vec4(pos, 1.0f));
	fragNormal = mat3(model) * normal;

	gl_Position = projection * view * model * vec4(pos, 1.0f);
}
```

## Fragment Shader Code (Initial, for Solid Color)
```c
#version 330 core
out vec4 FragColor;

uniform vec3 modelColor;

void main()
{
    FragColor = vec4(modelColor, 1.0f);
}
```

## Updated Renderer vertex attributes and uniforms (Initial, for Solid Color)
```cpp
colorShader.use();

// Set the uniform variables
colorShader.setUniform("model", currentModel.GetWorldTransformation() * currentModel.GetModelTransformation());
colorShader.setUniform("view", camera.GetViewTransformation());
colorShader.setUniform("projection", camera.GetProjectionTransformation());
colorShader.setUniform("modelColor", currentModel.GetColor());

// Drag our model's faces (triangles) in fill mode
glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
glBindVertexArray(currentModel.GetVAO());
glDrawArrays(GL_TRIANGLES, 0, currentModel.GetModelVertices().size());
glBindVertexArray(0);
```

## Phong Shading Implementation

**Model: Ogre**

![](https://i.ibb.co/y6CgqLM/ogre.png)
![](https://i.ibb.co/x81Jp2k/ogre2.png)
![](https://i.ibb.co/ZHF0SCm/ogre3.png)
![](https://i.ibb.co/KsSSSyJ/ogre4.png)
![](https://i.ibb.co/bFgSQFc/ogre5.png)
![](https://i.ibb.co/c6KwkZs/ogre6.png)

**Model: Armadillo**

![](https://i.ibb.co/DMzm9bW/armadillo.png)
![](https://i.ibb.co/SBSvVmh/armadillo2.png)
![](https://i.ibb.co/1sK5wj4/armadillo3.png)
![](https://i.ibb.co/vCFBw92/armadillo4.png)

## Texture Mapping
### Plane Mapping
![](https://i.ibb.co/9vg91WD/Sphere-Plane.png)
![](https://i.ibb.co/DMmGqN8/Teapot-Plane.png)

### Cylinderical Mapping
![](https://i.ibb.co/gWJ2hJQ/Sphere-Cylindrical.png)
![](https://i.ibb.co/kGqWyxZ/Teapot-Cylindrical.png)

### Spherical Mapping
![](https://i.ibb.co/kX3dQX9/Sphere-Spherical.png)
![](https://i.ibb.co/d6My954/Teapot-Spherical.png)

### Texture Coordinates
![](https://i.ibb.co/QQnbGNG/Texture-Coords-2.png)
![](https://i.ibb.co/rknfcwc/Texture-Coords.png)

## Normal Mapping

**Model: Sphere**  

Before enabling normal mapping:  

![](https://i.ibb.co/cJLCZHv/Normal-Mapping-before.png)

After normal mapping:

![](https://i.ibb.co/6gLRRXX/Normal-Mapping-after.png)

**Model: Cube**

Before enabling normal mapping:

![](https://i.ibb.co/HNDS7K9/Normal-Mapping-2-before.png)

After normal mapping:

![](https://i.ibb.co/c68f7s0/Normal-Mapping-2-after.png)

**other example:**
![](https://i.ibb.co/VTBBdmP/Texture-Coords-with-Normal-Mapping.png)


## Environment Mapping
![](https://i.ibb.co/M8fPpKZ/spherereflect.png)

![](https://i.ibb.co/g4YqR35/teapotreflect.png)

### Bonus: Added SkyBox
![](https://i.ibb.co/CJYLQjq/Environment-Mapping.png)

![](https://i.ibb.co/zhxsWCk/Environment-Mapping-2.png)

## Toon Shading
**Model: Teapot**

![](https://i.ibb.co/gzNV1hW/Toon-Shading-1.png)
![](https://i.ibb.co/FVKhSTY/Toon-Shading-2.png)

**other examples:**
![](https://i.ibb.co/KLjkT12/Toon-Shading-3.png)
![](https://i.ibb.co/n8n4mg6/Toon-Shading-5.png)
