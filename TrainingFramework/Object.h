#pragma once
#include "../Utilities/Math.h"
#include <vector>
#include "Camera.h"
#include "Model.h";
#include "Texture.h";
#include "Shaders.h"

class Object {
private:
	Matrix worldMatrix;
	Matrix scaleMatrix;
public:
	GLuint vboId, textureID, iboID;
	Model* model;
	Shaders* shader;
	std::vector<Texture*>textures;
	int modelID, numTextures, cubetexturesID, shaderID, numLights, objectID;
	std::vector<int>texturedID;
	std::vector<int>lightID;
	Vector3 position = { 0.0f, 0.0f, 0.0f };
	Vector3 scale{ 1.0f, 1.0f, 1.0f };
	Vector3 rotation{ 0.0f, 0.0f, 0.0f };
	float angleX = 0;
	float angleY = 0;			
	float angleZ = 0; 
	Matrix WVP;	
	Object();
	~Object();
	void InitWVP();
	void Draw();
	void Update(float deltaTime);
};