#include <stdafx.h>
#include "Model.h"
//#include "Vertex.h"
#include <fstream>

void Model::LoadModel() {
	char fullpath[200];
	sprintf(fullpath, "../ResourcesPacket/");
	sprintf(fullpath + 19, "%s", filepath);
	FILE* f = fopen(fullpath, "r");
	if (f == NULL) {
		printf("Can't found file\n");
	}
	char buffer[1024];
	memset(buffer, 0, sizeof(buffer));
	fgets(buffer, sizeof(buffer), f);
	char type[20];
	sscanf(buffer, "%s %d", type, &NrVertices);
	verticesData = new Vertex[NrVertices + 1];
	int index = 0;
	while (fgets(buffer, sizeof(buffer), f)) {
		if (strncmp(buffer, "NrIndices", 9) == 0) {
			sscanf(buffer, "%s %d", type, &NrIndices);
			break;
		}
		else {
			float x, y, z;
			char* newBuffer = strstr(buffer, "pos");
			newBuffer += 5;
			sscanf(newBuffer, "%f %s %f %s %f %s", &x, type, &y, type, &z, type);
			verticesData[index].pos.x = x;
			verticesData[index].pos.y = y;
			verticesData[index].pos.z = z;
			/*newBuffer = strstr(newBuffer, "[");
			newBuffer += 1;
			sscanf(newBuffer, "%f %s %f %s %f %s", &x, type, &y, type, &z, type);
			verticesData[index].norm.x = x;
			verticesData[index].norm.y = y;
			verticesData[index].norm.z = z;
			newBuffer = strstr(newBuffer, "[");
			newBuffer += 1;
			sscanf(newBuffer, "%f %s %f %s %f %s", &x, type, &y, type, &z, type);
			verticesData[index].binorm.x = x;
			verticesData[index].binorm.y = y;
			verticesData[index].binorm.z = z;
			newBuffer = strstr(newBuffer, "[");
			newBuffer += 1;
			sscanf(newBuffer, "%f %s %f %s %f %s", &x, type, &y, type, &z, type);
			verticesData[index].tgt.x = x;
			verticesData[index].tgt.y = y;
			verticesData[index].tgt.z = z;*/
			newBuffer = strstr(newBuffer, "uv");
			newBuffer += 4;
			sscanf(newBuffer, "%f %s %f %s", &x, type, &y, type);
			verticesData[index].uv.x = x;
			verticesData[index].uv.y = y;
			index++;
		}
		memset(buffer, 0, sizeof(buffer));
	}
	indices = new unsigned int[NrIndices + 1];
	int indexInDices = 0;
	while (fgets(buffer, sizeof(buffer), f)) {
		int tmp = 0;
		unsigned int a, b, c;
		sscanf(buffer, "%d %s %d %s %d %s %d", &tmp, type, &a, type, &b, type, &c);
		indices[indexInDices] = a;
		indexInDices++;
		indices[indexInDices] = b;
		indexInDices++;
		indices[indexInDices] = c;
		indexInDices++;
		memset(buffer, 0, sizeof(buffer));
	}
	fclose(f);
}

Model::~Model() {
	delete[] verticesData;
	delete[] indices;
}