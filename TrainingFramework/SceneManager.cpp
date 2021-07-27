#include "stdafx.h"
#include "SceneManager.h"
#include "Object.h"
#include <map>

SceneManager* SceneManager::s_Instance = NULL;
SceneManager::SceneManager(void) {
	this->Init();
}

SceneManager::~SceneManager(void) {
	printf("%s\n", "Destructor SceneManager");
}

SceneManager* SceneManager::GetInstance() {
	if (!s_Instance) {
		s_Instance = new SceneManager();
	}
	return s_Instance;
}

void SceneManager::Render() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	for (int i = 0; i < obj.size(); i++) {
		glUseProgram(obj[i]->shader->program);
		glBindBuffer(GL_ARRAY_BUFFER, obj[i]->vboId);
		if (obj[i]->numTextures >= 1) {
			int n = obj[i]->numTextures;
			map<int, char[10]> tex;
			for (int j = 0; j < n; j++) {
				memset(tex[j], 0, sizeof(tex[j]));
				strcpy(tex[j], "u_texture");
				tex[j][9] = j + '0';
				tex[j][10] = '\0';
				//printf("%s\n", tex[j]);
			}
			GLuint *l = new GLuint[n];
			for (int j = 0; j < obj[i]->numTextures; j++) {
				glActiveTexture(GL_TEXTURE0 + j);
				glBindTexture(GL_TEXTURE_2D, obj[i]->textureID[j]);
				l[j] = glGetUniformLocation(obj[i]->shader->program, tex[j]);
				glUniform1i(l[j], j);
			}
			GLint lerp = glGetUniformLocation(obj[i]->shader->program, "lerp");
			if (lerp != -1) {
				Vector3 posCam = Camera::GetInstance()->pos;
				Vector3 posObj = obj[i]->position;
				float disToCamera = sqrt((posCam.x - posObj.x) * (posCam.x - posObj.x) + (posCam.y - posObj.y) * (posCam.y - posObj.y) + (posCam.z - posObj.z));
				float fog_start = obj[i]->fog_start;
				float fog_length = obj[i]->fog_length;
				glUniform1f(lerp, (disToCamera-fog_start)/fog_length);
			}
			GLint fogColor = glGetUniformLocation(obj[i]->shader->program, "fogColor");
			if (fogColor != -1) {
				Vector3 color = obj[i]->fog_color;
				glUniform4f(fogColor, color.x, color.y, color.z, 1.0);
			}
			delete[] l;
			//printf("%f %f\n", obj[i]->fog_start, obj[i]->fog_length);
		}
		if (obj[i]->numCubes > 0) {
			glEnable(GL_TEXTURE_CUBE_MAP);
			glBindTexture(GL_TEXTURE_CUBE_MAP, obj[i]->textureID[0]);
			GLuint iTextureLoc = glGetUniformLocation(obj[i]->shader->program, "u_samplerCubeMap");
			glUniform1i(iTextureLoc, 0);
		}

		GLint MatrixID = glGetUniformLocation(obj[i]->shader->program, "u_WVP");
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &(obj[i]->WVP.m[0][0]));
		if (obj[i]->shader->positionAttribute != -1)
		{
			glEnableVertexAttribArray(obj[i]->shader->positionAttribute);
			glVertexAttribPointer(obj[i]->shader->positionAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
		}
		if (obj[i]->shader->uvAttribute != -1) {	
			glEnableVertexAttribArray(obj[i]->shader->uvAttribute);
			glVertexAttribPointer(obj[i]->shader->uvAttribute, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(GLfloat)));
		}
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, obj[i]->iboID);
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		glDrawElements(GL_TRIANGLES, obj[i]->model->NrIndices, GL_UNSIGNED_INT, (void*)0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glDisable(GL_TEXTURE_2D);
	}
}

void SceneManager::Update(float deltaTime) {
	Camera::GetInstance()->Update(deltaTime);
	for (int i = 0; i < obj.size(); i++) {
		obj[i]->Update(deltaTime);
	}
	Camera::GetInstance()->isDirty = 0;
}


