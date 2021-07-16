#pragma once
#include "../Utilities/Math.h"

class Object {
private:
	Matrix worldMatrix;
	Matrix scaleMatrix;
public:
	Vector3 position = { -1.0f, -1.0f, 0.0f };
	Vector3 scale = { 1.0f, 1.0f, 2.0f };
	Vector3 rotation = { 0.0f, 1.0f, 0.0f };
	Matrix WVP;
	Object();
	~Object();
	void InitWVP();
	void Draw();
	void Update(float deltaTime);
};