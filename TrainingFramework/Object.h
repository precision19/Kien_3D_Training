#pragma once
#include "../Utilities/Math.h"
#include "Camera.h"

class Object {
private:
	Matrix worldMatrix;
	Matrix scaleMatrix;
public:
	Camera c;
	Vector3 position = { -0.0f, -1.0f, 0.0f };
	Vector3 scale = { 1.5f, 1.0f, 1.0f };
	Vector3 rotation = { 1.0f, 1.0f, 1.0f };
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