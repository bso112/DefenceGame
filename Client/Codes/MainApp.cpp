#include "stdafx.h"
#include "..\Headers\MainApp.h"
#include "BackGround.h"
#include "Scene_Loading.h"
#include "Camera_Free.h"
#include "Cube.h"
#include "MyButton.h"
#include "Wall.h"
#include "Field.h"
#include "Trigger.h"
#include "MyImage.h"
#include "GameEndPanel.h"
#include "Image3D.h"
#include "Texture.h"
#include "Terrain.h"
#include "Sky.h"
#include "CommandCenter.h"
#include "Barricade.h"
#include "PickingMgr.h"
#include "Marine.h"
#include "TileUI.h"
#include "AIStateController.h"
#include <time.h>
#include "StageUIMgr.h"
#include "Walker.h"
#include "Runner.h"
#include "AttackTower.h"
#include "BombTower.h"

USING(Client)

CMainApp::CMainApp()
	: m_pManagement(CManagement::Get_Instance())
{
	Safe_AddRef(m_pManagement);

}

HRESULT CMainApp::Ready_MainApp()
{
	if (FAILED(Ready_Default_Setting()))
		return E_FAIL;

	if (FAILED(Ready_Default_Component()))
		return E_FAIL;

	if (FAILED(Ready_Default_GameObject()))
		return E_FAIL;

	if (FAILED(Ready_Start_Scene(SCENE_STAGE1)))
		return E_FAIL;

	srand(unsigned int(time(NULL)));
	CFontManager::Get_Instance()->Ready_FontManager(m_pGraphic_Device);
	return S_OK;
}

_int CMainApp::Update_MainApp(_double TimeDelta)
{
	if (0x8000 & GetKeyState(VK_LBUTTON))
	{
		int a = 10;
	}

	if (nullptr == m_pManagement)
		return -1;


	return m_pManagement->Update_Engine(TimeDelta);
}

HRESULT CMainApp::Render_MainApp()
{
	if (nullptr == m_pGraphic_Device ||
		nullptr == m_pManagement ||
		nullptr == m_pRenderer)
		return E_FAIL;



	m_pGraphic_Device->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, D3DXCOLOR(0.f, 0.f, 1.f, 1.f), 1.f, 0);

	m_pGraphic_Device->BeginScene();

	if (FAILED(m_pRenderer->Draw_RenderGroup()))
		return E_FAIL;

	if (FAILED(m_pManagement->Render_Engine()))
		return E_FAIL;

	m_pGraphic_Device->EndScene();

	m_pGraphic_Device->Present(nullptr, nullptr, 0, nullptr);

	return S_OK;
}

HRESULT CMainApp::Ready_Default_Setting()
{
	if (nullptr == m_pManagement)
		return E_FAIL;

	// 엔진 초기화.
	if (FAILED(m_pManagement->Ready_Engine(SCENE_END)))
		return E_FAIL;

	// 장치 초기화.
	if (FAILED(m_pManagement->Ready_Graphic_Device(g_hWnd, CGraphic_Device::TYPE_WIN, g_iWinSizeX, g_iWinSizeY, &m_pGraphic_Device)))
		return E_FAIL;

	if (FAILED(m_pManagement->Ready_Input_Device(g_hInst, g_hWnd)))
		return E_FAIL;

	return S_OK;
}

HRESULT CMainApp::Ready_Start_Scene(SCENEID eNextSceneID)
{
	if (nullptr == m_pManagement)
		return E_FAIL;

	if (FAILED(m_pManagement->SetUp_CurrentScene(CScene_Loading::Create(m_pGraphic_Device, eNextSceneID), eNextSceneID)))
		return E_FAIL;

	return S_OK;
}

