#pragma once
#include "../Utilities/Math.h"
#include "Globals.h"
#define pi 3.14159265359

class Camera {
private: 
	GLfloat m_FOV = 60*pi/180;
	GLfloat m_Near = 1.0f;
	GLfloat m_Far = 500.0f;
	Matrix viewMatrix;
	Matrix worldMatrix;
	Matrix perspectiveMatrix;
	Matrix mRotation;
	Matrix inverseRotation;
	Matrix mTranslation;
	Matrix inverseTranslation;
	float speedCamera = 10.0f;
	float speedRotation = 100.0f * pi / 180;
public:
	bool isDirty;
	int keyPressed;
	float maxAngleX = 90.0f * pi / 180;
	float minAngleX = -90.0f * pi / 180;
	float angleRotaX;
	int w, a, s, d, leftKey, rightKey, upKey, downKey;
	Camera();
	Vector3 pos = Vector3{ 0.0f, 0.0f, 4.0f };
	Vector3 target = Vector3{ 0.0f, 0.0f, 1.0f };
	Vector3 up = Vector3(0.0f, 1.0f, 0.0f);
	Matrix getViewMatrix();
	Matrix getWorldMatrix();
	Matrix getPerspectiveMatrix();
	Matrix caculatePerspectiveMatrix();
	Matrix caculateViewMatrix();
	Matrix caculateWorldMatrix();
	void setRnTMatrix();
	Vector3 moveForward(float);
	Vector3 moveBack(float);
	Vector3 moveLeft(float);
	Vector3 moveRight(float);
	Vector4 rotaRight(float);
	Vector4 rotaLeft(float);
	Vector4 rotaUp(float);
	Vector4 rotaDown(float);
	void Update(float);
	/*Vector3 lookAt;
	Vector3 up = Vector3(0.0f, 1.0f, 0.0f);
	void treatingMovement();
	Matrix* ViewMatrix;
	Matrix* WorldMatrix;
	Vector3 xaxis;
	Vector3 yaxis;
	Vector3 zaxis;*/
};