int SceneManager::InitShader() {
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	//triangle data (heap)
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	for (int i = 0; i < obj.size(); i++) {
		obj[i]->InitWVP();
		obj[i]->model->LoadModel();
		//buffer object
		glGenBuffers(1, &(obj[i]->vboId));
		glBindBuffer(GL_ARRAY_BUFFER, obj[i]->vboId);
		glBufferData(GL_ARRAY_BUFFER, obj[i]->model->NrVertices * sizeof(Vertex), obj[i]->model->verticesData, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glGenBuffers(1, &(obj[i]->iboID));
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, obj[i]->iboID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, obj[i]->model->NrIndices * sizeof(unsigned int), obj[i]->model->indices, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		if (obj[i]->textures.size() > 0) {
			for (int j = 0; j < obj[i]->textures.size(); j++) {
				//glActiveTexture(GL_TEXTURE0 + j);
				glGenTextures(1, &(obj[i]->textureID[j]));
				glBindTexture(GL_TEXTURE_2D, obj[i]->textureID[j]);
				char* imageData = LoadTGA(obj[i]->textures[j]->filepath, &width, &height, &bpp);
				GLuint bppType = GL_RGB;
				if (bpp == 32) bppType = GL_RGBA;
				if (strcmp(obj[i]->textures[j]->wrap, "REPEAT") == 0) {
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
				}
				if (strcmp(obj[i]->textures[j]->filter_min, "LINEAR") == 0) {
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				}
				else if (strcmp(obj[i]->textures[j]->filter_min, "NEAREST") == 0) {
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				}
				if (strcmp(obj[i]->textures[j]->filter_mag, "LINEAR") == 0) {
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				}
				else if (strcmp(obj[i]->textures[j]->filter_mag, "NEAREST") == 0) {
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				}
				glTexImage2D(GL_TEXTURE_2D, 0, bppType, width, height, 0, bppType, GL_UNSIGNED_BYTE, imageData);
				delete imageData;
				glBindTexture(GL_TEXTURE_2D, 0);
			}
			//creation of shaders and program 
		}
		if (obj[i]->numCubes > 0) {
			glGenTextures(1, &(obj[i]->textureID[0]));
			glBindTexture(GL_TEXTURE_CUBE_MAP, obj[i]->textureID[0]);
			char* cubePixels[6];
			for (int j = 0; j < 6; j++)
			{
				cubePixels[j] = LoadTGA(obj[i]->cube->fileComponent[j], &width, &height, &bpp);
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + j, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, cubePixels[j]);
			}
			if (strcmp(obj[i]->cube->wrap, "REPEAT") == 0) {
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_REPEAT);
			}
			else if (strcmp(obj[i]->cube->wrap, "CLAMP") == 0) {
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			}
			if (strcmp(obj[i]->cube->filter_min, "LINEAR") == 0) {
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			}
			else if (strcmp(obj[i]->cube->filter_min, "NEAREST") == 0) {
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			}
			if (strcmp(obj[i]->cube->filter_mag, "LINEAR") == 0) {
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			}
			else if (strcmp(obj[i]->cube->filter_mag, "NEAREST") == 0) {
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			}
			for (int j = 0; j < 6; j++) {
				delete cubePixels[j];
			}
			glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
			//obj[i]->cube->LoadCubeTexture();
		}
		obj[i]->shader->Init(obj[i]->shader->fileVS, obj[i]->shader->fileFS);
	}
	return 0;
}

void SceneManager::CleanUp() {
	for (int i = 0; i < obj.size(); i++) {
		glDeleteBuffers(1, &(obj[i]->vboId));
		glDeleteBuffers(1, &(obj[i]->iboID));
		delete obj[i];
	}
	delete s_Instance;
	delete ResourceManager::r_Instance;
	delete Camera::c_Instance;
}

void SceneManager::Key(unsigned char key, bool bIsPressed) {
	//printf("%c\n", key);
	if (key == 'W') {
		if (bIsPressed) {
			Camera::GetInstance()->keyPressed = Camera::GetInstance()->keyPressed | Camera::GetInstance()->w;
		}
		else {
			Camera::GetInstance()->keyPressed = Camera::GetInstance()->keyPressed ^ Camera::GetInstance()->w;
		}
	}
	if (key == 'A') {
		if (bIsPressed) {
			Camera::GetInstance()->keyPressed = Camera::GetInstance()->keyPressed | Camera::GetInstance()->a;
		}
		else {
			Camera::GetInstance()->keyPressed = Camera::GetInstance()->keyPressed ^ Camera::GetInstance()->a;
		}
	}
	if (key == 'S') {
		if (bIsPressed) {
			Camera::GetInstance()->keyPressed = Camera::GetInstance()->keyPressed | Camera::GetInstance()->s;
		}
		else {
			Camera::GetInstance()->keyPressed = Camera::GetInstance()->keyPressed ^ Camera::GetInstance()->s;
		}
	}
	if (key == 'D') {
		if (bIsPressed) {
			Camera::GetInstance()->keyPressed = Camera::GetInstance()->keyPressed | Camera::GetInstance()->d;
		}
		else {
			Camera::GetInstance()->keyPressed = Camera::GetInstance()->keyPressed ^ Camera::GetInstance()->d;
		}
	}
	if (key == VK_LEFT) {
		if (bIsPressed) {
			Camera::GetInstance()->keyPressed = Camera::GetInstance()->keyPressed | Camera::GetInstance()->leftKey;
		}
		else {
			Camera::GetInstance()->keyPressed = Camera::GetInstance()->keyPressed ^ Camera::GetInstance()->leftKey;
		}
	}
	if (key == VK_RIGHT) {
		if (bIsPressed) {
			Camera::GetInstance()->keyPressed = Camera::GetInstance()->keyPressed | Camera::GetInstance()->rightKey;
		}
		else {
			Camera::GetInstance()->keyPressed = Camera::GetInstance()->keyPressed ^ Camera::GetInstance()->rightKey;
		}
	}
	if (key == VK_UP) {
		if (bIsPressed) {
			Camera::GetInstance()->keyPressed = Camera::GetInstance()->keyPressed | Camera::GetInstance()->upKey;
		}
		else {
			Camera::GetInstance()->keyPressed = Camera::GetInstance()->keyPressed ^ Camera::GetInstance()->upKey;
		}
	}
	if (key == VK_DOWN) {
		if (bIsPressed) {
			Camera::GetInstance()->keyPressed = Camera::GetInstance()->keyPressed | Camera::GetInstance()->downKey;
		}
		else {
			Camera::GetInstance()->keyPressed = Camera::GetInstance()->keyPressed ^ Camera::GetInstance()->downKey;
		}
	}
}


