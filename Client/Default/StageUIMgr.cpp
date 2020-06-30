#include "stdafx.h"
#include "StageUIMgr.h"
#include "MyButton.h"
#include "MyImage.h"
#include "Management.h"
#include "GameManager.h"
#include "PickingMgr.h"
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
	CPickingMgr::Get_Instance()->Add_UI(m_pGameStartBtn, CPickingMgr::UI_NORMAL_ONLY);


	//바리케이트 설치 모드 버튼
	if (nullptr == pManagement) return E_FAIL;
	gameStartBtnDesc;
	gameStartBtnDesc.m_eSceneID = SCENE_STAGE1;
	gameStartBtnDesc.m_iTextureSceneID = SCENE_STATIC;
	gameStartBtnDesc.m_pTextureTag = L"Component_Texture_Barricade";
	gameStartBtnDesc.m_tBaseDesc = BASEDESC(_float3(50.f, g_iWinSizeY - 50.f, 0.f), _float3(100.f, 100.f, 1.f));
	if (nullptr == (m_pBuildBarricadeBtn = (CMyButton*)pManagement->Add_Object_ToLayer(SCENE_STATIC, L"GameObject_MyButton", SCENE_STAGE1, L"Layer_UI", &gameStartBtnDesc)))
		return E_FAIL;
	m_pBuildBarricadeBtn->Add_Listener([] { CPickingMgr::Get_Instance()->Set_Mode(CPickingMgr::MODE_BARRICADE); });
	CPickingMgr::Get_Instance()->Add_UI(m_pBuildBarricadeBtn, CPickingMgr::UI_NORMAL_ONLY);


	//본진 설치 모드 버튼
	if (nullptr == pManagement) return E_FAIL;
	gameStartBtnDesc;
	gameStartBtnDesc.m_eSceneID = SCENE_STAGE1;
	gameStartBtnDesc.m_iTextureSceneID = SCENE_STATIC;
	gameStartBtnDesc.m_pTextureTag = L"Component_Texture_CommandCenter";
	gameStartBtnDesc.m_tBaseDesc = BASEDESC(_float3(150.f, g_iWinSizeY - 50.f, 0.f), _float3(100.f, 100.f, 1.f));
	if (nullptr == (m_pBuildCommandCenterBtn = (CMyButton*)pManagement->Add_Object_ToLayer(SCENE_STATIC, L"GameObject_MyButton", SCENE_STAGE1, L"Layer_UI", &gameStartBtnDesc)))
		return E_FAIL;
	m_pBuildCommandCenterBtn->Add_Listener([] { CPickingMgr::Get_Instance()->Set_Mode(CPickingMgr::MONE_COMMANDCENTER); });
	CPickingMgr::Get_Instance()->Add_UI(m_pBuildCommandCenterBtn, CPickingMgr::UI_NORMAL_ONLY);


	//유닛1 생성 모드 버튼
	if (nullptr == pManagement) return E_FAIL;
	gameStartBtnDesc;
	gameStartBtnDesc.m_eSceneID = SCENE_STAGE1;
	gameStartBtnDesc.m_iTextureSceneID = SCENE_STATIC;
	gameStartBtnDesc.m_pTextureTag = L"Component_Texture_Unit1";
	gameStartBtnDesc.m_tBaseDesc = BASEDESC(_float3(250.f, g_iWinSizeY - 50.f, 0.f), _float3(100.f, 100.f, 1.f));
	if (nullptr == (m_pCreateUnit1Btn = (CMyButton*)pManagement->Add_Object_ToLayer(SCENE_STATIC, L"GameObject_MyButton", SCENE_STAGE1, L"Layer_UI", &gameStartBtnDesc)))
		return E_FAIL;
	m_pCreateUnit1Btn->Add_Listener([] { CPickingMgr::Get_Instance()->Set_Mode(CPickingMgr::MONE_COMMANDCENTER); });
	CPickingMgr::Get_Instance()->Add_UI(m_pCreateUnit1Btn, CPickingMgr::UI_NORMAL_ONLY);


	//유닛2 생성 모드 버튼
	if (nullptr == pManagement) return E_FAIL;
	gameStartBtnDesc;
	gameStartBtnDesc.m_eSceneID = SCENE_STAGE1;
	gameStartBtnDesc.m_iTextureSceneID = SCENE_STATIC;
	gameStartBtnDesc.m_pTextureTag = L"Component_Texture_Unit2";
	gameStartBtnDesc.m_tBaseDesc = BASEDESC(_float3(350.f, g_iWinSizeY - 50.f, 0.f), _float3(100.f, 100.f, 1.f));
	if (nullptr == (m_pCreateUnit2Btn = (CMyButton*)pManagement->Add_Object_ToLayer(SCENE_STATIC, L"GameObject_MyButton", SCENE_STAGE1, L"Layer_UI", &gameStartBtnDesc)))
		return E_FAIL;
	m_pCreateUnit2Btn->Add_Listener([] { CPickingMgr::Get_Instance()->Set_Mode(CPickingMgr::MONE_COMMANDCENTER); });
	CPickingMgr::Get_Instance()->Add_UI(m_pCreateUnit2Btn, CPickingMgr::UI_NORMAL_ONLY);


	//유닛3 생성 모드 버튼
	if (nullptr == pManagement) return E_FAIL;
	gameStartBtnDesc;
	gameStartBtnDesc.m_eSceneID = SCENE_STAGE1;
	gameStartBtnDesc.m_iTextureSceneID = SCENE_STATIC;
	gameStartBtnDesc.m_pTextureTag = L"Component_Texture_Unit3";
	gameStartBtnDesc.m_tBaseDesc = BASEDESC(_float3(450.f, g_iWinSizeY - 50.f, 0.f), _float3(100.f, 100.f, 1.f));
	if (nullptr == (m_pCreateUnit3Btn = (CMyButton*)pManagement->Add_Object_ToLayer(SCENE_STATIC, L"GameObject_MyButton", SCENE_STAGE1, L"Layer_UI", &gameStartBtnDesc)))
		return E_FAIL;
	m_pCreateUnit3Btn->Add_Listener([] { CPickingMgr::Get_Instance()->Set_Mode(CPickingMgr::MONE_COMMANDCENTER); });
	CPickingMgr::Get_Instance()->Add_UI(m_pCreateUnit3Btn, CPickingMgr::UI_NORMAL_ONLY);


	//유닛 및 건물 재판매 버튼
	if (nullptr == pManagement) return E_FAIL;
	gameStartBtnDesc;
	gameStartBtnDesc.m_eSceneID = SCENE_STAGE1;
	gameStartBtnDesc.m_iTextureSceneID = SCENE_STATIC;
	gameStartBtnDesc.m_pTextureTag = L"Component_Texture_Resell";
	gameStartBtnDesc.m_tBaseDesc = BASEDESC(_float3(g_iWinSizeX - 50.f, 400.f, 0.f), _float3(100.f, 100.f, 1.f));
	if (nullptr == (m_pResellBtn = (CMyButton*)pManagement->Add_Object_ToLayer(SCENE_STATIC, L"GameObject_MyButton", SCENE_STAGE1, L"Layer_UI", &gameStartBtnDesc)))
		return E_FAIL;
	m_pResellBtn->Add_Listener([] { CPickingMgr::Get_Instance()->Set_Mode(CPickingMgr::MONE_COMMANDCENTER); });
	CPickingMgr::Get_Instance()->Add_UI(m_pResellBtn, CPickingMgr::UI_INTERACT_ONLY);




	//이미지///////////////////////////////////////////////

	//웨이브 표시
	if (nullptr == pManagement) return E_FAIL;
	CMyImage::STATEDESC	gameImgDesc;
	gameImgDesc.m_eSceneID = SCENE_STAGE1;
	gameImgDesc.m_iTextureSceneID = SCENE_STATIC;
	gameImgDesc.m_pTextureTag = L"Component_Texture_Wave";
	gameImgDesc.m_tBaseDesc = BASEDESC(_float3(g_iWinSizeX*0.5f, 75.f, 0.f), _float3(150.f, 150.f, 1.f));
	if (nullptr == (m_pWaveIndicator = (CMyImage*)pManagement->Add_Object_ToLayer(SCENE_STATIC, L"GameObject_MyImage", SCENE_STAGE1, L"Layer_UI", &gameImgDesc)))
		return E_FAIL;
	CPickingMgr::Get_Instance()->Add_UI(m_pWaveIndicator, CPickingMgr::UI_ALWAYS);


	//돈
	if (nullptr == pManagement) return E_FAIL;
	gameImgDesc;
	gameImgDesc.m_eSceneID = SCENE_STAGE1;
	gameImgDesc.m_iTextureSceneID = SCENE_STATIC;
	gameImgDesc.m_pTextureTag = L"Component_Texture_Gold";
	gameImgDesc.m_tBaseDesc = BASEDESC(_float3(50, g_iWinSizeY - 200.f, 0.f), _float3(100.f, 100.f, 1.f));
	if (nullptr == (m_pGoldIndicator = (CMyImage*)pManagement->Add_Object_ToLayer(SCENE_STATIC, L"GameObject_MyImage", SCENE_STAGE1, L"Layer_UI", &gameImgDesc)))
		return E_FAIL;
	CPickingMgr::Get_Instance()->Add_UI(m_pGoldIndicator, CPickingMgr::UI_ALWAYS);


	//스텟창
	if (nullptr == pManagement) return E_FAIL;
	gameImgDesc;
	gameImgDesc.m_eSceneID = SCENE_STAGE1;
	gameImgDesc.m_iTextureSceneID = SCENE_STATIC;
	gameImgDesc.m_pTextureTag = L"Component_Texture_StatsUI";
	gameImgDesc.m_tBaseDesc = BASEDESC(_float3(g_iWinSizeX - 150, 150.f, 0.f), _float3(300.f, 300.f, 1.f));
	if (nullptr == (m_pStatsUI = (CMyImage*)pManagement->Add_Object_ToLayer(SCENE_STATIC, L"GameObject_MyImage", SCENE_STAGE1, L"Layer_UI", &gameImgDesc)))
		return E_FAIL;
	CPickingMgr::Get_Instance()->Add_UI(m_pStatsUI, CPickingMgr::UI_INTERACT_ONLY);


	return S_OK;
}

