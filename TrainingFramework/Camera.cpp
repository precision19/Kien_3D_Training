#include "stdafx.h"
#include "Camera.h"
#include <math.h>

Camera* Camera::c_Instance = NULL;

Camera* Camera::GetInstance() {
	if (c_Instance == NULL) {
		c_Instance = new Camera();
	}
	return c_Instance;
}

Camera::~Camera(void) {
	printf("%s\n", "Destructor SceneManager");
}

Camera::Camera(void) {
	w = 1;
	a = 1<<3; 
	s = 1<<1; 
	d = 1<<4;
	leftKey = 1<<5;
	rightKey = 1<<6;
	upKey = 1<<7;
	downKey = 1<<8;
	angleRotaX = 0;
	keyPressed = 0;
	isDirty = 0;
}

Matrix Camera::getPerspectiveMatrix() {
	return perspectiveMatrix;
}

Matrix Camera::getWorldMatrix() {
	return worldMatrix;
}

Matrix Camera::getViewMatrix() {
	return viewMatrix;
}

Matrix Camera::caculateViewMatrix() {
	Vector3 zaxis = (pos - target).Normalize();
	Vector3 xaxis = (up.Cross(zaxis)).Normalize();
	Vector3 yaxis = (zaxis.Cross(xaxis)).Normalize();
	viewMatrix.m[0][0] = xaxis.x;
	viewMatrix.m[0][1] = yaxis.x;
	viewMatrix.m[0][2] = zaxis.x;
	viewMatrix.m[0][3] = 0;
	viewMatrix.m[1][0] = xaxis.y;
	viewMatrix.m[1][1] = yaxis.y;
	viewMatrix.m[1][2] = zaxis.y;
	viewMatrix.m[1][3] = 0;
	viewMatrix.m[2][0] = xaxis.z;
	viewMatrix.m[2][1] = yaxis.z;
	viewMatrix.m[2][2] = zaxis.z;
	viewMatrix.m[2][3] = 0;
	viewMatrix.m[3][0] = -pos.Dot(xaxis);
	viewMatrix.m[3][1] = -pos.Dot(yaxis);
	viewMatrix.m[3][2] = -pos.Dot(zaxis);
	viewMatrix.m[3][3] = 1;
	return viewMatrix;
}

Matrix Camera::caculateWorldMatrix() {
	Vector3 zaxis = (pos - target).Normalize();
	Vector3 xaxis = (up.Cross(zaxis)).Normalize();
	Vector3 yaxis = (zaxis.Cross(xaxis)).Normalize();
	worldMatrix.m[0][0] = xaxis.x;
	worldMatrix.m[0][1] = xaxis.y;
	worldMatrix.m[0][2] = xaxis.z;
	worldMatrix.m[0][3] = 0;
	worldMatrix.m[1][0] = yaxis.x;
	worldMatrix.m[1][1] = yaxis.y;
	worldMatrix.m[1][2] = yaxis.z;
	worldMatrix.m[1][3] = 0;
	worldMatrix.m[2][0] = zaxis.x;
	worldMatrix.m[2][1] = zaxis.y;
	worldMatrix.m[2][2] = zaxis.z;
	worldMatrix.m[2][3] = 0;
	worldMatrix.m[3][0] = pos.x;
	worldMatrix.m[3][1] = pos.y;
	worldMatrix.m[3][2] = pos.z;
	worldMatrix.m[3][3] = 1;
	return worldMatrix;
}

Matrix Camera::caculatePerspectiveMatrix() {
	float aspect = (float)(Globals::screenWidth) / (float)Globals::screenHeight;
	perspectiveMatrix.SetPerspective(m_FOV, aspect, m_Near, m_Far);
	return perspectiveMatrix;
}

Vector3 Camera::moveForward(float deltaTime) {
	return -(pos - target).Normalize() * speedCamera * deltaTime;
}
Vector3 Camera::moveBack(float deltaTime) {
	return (pos - target).Normalize() * speedCamera * deltaTime;
}
Vector3 Camera::moveLeft(float deltaTime) {
	return -up.Cross((pos - target).Normalize()).Normalize() * speedCamera * deltaTime;
}
Vector3 Camera::moveRight(float deltaTime) {
	return up.Cross((pos - target).Normalize()).Normalize() * speedCamera * deltaTime;
}

