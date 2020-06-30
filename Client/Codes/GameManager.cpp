#include "stdafx.h"
#include "..\Headers\GameManager.h"
#include "Level.h"
#include "Management.h"
#include "StageUIMgr.h"
#include "Marine.h"
USING(Client)

IMPLEMENT_SINGLETON(CGameManager)

CGameManager::CGameManager()
{
}


_int CGameManager::Update()
{
	//웨이브 상태일때 체크
	if (m_bInWait)
	{
		CManagement* pManagement = CManagement::Get_Instance();
		if (nullptr == pManagement) return -1;
		CLayer* pUnitLayer = pManagement->Find_Layer(SCENE_STAGE1, L"Layer_Unit");
		//유닛이 다죽으면 다음 웨이브로 (건물 유지)
		if (nullptr == pUnitLayer) return 0;
		if (pUnitLayer->Get_ObjectList()->empty())
		{
			m_bInWait = true;
			SetWaveStart(false);
		}

		CLayer* pCommandLayer = pManagement->Find_Layer(SCENE_STAGE1, L"Layer_CommandCenter");
		if (nullptr == pCommandLayer) return 0;
		if (pCommandLayer->Get_ObjectList()->empty())
		{
			CStageUIMgr::Get_Instance()->Set_Active_GameOverPanel();

		}

	}

	return _int();
}