void SceneManager::Init() {
	FILE* f;
	f = fopen("../Resources/SM.txt", "r+");
	char buffer[1024];
	memset(buffer, 0, sizeof(buffer));
	if (!f) {
		printf("Can't find file\n");
	}
	else {
		int numCamera;
		fscanf(f, "#Cameras: %d\n", &numCamera);
		while (numCamera--) {
			int cameraID;
			fscanf(f, "ID %d\n", &cameraID);
			fscanf(f, "POSITION %f %f %f\n", &(Camera::GetInstance()->pos.x), &(Camera::GetInstance()->pos.y), &(Camera::GetInstance()->pos.z));
			fscanf(f, "TARGET %f %f %f\n", &(Camera::GetInstance()->target.x), &(Camera::GetInstance()->target.y), &(Camera::GetInstance()->target.z));
			fscanf(f, "UP %f %f %f\n", &(Camera::GetInstance()->up.x), &(Camera::GetInstance()->up.y), &(Camera::GetInstance()->up.z));
			fscanf(f, "FOVY %f\n", &(Camera::GetInstance()->m_FOV));
			fscanf(f, "NEAR %f\n", &(Camera::GetInstance()->m_Near));
			fscanf(f, "FAR %f\n", &(Camera::GetInstance()->m_Far));
			fscanf(f, "MOVE_SPEED %f\n", &(Camera::GetInstance()->speedCamera));
			fscanf(f, "ROTATE_SPEED %f\n", &(Camera::GetInstance()->speedRotation));
		}
		int numObj;
		fscanf(f, "#Objects: %d\n", &numObj);
		int ID = 0; char tmp[20];
		int index = 0;
		while (numObj--) {
			obj.push_back(new Object());
			fscanf(f, "ID %d %s\n", &obj[index]->objectID, tmp);
			strcpy(obj[index]->type, tmp);
			fscanf(f, "MODEL %d\n", &obj[index]->modelID);
			fscanf(f, "TEXTURES %d\n", &obj[index]->numTextures);
			for (int i = 0; i < obj[index]->numTextures; i++) {
				int indexTexture;
				fscanf(f, "TEXTURE %d\n", &indexTexture);
				obj[index]->texturedID.push_back(indexTexture);
			}
			fscanf(f, "CUBETEXTURES %d\n", &obj[index]->numCubes);
			fscanf(f, "SHADER %d\n", &obj[index]->shaderID);
			fscanf(f, "LIGHTS %d\n", &obj[index]->numLights);
			for (int i = 0; i < obj[index]->numLights; i++) {
				int indexLight;
				fscanf(f, "LIGHT %d\n", &indexLight);
				obj[index]->lightID.push_back(indexLight);
			}
			fscanf(f, "POSITION %f %f %f\n", &(obj[index]->position.x), &(obj[index]->position.y), &(obj[index]->position.z));
			fscanf(f, "ROTATION %f %f %f\n", &(obj[index]->rotation.x), &(obj[index]->rotation.y), &(obj[index]->rotation.z));
			fscanf(f, "SCALE %f %f %f\n", &(obj[index]->scale.x), &(obj[index]->scale.y), &(obj[index]->scale.z));
			if (strcmp(obj[index]->type, "Terrain") == 0) {
				fscanf(f, "TILING %d\n", &obj[index]->tiling);
				fscanf(f, "FOG_START %f\n", &obj[index]->fog_start);
				fscanf(f, "FOG_LENGTH %f\n", &obj[index]->fog_length);
				fscanf(f, "FOG_COLOR %f %f %f\n", &obj[index]->fog_color.x, &obj[index]->fog_color.y, &obj[index]->fog_color.z);
				printf("%f %f\n", obj[index]->fog_start, obj[index]->fog_length);
			}
			index++;
		}
	}
	for (int i = 0; i < obj.size(); i++) {
		obj[i]->model = ResourceManager::GetInstance()->model[obj[i]->modelID];
		obj[i]->shader = ResourceManager::GetInstance()->shader[obj[i]->shaderID];
		for (int j = 0; j < obj[i]->texturedID.size(); j++) {
			obj[i]->textures.push_back(ResourceManager::GetInstance()->texture[obj[i]->texturedID[j]]);
		}
		if (obj[i]->numCubes > 0) {
			obj[i]->cube = ResourceManager::GetInstance()->cube;
			printf(obj[i]->cube->wrap);
		}
	}

}