HRESULT CMainApp::Ready_Default_GameObject()
{
	if (nullptr == m_pManagement)
		return E_FAIL;

	if (FAILED(m_pManagement->Add_GameObject_Prototype(SCENE_STATIC, L"GameObject_BackGround", CBackGround::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pManagement->Add_GameObject_Prototype(SCENE_STATIC, L"GameObject_Camera_Free", CCamera_Free::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pManagement->Add_GameObject_Prototype(SCENE_STATIC, L"GameObject_Cube", CCube::Create(m_pGraphic_Device))))
		return E_FAIL;
	if (FAILED(m_pManagement->Add_GameObject_Prototype(SCENE_STATIC, L"GameObject_MyButton", CMyButton::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pManagement->Add_GameObject_Prototype(SCENE_STATIC, L"GameObject_MyImage", CMyImage::Create(m_pGraphic_Device))))
		return E_FAIL;


	if (FAILED(m_pManagement->Add_GameObject_Prototype(SCENE_STATIC, L"GameObject_Wall", CWall::Create(m_pGraphic_Device))))
		return E_FAIL;
	if (FAILED(m_pManagement->Add_GameObject_Prototype(SCENE_STATIC, L"GameObject_Background", CBackGround::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pManagement->Add_GameObject_Prototype(SCENE_STATIC, L"GameObject_Trigger", CTrigger::Create(m_pGraphic_Device))))
		return E_FAIL;


	if (FAILED(m_pManagement->Add_GameObject_Prototype(SCENE_STATIC, L"GameObject_Image3D", CImage3D::Create(m_pGraphic_Device))))
		return E_FAIL;


	if (FAILED(m_pManagement->Add_GameObject_Prototype(SCENE_STATIC, L"GameObject_Terrain", CTerrain::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pManagement->Add_GameObject_Prototype(SCENE_STATIC, L"GameObject_Sky", CSky::Create(m_pGraphic_Device))))
		return E_FAIL;


	if (FAILED(m_pManagement->Add_GameObject_Prototype(SCENE_STATIC, L"GameObject_Marine", CMarine::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pManagement->Add_GameObject_Prototype(SCENE_STATIC, L"GameObject_CommandCenter", CCommandCenter::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pManagement->Add_GameObject_Prototype(SCENE_STATIC, L"GameObject_AttackTower", CAttackTower::Create(m_pGraphic_Device))))
		return E_FAIL;


	if (FAILED(m_pManagement->Add_GameObject_Prototype(SCENE_STATIC, L"GameObject_BombTower", CBombTower::Create(m_pGraphic_Device))))
		return E_FAIL;


	if (FAILED(m_pManagement->Add_GameObject_Prototype(SCENE_STATIC, L"GameObject_Barricade", CBarricade::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pManagement->Add_GameObject_Prototype(SCENE_STATIC, L"GameObject_TileUI", CTileUI::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pManagement->Add_GameObject_Prototype(SCENE_STATIC, L"GameObject_Walker", CWalker::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pManagement->Add_GameObject_Prototype(SCENE_STATIC, L"GameObject_Runner", CRunner::Create(m_pGraphic_Device))))
		return E_FAIL;

	return S_OK;
}

HRESULT CMainApp::Ready_Default_Component()
{
	if (nullptr == m_pManagement)
		return E_FAIL;

	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC, L"Component_Renderer", m_pRenderer = CRenderer::Create(m_pGraphic_Device))))
		return E_FAIL;

	Safe_AddRef(m_pRenderer);

	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC, L"Component_Transform", CTransform::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC, L"Component_VIBuffer_Rect", CVIBuffer_Rect::Create(m_pGraphic_Device))))
		return E_FAIL;


	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC, L"Component_VIBuffer_Cube", CVIBuffer_Cube::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC, L"Component_VIBuffer_Brick", CVIBuffer_Brick::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC, L"Component_VIBuffer_ViewPort", CVIBuffer_ViewPort::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC, L"Component_VIBuffer_Terrain", CVIBuffer_Terrain::Create(m_pGraphic_Device, TILEX, TILEZ, TILESIZE))))
		return E_FAIL;


	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC, L"Component_Shader_Rect", CShader::Create(m_pGraphic_Device, L"../Bin/ShaderFiles/Shader_Rect.fx"))))
		return E_FAIL;

	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC, L"Component_Shader_ViewPort", CShader::Create(m_pGraphic_Device, L"../Bin/ShaderFiles/Shader_ViewPort.fx"))))
		return E_FAIL;

	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC, L"Component_Shader_Cube", CShader::Create(m_pGraphic_Device, L"../Bin/ShaderFiles/Shader_Cube.fx"))))
		return E_FAIL;

	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC, L"Component_Shader_Terrain", CShader::Create(m_pGraphic_Device, L"../Bin/ShaderFiles/Shader_Terrain.fx"))))
		return E_FAIL;
	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC, L"Component_Collider_Rect", CCollider_Rect::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC, L"Component_BoxCollider", CCollider_Box::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC, L"Component_Shader_Solid", CShader::Create(m_pGraphic_Device, L"../Bin/ShaderFiles/Shader_Solid.fx"))))
		return E_FAIL;

	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC, L"Component_Shader_Rect_NoImg", CShader::Create(m_pGraphic_Device, L"../Bin/ShaderFiles/Shader_Rect_NoImg.fx"))))
		return E_FAIL;

	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC, L"Component_AIStateCon", CAIStateController::Create(m_pGraphic_Device))))
		return E_FAIL;

