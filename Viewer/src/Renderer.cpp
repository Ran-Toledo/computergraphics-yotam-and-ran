#include "Renderer.h"
#include "MeshModel.h"
#include "PointLight.h"
#include "ShaderProgram.h"
#include <imgui/imgui.h>
#include <vector>
#include <sstream>
#include <iostream>
#include <memory>
#include <algorithm>

Renderer::Renderer()
{
	skyBoxModel = Utils::LoadMeshModel("..\\Data\\cube.obj");
}

Renderer::~Renderer()
{

}

void Renderer::Render(const Scene& scene)
{
	if (scene.GetCameraCount() > 0 && scene.GetLightCount() > 0)
	{
		int modelCount = scene.GetModelCount();
		const Camera& camera = scene.GetActiveCamera();


		for(int currentModelIndex = 0; currentModelIndex < modelCount; currentModelIndex++)
		{
			const MeshModel& currentModel = scene.GetModel(currentModelIndex);

			if (!currentModel.IsReflectionActivated())
			{
				// Activate the 'colorShader' program (vertex and fragment shaders)
				colorShader.use();

				// Set the uniform variables
				colorShader.setUniform("vlightCount", scene.GetLightCount());
				colorShader.setUniform("model", currentModel.GetWorldTransformation() * currentModel.GetModelTransformation());
				colorShader.setUniform("view", camera.GetViewTransformation());
				colorShader.setUniform("projection", camera.GetProjectionTransformation());
				colorShader.setUniform("viewPos", camera.GetEye());
				colorShader.setUniform("textureActivated", currentModel.IsTextureActivated());
				colorShader.setUniform("toonShadingActivated", currentModel.IsToonShadingActivated());
				colorShader.setUniform("textureType", currentModel.GetTextureMode());

				std::shared_ptr<PointLight> pointLight;
				for (int i = 0; i < scene.GetLightCount(); i++)
				{
					const std::shared_ptr<Light> currentLight = scene.GetLight(i);

					if (pointLight = std::dynamic_pointer_cast<PointLight>(currentLight))
					{
						std::ostringstream s;
						s << "lightsTransform[" << i << "]";
						std::string mystring = s.str();
						colorShader.setUniform(mystring.c_str(), pointLight->GetWorldTransformation() * pointLight->GetModelTransformation());
					}

					std::ostringstream s;
					s << "lightsMaterial[" << i << "]";
					std::string mystring = s.str();

					std::string ambientString = mystring + ".ambient";
					std::string specularString = mystring + ".specular";
					std::string diffuseString = mystring + ".diffuse";
					std::string ambientStrengthString = mystring + ".ambientStrength";
					std::string diffuseStrengthString = mystring + ".diffuseStrength";
					std::string specularStrengthString = mystring + ".specularStrength";

					colorShader.setUniform(ambientString.c_str(), currentLight->GetMaterial().ambient);
					colorShader.setUniform(specularString.c_str(), currentLight->GetMaterial().specular);
					colorShader.setUniform(diffuseString.c_str(), currentLight->GetMaterial().diffuse);
					colorShader.setUniform(ambientStrengthString.c_str(), currentLight->GetAmbientStrength());
					colorShader.setUniform(specularStrengthString.c_str(), currentLight->GetSpecularStrength());
					colorShader.setUniform(diffuseStrengthString.c_str(), currentLight->GetDiffuseStrength());
				}



				colorShader.setUniform("material.ambient", currentModel.GetMaterial().ambient);
				colorShader.setUniform("material.textureMap", 0);
				colorShader.setUniform("material.normalMap", 1);
				colorShader.setUniform("material.specular", currentModel.GetMaterial().specular);
				colorShader.setUniform("material.diffuse", currentModel.GetMaterial().diffuse);
				colorShader.setUniform("material.shininess", currentModel.GetShininess());
				colorShader.setUniform("normalMappingActivated", currentModel.IsNormalMappingActivated());


				if (currentModel.IsTextureActivated())
				{
					texture1.bind(0);
					if (currentModel.IsNormalMappingActivated())
					{
						normalTexture.bind(1);
					}
				}

				// Drag our model's faces (triangles) in fill mode
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				glBindVertexArray(currentModel.GetVAO());
				glDrawArrays(GL_TRIANGLES, 0, currentModel.GetModelVertices().size());
				glBindVertexArray(0);

				// Unset 'texture1' as the active texture at slot #0
				if (currentModel.IsTextureActivated())
				{
					texture1.unbind(0);
					if (currentModel.IsNormalMappingActivated())
						normalTexture.unbind(1);
				}
			}
			else
			{
				reflectionShader.use();
				reflectionShader.setUniform("eyePos", camera.GetEye());
				reflectionShader.setUniform("model", currentModel.GetWorldTransformation() * currentModel.GetModelTransformation());
				reflectionShader.setUniform("view", camera.GetViewTransformation());
				reflectionShader.setUniform("projection", camera.GetProjectionTransformation());
				reflectionShader.setUniform("skybox", 0);

				glBindVertexArray(currentModel.GetVAO());
				texture2.bind(0);
				glDrawArrays(GL_TRIANGLES, 0, currentModel.GetModelVertices().size());
				texture2.unbind(0);
			}
		}

		// Now We Draw Lights
		for (int currentLightIndex = 0; currentLightIndex < scene.GetLightCount(); currentLightIndex++)
		{
			const std::shared_ptr<Light> currentLight = scene.GetLight(currentLightIndex);

			std::shared_ptr<PointLight> pointLight;

			if (pointLight = std::dynamic_pointer_cast<PointLight>(currentLight))
			{
				lightShader.use();

				lightShader.setUniform("model", pointLight->GetWorldTransformation() * pointLight->GetModelTransformation());
				lightShader.setUniform("view", camera.GetViewTransformation());
				lightShader.setUniform("projection", camera.GetProjectionTransformation());


				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				glBindVertexArray(pointLight->GetVAO());
				glDrawArrays(GL_TRIANGLES, 0, pointLight->GetModelVertices().size());
				glBindVertexArray(0);
			}
		}

		// Render SkyBox
		if (scene.IsSkyBoxActivated())
		{
			glDepthFunc(GL_LEQUAL);
			skyBoxShader.use();
			skyBoxShader.setUniform("view", glm::mat4(glm::mat3(camera.GetViewTransformation())));
			skyBoxShader.setUniform("projection", camera.GetProjectionTransformation());

			texture2.bind(0);
			glBindVertexArray(skyBoxModel->GetVAO());
			glDrawArrays(GL_TRIANGLES, 0, skyBoxModel->GetModelVertices().size());
			glDepthFunc(GL_LESS);
			texture2.unbind(0);
		}
	}
}

void Renderer::LoadShaders()
{
	colorShader.loadShaders("vshader_color.glsl", "fshader_color.glsl");
	lightShader.loadShaders("vshader_light.glsl", "fshader_light.glsl");
	skyBoxShader.loadShaders("vshader_skybox.glsl", "fshader_skybox.glsl");
	reflectionShader.loadShaders("vshader_reflection.glsl", "fshader_reflection.glsl");
}

void Renderer::LoadTextures()
{
	texture1.loadTexture("..\\Data\\diffuse.png", true);
	normalTexture.loadTexture("..\\Data\\normalmap.png", true);
	texture2.loadSkyBox();
}