Vector4 Camera::rotaRight(float deltaTime) {
	Vector4 localTarget = Vector4(0.0f, 0.0f, -(pos-target).Length(), 1.0f);
	float angle = -speedRotation * deltaTime;
	Matrix RotationMatrixAroundY;
	Vector4 localOy = Vector4(0.0f, 1.0f, 0.0f, 0.0f) * getViewMatrix();
	RotationMatrixAroundY.SetRotationAngleAxis(angle, localOy.x, localOy.y, localOy.z);
	Vector4 localNewTarget = localTarget * RotationMatrixAroundY;
	Vector4 worldNewTarget = localNewTarget * caculateWorldMatrix();
	return worldNewTarget;
}

Vector4 Camera::rotaLeft(float deltaTime) {
	Vector4 localTarget = Vector4(0.0f, 0.0f, -(pos - target).Length(), 1.0f);
	float angle = +speedRotation * deltaTime;
	Matrix RotationMatrixAroundY;
	Vector4 localOy = Vector4(0.0f, 1.0f, 0.0f, 0.0f) * getViewMatrix();
	RotationMatrixAroundY.SetRotationAngleAxis(angle, localOy.x, localOy.y, localOy.z);
	Vector4 localNewTarget = localTarget * RotationMatrixAroundY;
	Vector4 worldNewTarget = localNewTarget * caculateWorldMatrix();
	return worldNewTarget;
}

Vector4 Camera::rotaUp(float deltaTime) {
	Vector4 localTarget = Vector4(0.0f, 0.0f, -(pos - target).Length(), 1.0f);
	float angle = +speedRotation * deltaTime;
	if (angleRotaX + angle > maxAngleX) {
		angle = maxAngleX - 0.001f - angleRotaX;
	}
	angleRotaX = angleRotaX + angle;
	Matrix RotationMatrixAroundX;
	Vector4 localOx = Vector4(1.0f, 0.0f, 0.0f, 0.0f) * getViewMatrix();
	RotationMatrixAroundX.SetRotationAngleAxis(angle, 1.0f, 0.0f, 0.0f);
	Vector4 localNewTarget = localTarget * RotationMatrixAroundX;
	Vector4 worldNewTarget = localNewTarget * caculateWorldMatrix();
	return worldNewTarget;
}

Vector4 Camera::rotaDown(float deltaTime) {
	Vector4 localTarget = Vector4(0.0f, 0.0f, -(pos - target).Length(), 1.0f);
	float angle = -speedRotation * deltaTime;
	if (angleRotaX + angle < minAngleX) {
		angle = minAngleX + 0.001f - angleRotaX;
	}
	angleRotaX = angleRotaX + angle;
	Matrix RotationMatrixAroundX;
	Vector4 localOx = Vector4(1.0f, 0.0f, 0.0f, 0.0f) * getViewMatrix();
	RotationMatrixAroundX.SetRotationAngleAxis(angle, 1.0f, 0.0f, 0.0f);
	Vector4 localNewTarget = localTarget * RotationMatrixAroundX;
	Vector4 worldNewTarget = localNewTarget * caculateWorldMatrix();
	return worldNewTarget;
}

void Camera::Update(float deltaTime) {
	Vector3 deltaMove;
	if (keyPressed & w) {
		deltaMove = moveForward(deltaTime);
		pos += deltaMove;
		target += deltaMove;
		isDirty = 1;
	}
	if (keyPressed & s) {
		deltaMove = moveBack(deltaTime);
		pos += deltaMove;
		target += deltaMove;
		isDirty = 1;
	}
	if (keyPressed & a) {
		deltaMove = moveLeft(deltaTime);
		pos += deltaMove;
		target += deltaMove;
		isDirty = 1;
	}
	if (keyPressed & d) {
		deltaMove = moveRight(deltaTime);
		pos += deltaMove;
		target += deltaMove;
		isDirty = 1;
	}
	if (keyPressed & leftKey) {
		Vector4 worldNewTarget = rotaLeft(deltaTime);
		target.x = worldNewTarget.x;
		target.y = worldNewTarget.y;
		target.z = worldNewTarget.z;
		isDirty = 1;
	}
	if (keyPressed & rightKey) {
		Vector4 worldNewTarget = rotaRight(deltaTime);
		target.x = worldNewTarget.x;
		target.y = worldNewTarget.y;
		target.z = worldNewTarget.z;
		isDirty = 1;
	}
	if (keyPressed & upKey) {
		Vector4 worldNewTarget = rotaUp(deltaTime);
		target.x = worldNewTarget.x;
		target.y = worldNewTarget.y;
		target.z = worldNewTarget.z;
		isDirty = 1;
	}
	if (keyPressed & downKey) {
		Vector4 worldNewTarget = rotaDown(deltaTime);
		target.x = worldNewTarget.x;
		target.y = worldNewTarget.y;
		target.z = worldNewTarget.z;
		isDirty = 1;
	}
	//w = 0; a = 0; s = 0; d = 0; leftKey = 0; rightKey = 0; upKey = 0; downKey = 0;
}

