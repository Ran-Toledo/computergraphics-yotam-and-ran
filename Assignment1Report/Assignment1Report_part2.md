# Assignment 1 – Part II

## Mesh Vertices and Face output to console:

Console output attached to the report

![](https://i.ibb.co/JQNvdct/IMG1.png)


## Scale and translate vertices to fit inside the window

Transforming the vertices to fit inside the window was performed by using the
following **FitToScreen** function:

```cpp
void MeshModel::FitToScreen(const int viewport_width, const int viewport_height)
{
	if (firstRender)
	{
		float biggestX = std::max_element(vertices_.begin(), vertices_.end(), [](const glm::vec3& v0, const glm::vec3& v1)
			{
				return (v0.x < v1.x);
			})->x;

		float biggestY = std::max_element(vertices_.begin(), vertices_.end(), [](const glm::vec3& v0, const glm::vec3& v1)
			{
				return (v0.y < v1.y);
			})->y;

		translation[1].x = viewport_width / 2.0f - biggestX;
		translation[1].y = viewport_height / 2.5f - biggestY;

		firstRender = false;
	}

}
```


## Transfer the mesh triangles to the Renderer

![](https://i.ibb.co/yfCKrXX/IMG2.png)


## GUI for Local and World Transformations

The transformations are computed based on the following transformation function:

```cpp
const glm::vec3 MeshModel::GetTransformedVertex(int index) const
{
	std::vector<glm::mat4> transformations = { glm::mat4(1.0f), glm::mat4(1.0f) };

	for (int i = 0; i < translation.size(); i++)
	{
		transformations[i] *= glm::mat4(
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			translation[i].x, translation[i].y, translation[i].z, 1
		);
	}


	for (int i = 0; i < rotation.size(); i++)
	{
		transformations[i] *= glm::mat4(
			std::cosf(rotation[i] / 180.f * M_PI), std::sinf(rotation[i] / 180.f * M_PI), 0, 0,
			-std::sinf(rotation[i] / 180.f * M_PI), std::cosf(rotation[i] / 180.f * M_PI), 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
		);
	}

	for (int i = 0; i < scale.size(); i++)
	{
		transformations[i] *= glm::mat4(
			scale[i].x, 0, 0, 0,
			0, scale[i].y, 0, 0,
			0, 0, scale[i].z, 0,
			0, 0, 0, 1
		);
	}

	
	return HomogeneousToVec3(transformations[0] * transformations[1] * Vec3ToHomogeneous(vertices_[index]));
}

const glm::vec3 MeshModel::GetNormalForVertex(int index) const
{
	const glm::vec3& normal = HomogeneousToVec3(Vec3ToHomogeneous(normals_[index]) * glm::mat4(
		15.0f, 0, 0, 0,
		0, 15.0f, 0, 0,
		0, 0, 15.0f, 0,
		0, 0, 0, 1
	));
	const glm::vec3& v = GetTransformedVertex(index);

	return glm::vec3(v + normal);
}
```

![](https://i.ibb.co/tP7mkC2/IMG3.png)


## Model transformation in Local vs World frames

### Local translation and then World rotation

![](https://i.ibb.co/G2wp4T9/IMG4.png)

![](https://i.ibb.co/0MDXjwg/IMG5.png)

![](https://i.ibb.co/hCdmN4j/IMG6.png)

### World translation and then Local rotation

![](https://i.ibb.co/G2wp4T9/IMG4.png)

![](https://i.ibb.co/C8L31jy/IMG10.png)

![](https://i.ibb.co/SNxvB1Z/IMG11.png)


## Enable loading several models, switching between active models, and transforming each independently

![](https://i.ibb.co/cJqXcJw/IMG12.png)

![](https://i.ibb.co/D1BRsCX/IMG13.png)

![](https://i.ibb.co/7Vv2Bhb/IMG14.png)

![](https://i.ibb.co/ZmwfJ42/IMG15.png)