vector<RECT> CStageUIMgr::Get_UIRect()
{
	vector<RECT> vecRc;
	if (nullptr == m_pGameStartBtn)
		return vector<RECT>();

	CTransform* pTransform = (CTransform*)m_pGameStartBtn->Find_Component(L"Com_Transform");
	vecRc.push_back(pTransform->Get_Rect());

	pTransform = (CTransform*)m_pBuildBarricadeBtn->Find_Component(L"Com_Transform");
	vecRc.push_back(pTransform->Get_Rect());

	pTransform = (CTransform*)m_pBuildCommandCenterBtn->Find_Component(L"Com_Transform");
	vecRc.push_back(pTransform->Get_Rect());

	pTransform = (CTransform*)m_pCreateUnit1Btn->Find_Component(L"Com_Transform");
	vecRc.push_back(pTransform->Get_Rect());

	pTransform = (CTransform*)m_pCreateUnit2Btn->Find_Component(L"Com_Transform");
	vecRc.push_back(pTransform->Get_Rect());

	pTransform = (CTransform*)m_pCreateUnit3Btn->Find_Component(L"Com_Transform");
	vecRc.push_back(pTransform->Get_Rect());

	pTransform = (CTransform*)m_pResellBtn->Find_Component(L"Com_Transform");
	vecRc.push_back(pTransform->Get_Rect());

	return vecRc;
}

void CStageUIMgr::Set_StateToPanel(CUnit::UNITSTATS tagStats)
{
	CPickingMgr::Get_Instance()->Set_Mode(CPickingMgr::MODE_UNIT_INTERACT);
}

void CStageUIMgr::Set_StateToPanel_Building(CBuilding::BUILDING_STAT tagStats_building)
{
	CPickingMgr::Get_Instance()->Set_Mode(CPickingMgr::MODE_UNIT_INTERACT);
}

void CStageUIMgr::Free()
{
}
