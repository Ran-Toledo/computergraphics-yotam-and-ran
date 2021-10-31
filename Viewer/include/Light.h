#pragma once

#include <glm/glm.hpp>
#include "MeshModel.h"
#include "Utils.h"

class Light
{
public:
	Light(const Material& material);

	const Material& GetMaterial()  const;
	void SetMaterial(const Material& material);

	float GetAmbientStrength() const;
	float GetDiffuseStrength() const;
	float GetSpecularStrength() const;

	void SetAmbientStrength(float strength);
	void SetDiffuseStrength(float strength);
	void SetSpecularStrength(float strength);

	virtual ~Light() {};

protected:
	Material material;

	float ambientStrength;
	float specularStrength;
	float diffuseStrength;
};
