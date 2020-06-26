#include "stdafx.h"
#include "..\Headers\Scene_Loading.h"
#include "Management.h"

#include "Scene_Stage.h"
#include "Scene_Stage2.h"
#include "Scene_Stage3.h"
#include "Scene_Stage4.h"
#include "Camera_Free.h"
#include "BackGround.h"

CScene_Loading::CScene_Loading(PDIRECT3DDEVICE9 pGraphic_Device)
	: CScene(pGraphic_Device)
{

}

HRESULT CScene_Loading::Ready_Scene(SCENEID eNextSceneID)
{
	m_eNextSceneID = eNextSceneID;

	if (FAILED(Ready_Layer_Camera(L"Layer_Camera")))
		return E_FAIL;

	if (FAILED(Ready_Layer_BackGround(L"Layer_BackGround")))
		return E_FAIL;

	m_pLoading = CLoading::Create(m_pGraphic_Device, eNextSceneID);
	if (nullptr == m_pLoading)
		return E_FAIL;

	return S_OK;
}

_int CScene_Loading::Update_Scene(_double TimeDelta)
{
	
	
	return _int();
}

HRESULT CScene_Loading::Render_Scene()
{
	if (true == m_pLoading->Get_Finished())
	{
		if (FAILED(Change_Scene()))
			return E_FAIL;

		return S_OK;
	}

	return S_OK;
}

HRESULT CScene_Loading::Change_Scene()
{	
	CManagement*	pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	Safe_AddRef(pManagement);

	CScene*		pScene = nullptr;

	switch (m_eNextSceneID)
	{
	case SCENE_MENU:
		break;

	case SCENE_STAGE:
		pScene = CScene_Stage::Create(m_pGraphic_Device);
		break;

	case SCENE_STAGE2:
		pScene = CScene_Stage2::Create(m_pGraphic_Device);
		break;

	case SCENE_STAGE3:
		pScene = CScene_Stage3::Create(m_pGraphic_Device);
		break;

	case SCENE_STAGE4:
		pScene = CScene_Stage4::Create(m_pGraphic_Device);
		break;

	}

	if (nullptr == pScene)
		return E_FAIL;

	if (FAILED(pManagement->SetUp_CurrentScene(pScene, (_uint)m_eNextSceneID)))
		return E_FAIL;

	Safe_Release(pManagement);
	

	return S_OK;
}

HRESULT CScene_Loading::Ready_Layer_Camera(const _tchar * pLayerTag)
{
	//CManagement*	pManagement = CManagement::Get_Instance();
	//if (nullptr == pManagement)
	//	return E_FAIL;

	//Safe_AddRef(pManagement);

	//CCamera::STATEDESC			StateDesc;
	//StateDesc.vEye = _float3(0.f, 0.0f, -5.f);
	//StateDesc.vAt = _float3(0.f, 0.f, 0.f);
	//StateDesc.vAxisY = _float3(0.f, 1.f, 0.f);

	//StateDesc.fFovy = D3DXToRadian(90.0f);
	//StateDesc.fAspect = _float(g_iWinSizeX) / g_iWinSizeY;
	//StateDesc.fNear = 0.2f;
	//StateDesc.fFar = 300.f;

	//StateDesc.TransformDesc.SpeedPerSec = 5.f;
	//StateDesc.TransformDesc.RotatePerSec = D3DXToRadian(90.0f);

	//if (FAILED(pManagement->Add_Object_ToLayer(SCENE_STATIC, L"GameObject_Camera_Free", SCENE_LOADING, pLayerTag, &StateDesc)))
	//	return E_FAIL;

	//Safe_Release(pManagement);

	return S_OK;
}

HRESULT CScene_Loading::Ready_Layer_BackGround(const _tchar * pLayerTag)
{
	CManagement*	pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	Safe_AddRef(pManagement);

	CBackGround::STATEDESC tBackgroundDesc;
	tBackgroundDesc.eTextureSceneID = SCENE_STATIC;
	tBackgroundDesc.pTextureTag = L"Component_Texture_Background";
	tBackgroundDesc.iTextureID = 2;
	tBackgroundDesc.tBaseDesc = BASEDESC(_float3(g_iWinSizeX * 0.5f, g_iWinSizeY * 0.5f, 0.f), _float3((_float)g_iWinSizeX, (_float)g_iWinSizeY, 1.f));

	if (FAILED(pManagement->Add_Object_ToLayer(SCENE_STATIC, L"GameObject_Background", SCENE_LOADING, L"Layer_Background", &tBackgroundDesc)))
		return E_FAIL;

	Safe_Release(pManagement);

	return S_OK;
}

CScene_Loading * CScene_Loading::Create(PDIRECT3DDEVICE9 pGraphic_Device, SCENEID eNextSceneID)
{
	CScene_Loading*	pInstance = new CScene_Loading(pGraphic_Device);

	if (FAILED(pInstance->Ready_Scene(eNextSceneID)))
	{
		MSG_BOX("Failed To Create CScene_Loading");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CScene_Loading::Free()
{
	Safe_Release(m_pLoading);

	CManagement::Get_Instance()->Clear_Object_Manager(SCENE_LOADING);

	CScene::Free();
}
