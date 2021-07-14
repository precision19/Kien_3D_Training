#pragma once
#include "Vertex.h"
class Model {
public:
	int NrVertices;
	int NrIndices;
	Vertex* verticesData;
	unsigned int* indices;
	void LoadModel(char* filepath);
	~Model();
};