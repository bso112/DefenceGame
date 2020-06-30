#include "stdafx.h"
#include "StageUIMgr.h"
#include "MyButton.h"
#include "Management.h"
#include "GameManager.h"
IMPLEMENT_SINGLETON(CStageUIMgr)





HRESULT CStageUIMgr::Ready_StageUIMgr()
{

	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement) return E_FAIL;
	CMyButton::STATEDESC gameStartBtnDesc;
	CMyButton* pGameStartBtn = nullptr;
	gameStartBtnDesc.m_eSceneID = SCENE_STAGE1;
	gameStartBtnDesc.m_iTextureSceneID = SCENE_STATIC;
	gameStartBtnDesc.m_pTextureTag = L"Component_Texture_Btn";
	gameStartBtnDesc.m_tBaseDesc = BASEDESC(_float3(g_iWinSizeX - 200.f, g_iWinSizeY - 200.f, 1.f), _float3(100.f, 100.f, 1.f));
	if (nullptr == (pGameStartBtn = (CMyButton*)pManagement->Add_Object_ToLayer(SCENE_STATIC, L"GameObject_MyButton", SCENE_STAGE1, L"Layer_UI", &gameStartBtnDesc)))
		return E_FAIL;

	pGameStartBtn->Add_Listener([] { CGameManager::Get_Instance()->SetGameStart(true); });
	
	return S_OK;
}

void CStageUIMgr::Free()
{
}
