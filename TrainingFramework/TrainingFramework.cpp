// TrainingFramework.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "../Utilities/utilities.h" // if you use STL, please include this line AFTER all other include
#include "Vertex.h"
#include "Shaders.h"
#include "Globals.h"
#include "Model.h"
#include <conio.h>
#include <fstream>
#include "Object.h"

GLuint vboId;
GLuint iboID;
GLuint textureID;
GLint width;
GLint height;
GLint bpp;
Shaders myShaders;
Model* model = new Model();
Object* obj = new Object();


int Init ( ESContext *esContext )
{
	glClearColor ( 1.0f, 1.0f, 1.0f, 1.0f );
	//triangle data (heap)
	glEnable(GL_DEPTH_TEST);
	obj->InitWVP();
	char* filepath = "../ResourcesPacket/Models/Woman1.nfg";
	model->LoadModel(filepath);

	//buffer object
	glGenBuffers(1, &vboId);
	glBindBuffer(GL_ARRAY_BUFFER, vboId);
	glBufferData(GL_ARRAY_BUFFER, model->NrVertices*sizeof(Vertex), model->verticesData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glGenBuffers(1, &iboID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, model->NrIndices*sizeof(unsigned	int), model->indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	char* imageData = LoadTGA("../ResourcesPacket/Textures/Woman1.tga", &width, &height, &bpp);
	GLuint bppType = GL_RGB;
	if (bpp == 32) bppType = GL_RGBA;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, bppType, width, height, 0, bppType, GL_UNSIGNED_BYTE, imageData);
	delete imageData;
	//glGenerateMipmap(GL_TEXTURE_2D);
	//creation of shaders and program 
	glBindTexture(GL_TEXTURE_2D, 0); 
	//glEnable(GL_DEPTH_TEST);
	return myShaders.Init("../Resources/Shaders/TriangleShaderVS.vs", "../Resources/Shaders/TriangleShaderFS.fs");
}

void Draw ( ESContext *esContext )
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(myShaders.program);

	glBindBuffer(GL_ARRAY_BUFFER, vboId);
	glBindTexture(GL_TEXTURE_2D, textureID);
	GLuint iTextureLoc = glGetUniformLocation(myShaders.program, "u_texture");
	glUniform1i(iTextureLoc, 0);
	GLint MatrixID = glGetUniformLocation(myShaders.program, "u_WVP");
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &(obj->WVP.m[0][0]));
	if(myShaders.positionAttribute != -1)
	{
		glEnableVertexAttribArray(myShaders.positionAttribute);
		glVertexAttribPointer(myShaders.positionAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	}
	if (myShaders.uvAttribute != -1) {
		glEnableVertexAttribArray(myShaders.uvAttribute);
		glVertexAttribPointer(myShaders.uvAttribute, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3*sizeof(GLfloat)));
	}
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboID);
	//glDrawArrays(GL_TRIANGLES, 0, 3);
	glDrawElements(GL_TRIANGLES, model->NrIndices, GL_UNSIGNED_INT, (void*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	eglSwapBuffers ( esContext->eglDisplay, esContext->eglSurface );
}

void Update ( ESContext *esContext, float deltaTime )
{

}

void Key ( ESContext *esContext, unsigned char key, bool bIsPressed)
{

}

void CleanUp()
{
	glDeleteBuffers(1, &vboId);
	//model = NULL;
	model->~Model();
}

int _tmain(int argc, _TCHAR* argv[])
{
	ESContext esContext;

    esInitContext ( &esContext );

	esCreateWindow ( &esContext, "Triangle Texture", Globals::screenWidth, Globals::screenHeight, ES_WINDOW_RGB | ES_WINDOW_DEPTH);

	if ( Init ( &esContext ) != 0 )
		return 0;

	esRegisterDrawFunc ( &esContext, Draw );
	esRegisterUpdateFunc ( &esContext, Update );
	esRegisterKeyFunc ( &esContext, Key);

	esMainLoop ( &esContext );

	//releasing OpenGL resources
	CleanUp();

	//identifying memory leaks
	MemoryDump();
	printf("Press any key...\n");
	_getch();

	return 0;
}

