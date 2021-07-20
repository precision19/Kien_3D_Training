#pragma once
#include "../Utilities/utilities.h"
#include <vector>

class Shaders 
{
public:
	GLuint program, vertexShader, fragmentShader;
	char fileVS[260];
	char fileFS[260];
	GLint positionAttribute;
	GLint colorAttribute;
	GLint uvAttribute;
	int shaderID;
	std::vector<char*>states;
	std::vector<int>status; // status of states
	int Init(char * fileVertexShader, char * fileFragmentShader);
	~Shaders();
};