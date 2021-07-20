#pragma once
#include "Vertex.h"
class Model {
public:
	int modelID;
	char filepath[200];
	int NrVertices;
	int NrIndices;
	Vertex* verticesData;
	unsigned int* indices;
	void LoadModel();
	~Model();
};