HRESULT CGameManager::OnGameStart()
{
	//웨이브시작
	//커멘드 센터가 없으면
	CManagement* pEngineMgr = CManagement::Get_Instance();
	if (nullptr == pEngineMgr) return -1;
	CLayer* pLayer = pEngineMgr->Find_Layer(SCENE_STAGE1, L"Layer_CommandCenter");
	if (nullptr == pLayer || pLayer->Get_ObjectList()->empty())
	{
		CStageUIMgr::Get_Instance()->Set_Active_GameOverPanel();
		//게임오버
		return -1;
	}

	if (0 == m_iWave)
	{
		CMarine::STATEDESC tMarineDesc;
		tMarineDesc.pTextureTag = L"Component_Texture_Cube";
		tMarineDesc.iTextureID = 0;
		tMarineDesc.eTextureSceneID = SCENE_STATIC;
		tMarineDesc.eSceneID = SCENE_STAGE1;
		tMarineDesc.tBaseDesc = BASEDESC(_float3(0.f, 0.5f, 0.f), _float3(1.f, 1.f, 1.f));
		if (FAILED(pEngineMgr->Add_Object_ToLayer(SCENE_STATIC, L"GameObject_Walker", SCENE_STAGE1, L"Layer_Unit", &tMarineDesc)))
			return E_FAIL;

		tMarineDesc.pTextureTag = L"Component_Texture_Cube";
		tMarineDesc.iTextureID = 0;
		tMarineDesc.eTextureSceneID = SCENE_STATIC;
		tMarineDesc.eSceneID = SCENE_STAGE1;
		tMarineDesc.tBaseDesc = BASEDESC(_float3(0.f, 0.5f, 10.f), _float3(1.f, 1.f, 1.f));
		if (FAILED(pEngineMgr->Add_Object_ToLayer(SCENE_STATIC, L"GameObject_Walker", SCENE_STAGE1, L"Layer_Unit", &tMarineDesc)))
			return E_FAIL;



		tMarineDesc.pTextureTag = L"Component_Texture_Cube";
		tMarineDesc.iTextureID = 0;
		tMarineDesc.eTextureSceneID = SCENE_STATIC;
		tMarineDesc.eSceneID = SCENE_STAGE1;
		tMarineDesc.tBaseDesc = BASEDESC(_float3(0.f, 0.5f, 20.f), _float3(1.f, 1.f, 1.f));
		if (FAILED(pEngineMgr->Add_Object_ToLayer(SCENE_STATIC, L"GameObject_Walker", SCENE_STAGE1, L"Layer_Unit", &tMarineDesc)))
			return E_FAIL;



		tMarineDesc.pTextureTag = L"Component_Texture_Cube";
		tMarineDesc.iTextureID = 0;
		tMarineDesc.eTextureSceneID = SCENE_STATIC;
		tMarineDesc.eSceneID = SCENE_STAGE1;
		tMarineDesc.tBaseDesc = BASEDESC(_float3(0.f, 0.5f, 30.f), _float3(1.f, 1.f, 1.f));
		if (FAILED(pEngineMgr->Add_Object_ToLayer(SCENE_STATIC, L"GameObject_Walker", SCENE_STAGE1, L"Layer_Unit", &tMarineDesc)))
			return E_FAIL;



		tMarineDesc.pTextureTag = L"Component_Texture_Cube";
		tMarineDesc.iTextureID = 0;
		tMarineDesc.eTextureSceneID = SCENE_STATIC;
		tMarineDesc.eSceneID = SCENE_STAGE1;
		tMarineDesc.tBaseDesc = BASEDESC(_float3(0.f, 0.5f, 40.f), _float3(1.f, 1.f, 1.f));
		if (FAILED(pEngineMgr->Add_Object_ToLayer(SCENE_STATIC, L"GameObject_Walker", SCENE_STAGE1, L"Layer_Unit", &tMarineDesc)))
			return E_FAIL;


		tMarineDesc.pTextureTag = L"Component_Texture_Cube";
		tMarineDesc.iTextureID = 0;
		tMarineDesc.eTextureSceneID = SCENE_STATIC;
		tMarineDesc.eSceneID = SCENE_STAGE1;
		tMarineDesc.tBaseDesc = BASEDESC(_float3(0.f, 0.5f, -40.f), _float3(1.f, 1.f, 1.f));
		if (FAILED(pEngineMgr->Add_Object_ToLayer(SCENE_STATIC, L"GameObject_Walker", SCENE_STAGE1, L"Layer_Unit", &tMarineDesc)))
			return E_FAIL;

		tMarineDesc.pTextureTag = L"Component_Texture_Cube";
		tMarineDesc.iTextureID = 0;
		tMarineDesc.eTextureSceneID = SCENE_STATIC;
		tMarineDesc.eSceneID = SCENE_STAGE1;
		tMarineDesc.tBaseDesc = BASEDESC(_float3(0.f, 0.5f, -30.f), _float3(1.f, 1.f, 1.f));
		if (FAILED(pEngineMgr->Add_Object_ToLayer(SCENE_STATIC, L"GameObject_Walker", SCENE_STAGE1, L"Layer_Unit", &tMarineDesc)))
			return E_FAIL;



		tMarineDesc.pTextureTag = L"Component_Texture_Cube";
		tMarineDesc.iTextureID = 0;
		tMarineDesc.eTextureSceneID = SCENE_STATIC;
		tMarineDesc.eSceneID = SCENE_STAGE1;
		tMarineDesc.tBaseDesc = BASEDESC(_float3(20.f, 0.5f, -20.f), _float3(1.f, 1.f, 1.f));
		if (FAILED(pEngineMgr->Add_Object_ToLayer(SCENE_STATIC, L"GameObject_Walker", SCENE_STAGE1, L"Layer_Unit", &tMarineDesc)))
			return E_FAIL;



		tMarineDesc.pTextureTag = L"Component_Texture_Cube";
		tMarineDesc.iTextureID = 0;
		tMarineDesc.eTextureSceneID = SCENE_STATIC;
		tMarineDesc.eSceneID = SCENE_STAGE1;
		tMarineDesc.tBaseDesc = BASEDESC(_float3(30.f, 0.5f, -10.f), _float3(1.f, 1.f, 1.f));
		if (FAILED(pEngineMgr->Add_Object_ToLayer(SCENE_STATIC, L"GameObject_Walker", SCENE_STAGE1, L"Layer_Unit", &tMarineDesc)))
			return E_FAIL;



		tMarineDesc.pTextureTag = L"Component_Texture_Cube";
		tMarineDesc.iTextureID = 0;
		tMarineDesc.eTextureSceneID = SCENE_STATIC;
		tMarineDesc.eSceneID = SCENE_STAGE1;
		tMarineDesc.tBaseDesc = BASEDESC(_float3(50.f, 0.5f, 0.f), _float3(1.f, 1.f, 1.f));
		if (FAILED(pEngineMgr->Add_Object_ToLayer(SCENE_STATIC, L"GameObject_Walker", SCENE_STAGE1, L"Layer_Unit", &tMarineDesc)))
			return E_FAIL;


		tMarineDesc.pTextureTag = L"Component_Texture_Cube";
		tMarineDesc.iTextureID = 0;
		tMarineDesc.eTextureSceneID = SCENE_STATIC;
		tMarineDesc.eSceneID = SCENE_STAGE1;
		tMarineDesc.tBaseDesc = BASEDESC(_float3(0.f, 0.5f, 10.f), _float3(1.f, 1.f, 1.f));
		if (FAILED(pEngineMgr->Add_Object_ToLayer(SCENE_STATIC, L"GameObject_Walker", SCENE_STAGE1, L"Layer_Unit", &tMarineDesc)))
			return E_FAIL;

		tMarineDesc.pTextureTag = L"Component_Texture_Cube";
		tMarineDesc.iTextureID = 0;
		tMarineDesc.eTextureSceneID = SCENE_STATIC;
		tMarineDesc.eSceneID = SCENE_STAGE1;
		tMarineDesc.tBaseDesc = BASEDESC(_float3(0.f, 0.5f, 20.f), _float3(1.f, 1.f, 1.f));
		if (FAILED(pEngineMgr->Add_Object_ToLayer(SCENE_STATIC, L"GameObject_Walker", SCENE_STAGE1, L"Layer_Unit", &tMarineDesc)))
			return E_FAIL;

		tMarineDesc.pTextureTag = L"Component_Texture_Cube";
		tMarineDesc.iTextureID = 0;
		tMarineDesc.eTextureSceneID = SCENE_STATIC;
		tMarineDesc.eSceneID = SCENE_STAGE1;
		tMarineDesc.tBaseDesc = BASEDESC(_float3(0.f, 0.5f, 30.f), _float3(1.f, 1.f, 1.f));
		if (FAILED(pEngineMgr->Add_Object_ToLayer(SCENE_STATIC, L"GameObject_Walker", SCENE_STAGE1, L"Layer_Unit", &tMarineDesc)))
			return E_FAIL;


		tMarineDesc.pTextureTag = L"Component_Texture_Cube";
		tMarineDesc.iTextureID = 0;
		tMarineDesc.eTextureSceneID = SCENE_STATIC;
		tMarineDesc.eSceneID = SCENE_STAGE1;
		tMarineDesc.tBaseDesc = BASEDESC(_float3(30.f, 0.5f, 50.f), _float3(1.f, 1.f, 1.f));
		if (FAILED(pEngineMgr->Add_Object_ToLayer(SCENE_STATIC, L"GameObject_Runner", SCENE_STAGE1, L"Layer_Unit", &tMarineDesc)))
			return E_FAIL;

	}
	else if (1 == m_iWave)
	{
		CMarine::STATEDESC tMarineDesc;
		tMarineDesc.pTextureTag = L"Component_Texture_Cube";
		tMarineDesc.iTextureID = 0;
		tMarineDesc.eTextureSceneID = SCENE_STATIC;
		tMarineDesc.eSceneID = SCENE_STAGE1;
		tMarineDesc.tBaseDesc = BASEDESC(_float3(0.f, 0.5f, 0.f), _float3(1.f, 1.f, 1.f));
		if (FAILED(pEngineMgr->Add_Object_ToLayer(SCENE_STATIC, L"GameObject_Walker", SCENE_STAGE1, L"Layer_Unit", &tMarineDesc)))
			return E_FAIL;

		tMarineDesc.pTextureTag = L"Component_Texture_Cube";
		tMarineDesc.iTextureID = 0;
		tMarineDesc.eTextureSceneID = SCENE_STATIC;
		tMarineDesc.eSceneID = SCENE_STAGE1;
		tMarineDesc.tBaseDesc = BASEDESC(_float3(10.f, 0.5f, 0.f), _float3(1.f, 1.f, 1.f));
		if (FAILED(pEngineMgr->Add_Object_ToLayer(SCENE_STATIC, L"GameObject_Walker", SCENE_STAGE1, L"Layer_Unit", &tMarineDesc)))
			return E_FAIL;



		tMarineDesc.pTextureTag = L"Component_Texture_Cube";
		tMarineDesc.iTextureID = 0;
		tMarineDesc.eTextureSceneID = SCENE_STATIC;
		tMarineDesc.eSceneID = SCENE_STAGE1;
		tMarineDesc.tBaseDesc = BASEDESC(_float3(20.f, 0.5f, 0.f), _float3(1.f, 1.f, 1.f));
		if (FAILED(pEngineMgr->Add_Object_ToLayer(SCENE_STATIC, L"GameObject_Walker", SCENE_STAGE1, L"Layer_Unit", &tMarineDesc)))
			return E_FAIL;



		tMarineDesc.pTextureTag = L"Component_Texture_Cube";
		tMarineDesc.iTextureID = 0;
		tMarineDesc.eTextureSceneID = SCENE_STATIC;
		tMarineDesc.eSceneID = SCENE_STAGE1;
		tMarineDesc.tBaseDesc = BASEDESC(_float3(30.f, 0.5f, 0.f), _float3(1.f, 1.f, 1.f));
		if (FAILED(pEngineMgr->Add_Object_ToLayer(SCENE_STATIC, L"GameObject_Walker", SCENE_STAGE1, L"Layer_Unit", &tMarineDesc)))
			return E_FAIL;



		tMarineDesc.pTextureTag = L"Component_Texture_Cube";
		tMarineDesc.iTextureID = 0;
		tMarineDesc.eTextureSceneID = SCENE_STATIC;
		tMarineDesc.eSceneID = SCENE_STAGE1;
		tMarineDesc.tBaseDesc = BASEDESC(_float3(40.f, 0.5f, 0.f), _float3(1.f, 1.f, 1.f));
		if (FAILED(pEngineMgr->Add_Object_ToLayer(SCENE_STATIC, L"GameObject_Walker", SCENE_STAGE1, L"Layer_Unit", &tMarineDesc)))
			return E_FAIL;


		tMarineDesc.pTextureTag = L"Component_Texture_Cube";
		tMarineDesc.iTextureID = 0;
		tMarineDesc.eTextureSceneID = SCENE_STATIC;
		tMarineDesc.eSceneID = SCENE_STAGE1;
		tMarineDesc.tBaseDesc = BASEDESC(_float3(0.f, 0.5f, 0.f), _float3(1.f, 1.f, 1.f));
		if (FAILED(pEngineMgr->Add_Object_ToLayer(SCENE_STATIC, L"GameObject_Walker", SCENE_STAGE1, L"Layer_Unit", &tMarineDesc)))
			return E_FAIL;

		tMarineDesc.pTextureTag = L"Component_Texture_Cube";
		tMarineDesc.iTextureID = 0;
		tMarineDesc.eTextureSceneID = SCENE_STATIC;
		tMarineDesc.eSceneID = SCENE_STAGE1;
		tMarineDesc.tBaseDesc = BASEDESC(_float3(10.f, 0.5f, 0.f), _float3(1.f, 1.f, 1.f));
		if (FAILED(pEngineMgr->Add_Object_ToLayer(SCENE_STATIC, L"GameObject_Walker", SCENE_STAGE1, L"Layer_Unit", &tMarineDesc)))
			return E_FAIL;



		tMarineDesc.pTextureTag = L"Component_Texture_Cube";
		tMarineDesc.iTextureID = 0;
		tMarineDesc.eTextureSceneID = SCENE_STATIC;
		tMarineDesc.eSceneID = SCENE_STAGE1;
		tMarineDesc.tBaseDesc = BASEDESC(_float3(20.f, 0.5f, 0.f), _float3(1.f, 1.f, 1.f));
		if (FAILED(pEngineMgr->Add_Object_ToLayer(SCENE_STATIC, L"GameObject_Walker", SCENE_STAGE1, L"Layer_Unit", &tMarineDesc)))
			return E_FAIL;



		tMarineDesc.pTextureTag = L"Component_Texture_Cube";
		tMarineDesc.iTextureID = 0;
		tMarineDesc.eTextureSceneID = SCENE_STATIC;
		tMarineDesc.eSceneID = SCENE_STAGE1;
		tMarineDesc.tBaseDesc = BASEDESC(_float3(30.f, 0.5f, 0.f), _float3(1.f, 1.f, 1.f));
		if (FAILED(pEngineMgr->Add_Object_ToLayer(SCENE_STATIC, L"GameObject_Walker", SCENE_STAGE1, L"Layer_Unit", &tMarineDesc)))
			return E_FAIL;



		tMarineDesc.pTextureTag = L"Component_Texture_Cube";
		tMarineDesc.iTextureID = 0;
		tMarineDesc.eTextureSceneID = SCENE_STATIC;
		tMarineDesc.eSceneID = SCENE_STAGE1;
		tMarineDesc.tBaseDesc = BASEDESC(_float3(50.f, 0.5f, 0.f), _float3(1.f, 1.f, 1.f));
		if (FAILED(pEngineMgr->Add_Object_ToLayer(SCENE_STATIC, L"GameObject_Walker", SCENE_STAGE1, L"Layer_Unit", &tMarineDesc)))
			return E_FAIL;


		tMarineDesc.pTextureTag = L"Component_Texture_Cube";
		tMarineDesc.iTextureID = 0;
		tMarineDesc.eTextureSceneID = SCENE_STATIC;
		tMarineDesc.eSceneID = SCENE_STAGE1;
		tMarineDesc.tBaseDesc = BASEDESC(_float3(0.f, 0.5f, 10.f), _float3(1.f, 1.f, 1.f));
		if (FAILED(pEngineMgr->Add_Object_ToLayer(SCENE_STATIC, L"GameObject_Walker", SCENE_STAGE1, L"Layer_Unit", &tMarineDesc)))
			return E_FAIL;

		tMarineDesc.pTextureTag = L"Component_Texture_Cube";
		tMarineDesc.iTextureID = 0;
		tMarineDesc.eTextureSceneID = SCENE_STATIC;
		tMarineDesc.eSceneID = SCENE_STAGE1;
		tMarineDesc.tBaseDesc = BASEDESC(_float3(0.f, 0.5f, 20.f), _float3(1.f, 1.f, 1.f));
		if (FAILED(pEngineMgr->Add_Object_ToLayer(SCENE_STATIC, L"GameObject_Walker", SCENE_STAGE1, L"Layer_Unit", &tMarineDesc)))
			return E_FAIL;

		tMarineDesc.pTextureTag = L"Component_Texture_Cube";
		tMarineDesc.iTextureID = 0;
		tMarineDesc.eTextureSceneID = SCENE_STATIC;
		tMarineDesc.eSceneID = SCENE_STAGE1;
		tMarineDesc.tBaseDesc = BASEDESC(_float3(0.f, 0.5f, 30.f), _float3(1.f, 1.f, 1.f));
		if (FAILED(pEngineMgr->Add_Object_ToLayer(SCENE_STATIC, L"GameObject_Walker", SCENE_STAGE1, L"Layer_Unit", &tMarineDesc)))
			return E_FAIL;


		tMarineDesc.pTextureTag = L"Component_Texture_Cube";
		tMarineDesc.iTextureID = 0;
		tMarineDesc.eTextureSceneID = SCENE_STATIC;
		tMarineDesc.eSceneID = SCENE_STAGE1;
		tMarineDesc.tBaseDesc = BASEDESC(_float3(60.f, 0.5f, 30.f), _float3(1.f, 1.f, 1.f));
		if (FAILED(pEngineMgr->Add_Object_ToLayer(SCENE_STATIC, L"GameObject_Runner", SCENE_STAGE1, L"Layer_Unit", &tMarineDesc)))
			return E_FAIL;
	}
	else if (2 == m_iWave)
	{
		CMarine::STATEDESC tMarineDesc;
		tMarineDesc.pTextureTag = L"Component_Texture_Cube";
		tMarineDesc.iTextureID = 0;
		tMarineDesc.eTextureSceneID = SCENE_STATIC;
		tMarineDesc.eSceneID = SCENE_STAGE1;
		tMarineDesc.tBaseDesc = BASEDESC(_float3(60.f, 0.5f, 30.f), _float3(1.f, 1.f, 1.f));
		if (FAILED(pEngineMgr->Add_Object_ToLayer(SCENE_STATIC, L"GameObject_Walker", SCENE_STAGE1, L"Layer_Unit", &tMarineDesc)))
			return E_FAIL;

		tMarineDesc.pTextureTag = L"Component_Texture_Cube";
		tMarineDesc.iTextureID = 0;
		tMarineDesc.eTextureSceneID = SCENE_STATIC;
		tMarineDesc.eSceneID = SCENE_STAGE1;
		tMarineDesc.tBaseDesc = BASEDESC(_float3(60.f, 0.5f, 40.f), _float3(1.f, 1.f, 1.f));
		if (FAILED(pEngineMgr->Add_Object_ToLayer(SCENE_STATIC, L"GameObject_Walker", SCENE_STAGE1, L"Layer_Unit", &tMarineDesc)))
			return E_FAIL;


		tMarineDesc.pTextureTag = L"Component_Texture_Cube";
		tMarineDesc.iTextureID = 0;
		tMarineDesc.eTextureSceneID = SCENE_STATIC;
		tMarineDesc.eSceneID = SCENE_STAGE1;
		tMarineDesc.tBaseDesc = BASEDESC(_float3(60.f, 0.5f, 0.f), _float3(1.f, 1.f, 1.f));
		if (FAILED(pEngineMgr->Add_Object_ToLayer(SCENE_STATIC, L"GameObject_Walker", SCENE_STAGE1, L"Layer_Unit", &tMarineDesc)))
			return E_FAIL;


		tMarineDesc.pTextureTag = L"Component_Texture_Cube";
		tMarineDesc.iTextureID = 0;
		tMarineDesc.eTextureSceneID = SCENE_STATIC;
		tMarineDesc.eSceneID = SCENE_STAGE1;
		tMarineDesc.tBaseDesc = BASEDESC(_float3(30.f, 0.5f, 0.f), _float3(1.f, 1.f, 1.f));
		if (FAILED(pEngineMgr->Add_Object_ToLayer(SCENE_STATIC, L"GameObject_Runner", SCENE_STAGE1, L"Layer_Unit", &tMarineDesc)))
			return E_FAIL;


		tMarineDesc.pTextureTag = L"Component_Texture_Cube";
		tMarineDesc.iTextureID = 0;
		tMarineDesc.eTextureSceneID = SCENE_STATIC;
		tMarineDesc.eSceneID = SCENE_STAGE1;
		tMarineDesc.tBaseDesc = BASEDESC(_float3(30.f, 0.5f, 60.f), _float3(1.f, 1.f, 1.f));
		if (FAILED(pEngineMgr->Add_Object_ToLayer(SCENE_STATIC, L"GameObject_Runner", SCENE_STAGE1, L"Layer_Unit", &tMarineDesc)))
			return E_FAIL;

		tMarineDesc.pTextureTag = L"Component_Texture_Cube";
		tMarineDesc.iTextureID = 0;
		tMarineDesc.eTextureSceneID = SCENE_STATIC;
		tMarineDesc.eSceneID = SCENE_STAGE1;
		tMarineDesc.tBaseDesc = BASEDESC(_float3(40.f, 0.5f, 60.f), _float3(1.f, 1.f, 1.f));
		if (FAILED(pEngineMgr->Add_Object_ToLayer(SCENE_STATIC, L"GameObject_Runner", SCENE_STAGE1, L"Layer_Unit", &tMarineDesc)))
			return E_FAIL;


		tMarineDesc.pTextureTag = L"Component_Texture_Cube";
		tMarineDesc.iTextureID = 0;
		tMarineDesc.eTextureSceneID = SCENE_STATIC;
		tMarineDesc.eSceneID = SCENE_STAGE1;
		tMarineDesc.tBaseDesc = BASEDESC(_float3(20.f, 0.5f, 60.f), _float3(1.f, 1.f, 1.f));
		if (FAILED(pEngineMgr->Add_Object_ToLayer(SCENE_STATIC, L"GameObject_Runner", SCENE_STAGE1, L"Layer_Unit", &tMarineDesc)))
			return E_FAIL;

		tMarineDesc.pTextureTag = L"Component_Texture_Cube";
		tMarineDesc.iTextureID = 0;
		tMarineDesc.eTextureSceneID = SCENE_STATIC;
		tMarineDesc.eSceneID = SCENE_STAGE1;
		tMarineDesc.tBaseDesc = BASEDESC(_float3(60.f, 0.5f, 10.f), _float3(1.f, 1.f, 1.f));
		if (FAILED(pEngineMgr->Add_Object_ToLayer(SCENE_STATIC, L"GameObject_Walker", SCENE_STAGE1, L"Layer_Unit", &tMarineDesc)))
			return E_FAIL;


		tMarineDesc.pTextureTag = L"Component_Texture_Cube";
		tMarineDesc.iTextureID = 0;
		tMarineDesc.eTextureSceneID = SCENE_STATIC;
		tMarineDesc.eSceneID = SCENE_STAGE1;
		tMarineDesc.tBaseDesc = BASEDESC(_float3(20.f, 0.5f, 0.f), _float3(1.f, 1.f, 1.f));
		if (FAILED(pEngineMgr->Add_Object_ToLayer(SCENE_STATIC, L"GameObject_Walker", SCENE_STAGE1, L"Layer_Unit", &tMarineDesc)))
			return E_FAIL;


		tMarineDesc.pTextureTag = L"Component_Texture_Cube";
		tMarineDesc.iTextureID = 0;
		tMarineDesc.eTextureSceneID = SCENE_STATIC;
		tMarineDesc.eSceneID = SCENE_STAGE1;
		tMarineDesc.tBaseDesc = BASEDESC(_float3(20.f, 0.5f, 50.f), _float3(1.f, 1.f, 1.f));
		if (FAILED(pEngineMgr->Add_Object_ToLayer(SCENE_STATIC, L"GameObject_Walker", SCENE_STAGE1, L"Layer_Unit", &tMarineDesc)))
			return E_FAIL;

		tMarineDesc.pTextureTag = L"Component_Texture_Cube";
		tMarineDesc.iTextureID = 0;
		tMarineDesc.eTextureSceneID = SCENE_STATIC;
		tMarineDesc.eSceneID = SCENE_STAGE1;
		tMarineDesc.tBaseDesc = BASEDESC(_float3(60.f, 0.5f, 60.f), _float3(1.f, 1.f, 1.f));
		if (FAILED(pEngineMgr->Add_Object_ToLayer(SCENE_STATIC, L"GameObject_Walker", SCENE_STAGE1, L"Layer_Unit", &tMarineDesc)))
			return E_FAIL;


		tMarineDesc.pTextureTag = L"Component_Texture_Cube";
		tMarineDesc.iTextureID = 0;
		tMarineDesc.eTextureSceneID = SCENE_STATIC;
		tMarineDesc.eSceneID = SCENE_STAGE1;
		tMarineDesc.tBaseDesc = BASEDESC(_float3(10.f, 0.5f, 30.f), _float3(1.f, 1.f, 1.f));
		if (FAILED(pEngineMgr->Add_Object_ToLayer(SCENE_STATIC, L"GameObject_Walker", SCENE_STAGE1, L"Layer_Unit", &tMarineDesc)))
			return E_FAIL;

	}


	return S_OK;
}

HRESULT CGameManager::OnGameEnd()
{
	return S_OK;
}

void CGameManager::Free()
{

}