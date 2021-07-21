#include "stdafx.h"
#include "Object.h"
#define pi 3.14159265359

void Object::InitWVP() {
	Matrix Rx, Ry, Rz;
	Matrix m_Translation, m_Rotation, m_Scale;
	m_Scale.SetScale(scale.x, scale.y, scale.z);
	if (angleX >= 90) angleX = 89;
	if (angleX <= -90) angleX = -89;
	m_Rotation = Rz.SetRotationZ(rotation.z * float(pi*angleZ / 180.0f)) * Rx.SetRotationX(rotation.x * float(pi*angleX / 180.0f)) * Ry.SetRotationY(rotation.y * float(pi*angleY / 180.0f));
	m_Translation.SetTranslation(position.x, position.y, position.z);
	worldMatrix = m_Scale * m_Rotation * m_Translation;
	//Matrix vMatrix, pMatrix;
	WVP = worldMatrix * Camera::GetInstance()->caculateViewMatrix() * Camera::GetInstance()->caculatePerspectiveMatrix();
}

Object::Object() {
	numCubes = 0; numLights = 0; numTextures = 0;
}

Object::~Object() {
	delete model;
	//delete shader;
}

void Object::Update(float frameTime) {
	//c.a = 0; c.w = 0; c.d = 0; c.s = 0;
	if (Camera::GetInstance()->isDirty == 1) {
		WVP = worldMatrix * Camera::GetInstance()->caculateViewMatrix() * Camera::GetInstance()->getPerspectiveMatrix();
	}
	else {
		WVP = worldMatrix * Camera::GetInstance()->getViewMatrix() * Camera::GetInstance()->getPerspectiveMatrix();
	}
}


