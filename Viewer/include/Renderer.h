#pragma once
#include "Scene.h"
#include "ShaderProgram.h"
#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <memory>
#include "Texture2D.h"

class Renderer
{
private:
	ShaderProgram lightShader;
	ShaderProgram colorShader;
	ShaderProgram skyBoxShader;
	ShaderProgram reflectionShader;
	Texture2D texture1;
	Texture2D texture2;
	Texture2D normalTexture;
	std::shared_ptr<MeshModel> skyBoxModel;

public:
	Renderer();
	~Renderer();

	void Render(const Scene& scene);
	void LoadShaders();
	void LoadTextures();
};
