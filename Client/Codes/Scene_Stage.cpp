#include "stdafx.h"
#include "..\Headers\Scene_Stage.h"
#include "Management.h"
#include "Camera_Free.h"
#include "KeyMgr.h"
#include "MyButton.h"
#include "GameManager.h"
#include "Cube.h"
#include "CommandCenter.h"
#include "Barricade.h"
#include "PickingMgr.h"
#include "Terrain.h"
#include "Object_Manager.h"
#include "StageUIMgr.h"
CScene_Stage::CScene_Stage(PDIRECT3DDEVICE9 pGraphic_Device)
	: CScene(pGraphic_Device)
{
}

HRESULT CScene_Stage::Ready_Scene()
{
	CManagement*	pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	CKeyMgr* pKeyMgr = CKeyMgr::Get_Instance();
	pKeyMgr->RegisterObserver(SCENE_STAGE1, this);

#pragma region 朝五虞持失

	CCamera::STATEDESC			StateDesc;
	StateDesc.vEye = _float3(TILEX * 0.5f, 20.f, TILEZ * 0.5f);
	//StateDesc.vAt = _float3(TILEX * 0.5f, 0.f, TILEZ * 0.5f);
	StateDesc.vAt = _float3(TILEX * 0.5f, 0.f, TILEZ * 0.5f - 1.f);
	StateDesc.vAxisY = _float3(0.f, 1.f, 0.f);

	StateDesc.fFovy = D3DXToRadian(90.0f);
	StateDesc.fAspect = _float(g_iWinSizeX) / g_iWinSizeY;
	StateDesc.fNear = 0.2f;
	StateDesc.fFar = 300.f;

	StateDesc.TransformDesc.SpeedPerSec = 5.f;
	StateDesc.TransformDesc.RotatePerSec = D3DXToRadian(90.0f);

	if (FAILED(pManagement->Add_Object_ToLayer(SCENE_STATIC, L"GameObject_Camera_Free", SCENE_STAGE1, L"Layer_Camera", &StateDesc)))
		return E_FAIL;

#pragma endregion

	CKeyMgr::Get_Instance()->RegisterObserver(SCENE_STAGE1, this);

	CStageUIMgr::Get_Instance()->Ready_StageUIMgr();

	return S_OK;
}

_int CScene_Stage::Update_Scene(_double TimeDelta)
{
	CPickingMgr* pPickMgr = CPickingMgr::Get_Instance();
	pPickMgr->Update_UI();
	pPickMgr->Check_Mouse();
	pPickMgr->Clear_DeadFocus();
	return _int();

}

HRESULT CScene_Stage::Render_Scene()
{
	CKeyMgr::Get_Instance()->Key_Down(VK_LBUTTON, SCENE_STAGE1);
	CKeyMgr::Get_Instance()->Key_Up(VK_LBUTTON, SCENE_STAGE1);




	CKeyMgr::Get_Instance()->Key_Down('1', SCENE_STAGE1);
	CKeyMgr::Get_Instance()->Key_Down('2', SCENE_STAGE1);

	CKeyMgr::Get_Instance()->Key_Down('3', SCENE_STAGE1);


	CKeyMgr::Get_Instance()->Key_Down(VK_ESCAPE, SCENE_STAGE1);


	CKeyMgr::Get_Instance()->Key_Update();

	return S_OK;
}

HRESULT CScene_Stage::OnKeyDown(_int KeyCode)
{
	return S_OK;
}

CScene_Stage * CScene_Stage::Create(PDIRECT3DDEVICE9 pGraphic_Device)
{
	CScene_Stage*	pInstance = new CScene_Stage(pGraphic_Device);

	if (FAILED(pInstance->Ready_Scene()))
	{
		MSG_BOX("Failed To Create CScene_Stage");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CScene_Stage::Free()
{
	CKeyMgr* pKeyMgr = CKeyMgr::Get_Instance();
	pKeyMgr->UnRegisterObserver(SCENE_STAGE1, this);

	CKeyMgr::Get_Instance()->UnRegisterObserver(SCENE_STAGE1, this);
	CScene::Free();
}

