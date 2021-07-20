#pragma once
class Texture {
public:
	char filepath[200];
	char wrap[50];
	char filter_min[50];
	char filter_mag[50];
	int textureID;
	Texture();
	~Texture();
};