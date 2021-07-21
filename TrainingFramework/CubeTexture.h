#pragma once
class CubeTexture {
public:
	char filepath[200];
	char wrap[50];
	char filter_min[50];
	char filter_mag[50];
	char fileComponent[6][200];
	int cubeTextureID;
	CubeTexture();
	~CubeTexture();
};