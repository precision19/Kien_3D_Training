#pragma once
#include "../Utilities/Math.h"

class Object {
private:
	Matrix worldMatrix;
	Matrix scaleMatrix;
public:
	Vector3 position = { -0.0f, -0.0f, 0.0f };
	Vector3 scale = { 0.5f, 0.5f, 1.0f };
	Vector3 rotation = { 1.0f, 1.0f, 1.0f };
	float cornerX = 0;
	float cornerY = 180;
	float cornerZ = 90;
	Matrix WVP;
	Object();
	~Object();
	void InitWVP();
	void Draw();
	void Update(float deltaTime);
};