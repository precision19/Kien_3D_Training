#pragma once
#include <vector>
#include "Model.h"
#include "Texture.h"
#include "Shaders.h"
class ResourceManager {
public:
	static ResourceManager* r_Instance;
	static ResourceManager* GetInstance();
	std::vector<Model*>model;
	std::vector<Texture*>texture;
	std::vector<Shaders*>shader;
	void Init();
	//Constructor
	ResourceManager();
	//Destructor
	~ResourceManager();
};