#pragma region TEXTURE

	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC, L"Component_Texture_Wall", CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/Wall/Wall%d.png", 1))))
		return E_FAIL;

	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC, L"Component_Texture_Background", CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/Background/background%d.jpg", 3))))
		return E_FAIL;

	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC, L"Component_Texture_Cube", CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/Brick/brick%d.jpg", 5))))
		return E_FAIL;

	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC, L"Component_Texture_Btn", CTexture::Create(m_pGraphic_Device, L"../../Client/Bin/Resources/Textures/Button/btn%d.png", 2))))
		return E_FAIL;


	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC, L"Component_Texture_Trigger", CTexture::Create(m_pGraphic_Device, L"../../Client/Bin/Resources/Textures/Trigger/trigger%d.png", 2))))
		return E_FAIL;

	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC, L"Component_Texture_Banner", CTexture::Create(m_pGraphic_Device, L"../../Client/Bin/Resources/Textures/Banner/banner%d.png", 1))))
		return E_FAIL;

	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC, L"Component_Texture_SkyBox", CTexture::Create(m_pGraphic_Device, L"../../Client/Bin/Resources/Textures/SkyBox/skybox%d.dds", 1, CTexture::TYPE_CUBE))))
		return E_FAIL;

	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC, L"Component_Texture_Terrain", CTexture::Create(m_pGraphic_Device, L"../../Client/Bin/Resources/Textures/Terrain/grass%d.tga", 1))))
		return E_FAIL;

	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC, L"Component_Texture_Barricade", CTexture::Create(m_pGraphic_Device, L"../../Client/Bin/Resources/Textures/UI/Button/barricade.png"))))
		return E_FAIL;

	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC, L"Component_Texture_CommandCenter", CTexture::Create(m_pGraphic_Device, L"../../Client/Bin/Resources/Textures/UI/Button/commandcenter.png"))))
		return E_FAIL;

	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC, L"Component_Texture_Unit1", CTexture::Create(m_pGraphic_Device, L"../../Client/Bin/Resources/Textures/UI/Button/unit1.png"))))
		return E_FAIL;

	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC, L"Component_Texture_Unit2", CTexture::Create(m_pGraphic_Device, L"../../Client/Bin/Resources/Textures/UI/Button/unit2.png"))))
		return E_FAIL;

	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC, L"Component_Texture_Unit3", CTexture::Create(m_pGraphic_Device, L"../../Client/Bin/Resources/Textures/UI/Button/unit3.png"))))
		return E_FAIL;

	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC, L"Component_Texture_Wave", CTexture::Create(m_pGraphic_Device, L"../../Client/Bin/Resources/Textures/UI/Button/wave_indicator.png"))))
		return E_FAIL;

	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC, L"Component_Texture_Gold", CTexture::Create(m_pGraphic_Device, L"../../Client/Bin/Resources/Textures/UI/Button/gold.png"))))
		return E_FAIL;

	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC, L"Component_Texture_StatsUI", CTexture::Create(m_pGraphic_Device, L"../../Client/Bin/Resources/Textures/UI/Button/statsUI.png"))))
		return E_FAIL;

	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC, L"Component_Texture_Resell", CTexture::Create(m_pGraphic_Device, L"../../Client/Bin/Resources/Textures/UI/Button/Resell.png"))))
		return E_FAIL;


	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC, L"Component_Texture_Marine", CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/Unit/marine%d.png", 1))))
		return E_FAIL;
	
	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC, L"Component_Texture_Explosion", CTexture::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/Effect/explosion%d.png", 1))))
		return E_FAIL;

#pragma endregion


	return S_OK;
}

CMainApp * CMainApp::Create()
{
	CMainApp*	pInstance = new CMainApp();

	if (FAILED(pInstance->Ready_MainApp()))
	{
		MSG_BOX("Failed To Create CMainApp");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CMainApp::Free()
{
	Safe_Release(m_pRenderer);
	Safe_Release(m_pGraphic_Device);
	Safe_Release(m_pManagement);

	CPickingMgr::Get_Instance()->Destroy_Instance();
	CStageUIMgr::Get_Instance()->Destroy_Instance();
	CGameManager::Get_Instance()->Destroy_Instance();
	CFontManager::Get_Instance()->Destroy_Instance();

	CManagement::Release_Engine();
}