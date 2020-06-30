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
	gameStartBtnDesc.m_eSceneID = SCENE_STAGE1;
	gameStartBtnDesc.m_iTextureSceneID = SCENE_STATIC;
	gameStartBtnDesc.m_pTextureTag = L"Component_Texture_Btn";
	gameStartBtnDesc.m_tBaseDesc = BASEDESC(_float3(g_iWinSizeX - 200.f, g_iWinSizeY - 200.f, 0.f), _float3(100.f, 100.f, 1.f));
	if (nullptr == (m_pGameStartBtn = (CMyButton*)pManagement->Add_Object_ToLayer(SCENE_STATIC, L"GameObject_MyButton", SCENE_STAGE1, L"Layer_UI", &gameStartBtnDesc)))
		return E_FAIL;

	m_pGameStartBtn->Add_Listener([] { CGameManager::Get_Instance()->SetGameStart(true); });
	
	return S_OK;
}

vector<RECT> CStageUIMgr::Get_UIRect()
{
	vector<RECT> vecRc;
	if (nullptr == m_pGameStartBtn)
		return vector<RECT>();

	CTransform* pTransform = (CTransform*)m_pGameStartBtn->Find_Component(L"Com_Transform");
	vecRc.push_back(pTransform->Get_Rect());
	return vecRc;
}

void CStageUIMgr::Free()
{
}
