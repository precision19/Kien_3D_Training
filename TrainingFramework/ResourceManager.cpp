#include "stdafx.h"
#include "ResourceManager.h"

ResourceManager* ResourceManager::r_Instance = NULL;
ResourceManager* ResourceManager::GetInstance() {
	if (r_Instance == NULL) {
		r_Instance = new ResourceManager();
	}
	return r_Instance;
}

ResourceManager::ResourceManager(void) {
	this->Init();
}

ResourceManager::~ResourceManager(void) {
	printf("%s\n", "Destructor ResourceManager");
	for (int i = 0; i < texture.size(); i++) {
		delete texture[i];
	}
	for (int i = 0; i < shader.size(); i++) {
		delete shader[i];
	}
	delete cube;
}

void ResourceManager::Init() {
	FILE* f;
	f = fopen("../Resources/RM.txt", "r+");
	if (f == NULL) {
		printf("Can't open file resources\n");
	}
	else {
		int numModels;
		fscanf(f, "#Models: %d\n", &numModels);
		for(int i=0; i<numModels; i++){
			model.push_back(new Model());
			fscanf(f, "ID %d\n", &(model[i]->modelID));
			char filepath[1024];
			memset(filepath, 0, sizeof(filepath));
			fscanf(f, "FILE %s\n", filepath);
			strcpy(model[i]->filepath, filepath);
			/*if (strstr(filepath, "heightmap") != NULL) {
				fscanf(f, "SCALE %f\n", &model[i]->scale);
				fscanf(f, "OFFSET %d\n", &model[i]->offset);
			}*/
		}
		int numTextures;
		fscanf(f, "#2D Textures: %d\n", &numTextures);
		for(int i=0; i<numTextures; i++) {
			texture.push_back(new Texture());
			fscanf(f, "ID %d\n", &(texture[i]->textureID));
			//filepath textures
			char filepath[1024];
			memset(filepath, 0, sizeof(filepath));
			fscanf(f, "FILE %s\n", filepath);
			strcpy(texture[i]->filepath, filepath);
			//wrap textures
			char wrap[100];
			memset(wrap, 0, sizeof(wrap));
			fscanf(f, "WRAP %s\n", wrap);
			strcpy(texture[i]->wrap, wrap);
			//filter texures
			char filter_min[100], filter_mag[100];
			memset(filter_min, 0, sizeof(filter_min));
			memset(filter_mag, 0, sizeof(filter_mag));
			fscanf(f, "FILTER %s %s\n", filter_min, filter_mag);
			strcpy(texture[i]->filter_min, filter_min);
			strcpy(texture[i]->filter_mag, filter_mag);
		}

		int numCubes;
		fscanf(f, "#Cube Textures: %d\n", &numCubes);
		cube = new CubeTexture();
		fscanf(f, "ID %d\n", &(cube->cubeTextureID));
		//filepath textures
		char filepath[1024];
		memset(filepath, 0, sizeof(filepath));
		fscanf(f, "FILE %s\n", filepath);
		strcpy(cube->filepath, filepath);
		for (int i = 0; i < 6; i++) {
			char component[1024];
			memset(component, 0, sizeof(component));
			fscanf(f, "COMPONENT %s\n", component);
			strcpy(cube->fileComponent[i], component);
		}
		//wrap textures
		char wrap[100];
		memset(wrap, 0, sizeof(wrap));
		fscanf(f, "WRAP %s\n", wrap);
		strcpy(cube->wrap, wrap);
		//filter texures
		char filter_min[100], filter_mag[100];
		memset(filter_min, 0, sizeof(filter_min));
		memset(filter_mag, 0, sizeof(filter_mag));
		fscanf(f, "FILTER %s %s\n", filter_min, filter_mag);
		strcpy(cube->filter_min, filter_min);
		strcpy(cube->filter_mag, filter_mag);

		int numShaders;
		fscanf(f, "#Shaders: %d\n", &numShaders);
		for (int i = 0; i < numShaders; i++) {
			shader.push_back(new Shaders());
			fscanf(f, "ID %d\n", &(shader[i]->shaderID));
			char fileVS[260];
			char fileFS[260];
			memset(fileVS, sizeof(fileVS), 0);
			memset(fileFS, sizeof(fileFS), 0);
			fscanf(f, "VS %s\n", fileVS);
			fscanf(f, "FS %s\n", fileFS);
			strcpy(shader[i]->fileVS, fileVS);
			strcpy(shader[i]->fileFS, fileFS);
			int numStates;
			fscanf(f, "STATES %d\n", &numStates);
			for (int j = 0; j < numStates; j++) {
				char nameStates[100];
				memset(nameStates, 0, sizeof(nameStates));
				int statusStates;
				fscanf(f, "%s %d\n", nameStates, &statusStates);
				if(strcmp(nameStates, "GL_DEPTH_TEST") == 0)
					(shader[i]->states).push_back("GL_DEPTH_TEST");
				if (strcmp(nameStates, "GL_CULL_FACE") == 0)
					(shader[i]->states).push_back("GL_CULL_FACE");
				(shader[i]->status).push_back(statusStates);
				printf("%s\n", nameStates);
			}
			for (int j = 0; j < numStates; j++) {
				printf("%s\n", shader[i]->states[j]);
			}
		}
	}
}