#include "stdafx.h"
#include "..\Headers\GameManager.h"
#include "Level.h"

USING(Client)

IMPLEMENT_SINGLETON(CGameManager)

CGameManager::CGameManager()
{
}


HRESULT CGameManager::OnGameStart()
{

	return S_OK;
}

HRESULT CGameManager::OnGameEnd()
{
	return S_OK;
}

void CGameManager::Free()
{

}