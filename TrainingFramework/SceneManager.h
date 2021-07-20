#pragma once
#include <vector>
#include "Object.h"
#include "ResourceManager.h"
using namespace std;
class SceneManager {
public:
	GLint height, width, bpp;
	static SceneManager* s_Instance;
	static SceneManager* GetInstance();
	//Constructor
	SceneManager(void);
	//Destructor
	~SceneManager(void);
	vector<Object*>obj;
	virtual void Render();
	void Init();
	int InitShader();
	void Key(unsigned char, bool);
	void Update(float);
	void CleanUp();
};