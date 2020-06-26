#include "stdafx.h"
#include "..\Headers\Level.h"

#include "Management.h"
#include "Cube.h"

CLevel::CLevel()
{
}

HRESULT CLevel::Ready_Level(_uint Level)
{
	switch (Level)
	{
	case 0:
		return Ready_Level_One();
	case 1:
		return Ready_Level_Two();
	case 2:
		return Ready_Level_Tree();
	case 3:
		return Ready_Level_Four();
	}

	return S_OK;
}

CCube * CLevel::PickBrick(POINT pt)
{
	return nullptr;
}

HRESULT CLevel::Ready_Level_One()
{




	return S_OK;
}

HRESULT CLevel::Ready_Level_Two()
{



	return S_OK;
}

HRESULT CLevel::Ready_Level_Tree()
{


	return S_OK;
}

HRESULT CLevel::Ready_Level_Four()
{

	return S_OK;
}

CLevel * CLevel::Create(_uint Level)
{
	CLevel* pInstance = new CLevel;
	if (FAILED(pInstance->Ready_Level(Level)))
	{
		MSG_BOX("Failed To Create Level");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel::Free()
{
	for (auto& Brick : m_Bricks)
		Safe_Release(Brick);

	m_Bricks.clear();
}