void Camera::setRnTMatrix() {
	Vector3 zaxis = (pos - target).Normalize();
	Vector3 xaxis = (up.Cross(zaxis)).Normalize();
	Vector3 yaxis = (zaxis.Cross(xaxis)).Normalize();
	//set rotation
	mRotation.m[0][0] = xaxis.x;
	mRotation.m[0][1] = xaxis.y;
	mRotation.m[0][2] = xaxis.z;
	mRotation.m[0][3] = 0;
	mRotation.m[1][0] = yaxis.x;
	mRotation.m[1][1] = yaxis.y;
	mRotation.m[1][2] = yaxis.z;
	mRotation.m[1][3] = 0;
	mRotation.m[2][0] = zaxis.x;
	mRotation.m[2][1] = zaxis.y;
	mRotation.m[2][2] = zaxis.z;
	mRotation.m[2][3] = 0;
	mRotation.m[0][0] = 0;
	mRotation.m[0][1] = 0;
	mRotation.m[0][2] = 0;
	mRotation.m[0][3] = 1;
	//inverse rotation
	inverseRotation.m[0][0] = xaxis.x;
	inverseRotation.m[1][0] = xaxis.y;
	inverseRotation.m[2][0] = xaxis.z;
	inverseRotation.m[3][0] = 0;
	inverseRotation.m[0][1] = yaxis.x;
	inverseRotation.m[1][1] = yaxis.y;
	inverseRotation.m[2][1] = yaxis.z;
	inverseRotation.m[3][1] = 0;
	inverseRotation.m[0][2] = zaxis.x;
	inverseRotation.m[1][2] = zaxis.y;
	inverseRotation.m[2][2] = zaxis.z;
	inverseRotation.m[3][2] = 0;
	inverseRotation.m[0][3] = 0;
	inverseRotation.m[1][3] = 0;
	inverseRotation.m[2][3] = 0;
	inverseRotation.m[3][3] = 1;
	//translation matrix
	mTranslation.m[0][0] = 1;
	mTranslation.m[0][1] = 0;
	mTranslation.m[0][2] = 0;
	mTranslation.m[0][3] = 0;
	mTranslation.m[1][0] = 0;
	mTranslation.m[1][1] = 1;
	mTranslation.m[1][2] = 0;
	mTranslation.m[1][3] = 0;
	mTranslation.m[2][0] = 0;
	mTranslation.m[2][1] = 0;
	mTranslation.m[2][2] = 1;
	mTranslation.m[2][3] = 0;
	mTranslation.m[3][0] = pos.x;
	mTranslation.m[3][1] = pos.y;
	mTranslation.m[3][2] = pos.z;
	mTranslation.m[3][3] = 1;
	//inverse matrix
	inverseTranslation.m[0][0] = 1;
	inverseTranslation.m[0][1] = 0;
	inverseTranslation.m[0][2] = 0;
	inverseTranslation.m[0][3] = 0;
	inverseTranslation.m[1][0] = 0;
	inverseTranslation.m[1][1] = 1;
	inverseTranslation.m[1][2] = 0;
	inverseTranslation.m[1][3] = 0;
	inverseTranslation.m[2][0] = 0;
	inverseTranslation.m[2][1] = 0;
	inverseTranslation.m[2][2] = 1;
	inverseTranslation.m[2][3] = 0;
	inverseTranslation.m[3][0] = -pos.x;
	inverseTranslation.m[3][1] = -pos.y;
	inverseTranslation.m[3][2] = -pos.z;
	inverseTranslation.m[3][3] = 1;
}