#include "stdafx.h"
#include "Object.h"
#define pi 3.14159265359

void Object::InitWVP() {
	Matrix Rx, Ry, Rz;
	Matrix m_Translation, m_Rotation, m_Scale;
	m_Scale.SetScale(scale.x, scale.y, scale.z);
	m_Rotation = Rz.SetRotationZ(rotation.z * float(75.0f / 180.0f)) * Rx.SetRotationX(rotation.x * float(75.0f / 180.0f)) * Ry.SetRotationY(rotation.y * float(120.0f / 180.0f));
	m_Translation.SetTranslation(position.x, position.y, position.z);
	worldMatrix = m_Scale * m_Rotation * m_Translation;
	//Matrix vMatrix, pMatrix;
	WVP = worldMatrix;// *vMatrix.SetIdentity()* pMatrix.SetIdentity();
}

Object::Object() {

}

