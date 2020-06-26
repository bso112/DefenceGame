#pragma once

namespace Client
{
	const unsigned int g_iWinSizeX = 1280;
	const unsigned int g_iWinSizeY = 720;
	
	enum SCENEID { SCENE_STATIC, SCENE_LOADING, SCENE_MENU, SCENE_STAGE, SCENE_STAGE2, SCENE_STAGE3, SCENE_STAGE4, SCENE_END };

}	



using namespace Client;
extern HWND g_hWnd;

#include <functional>
#include <time.h>
#include "Client_Struct.h"

#define PASS_DEFAULT 0
#define PASS_WALL 1
#define PASS_GOAL 2