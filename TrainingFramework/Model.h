#pragma once
#include "Vertex.h"
class Model {
public:
	float scale;
	int offset;
	int modelID;
	char filepath[200];
	int NrVertices;
	int NrIndices;
	Vertex* verticesData;
	unsigned int* indices;
	void LoadModel();
	~Model();
};