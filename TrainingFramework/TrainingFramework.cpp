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
#include "SceneManager.h"
#include "ResourceManager.h"

int Init ( ESContext *esContext )
{
	return SceneManager::GetInstance()->InitShader();
}

void Draw ( ESContext *esContext )
{
	SceneManager::GetInstance()->Render();
	eglSwapBuffers ( esContext->eglDisplay, esContext->eglSurface );
}

void Update ( ESContext *esContext, float deltaTime )
{
	SceneManager::GetInstance()->Update(deltaTime);
}

void Key ( ESContext *esContext, unsigned char key, bool bIsPressed)
{
	SceneManager::GetInstance()->Key(key, bIsPressed);
}

void CleanUp()
{
	//printf("%s\n", "bac");
	//model = NULL;
	/*for (int i = 0; i < scenemanager::getinstance()->obj.size(); i++) {
		scenemanager::getinstance()->obj[i]->model->~model();
	}*/
	SceneManager::GetInstance()->CleanUp();
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

