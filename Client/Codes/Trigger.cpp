#include "stdafx.h"
#include "Trigger.h"
#include "Cube.h"
#include "GameManager.h"
#include "Wall.h"
#include "Image3D.h"

CTrigger::CTrigger(PDIRECT3DDEVICE9 pGraphic_Device)
	:CGameObject(pGraphic_Device)
{
}

CTrigger::CTrigger(const CTrigger & rhs)
	: CGameObject(rhs)
{
}

HRESULT CTrigger::Ready_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CTrigger::Ready_GameObject(void * pArg)
{
	if (nullptr != pArg)
		memcpy(&m_tDesc, pArg, sizeof(STATEDESC));

	if (FAILED(Add_Component(SCENE_STATIC, L"Component_Transform", L"Com_Transform", (CComponent**)&m_pTransform)))
		return E_FAIL;

	if (FAILED(Add_Component(m_tDesc.eTextureSceneID, m_tDesc.pTextureTag, L"Com_Texture", (CComponent**)&m_pTexture)))
		return E_FAIL;


	if (FAILED(Add_Component(SCENE_STATIC, L"Component_Shader_Rect", L"Com_Shader_Rect", (CComponent**)&m_pShader)))
		return E_FAIL;

	if (FAILED(Add_Component(SCENE_STATIC, L"Component_VIBuffer_Cube", L"Com_VIBuffer_Cube", (CComponent**)&m_pVIBuffer)))
		return E_FAIL;

	if (FAILED(Add_Component(SCENE_STATIC, L"Component_Renderer", L"Com_Renderer", (CComponent**)&m_pRenderer)))
		return E_FAIL;

	if (FAILED(Add_Component(SCENE_STATIC, L"Component_BoxCollider", L"Com_Collider", (CComponent**)&m_pBoxCollider)))
		return E_FAIL;

	m_pTransform->SetUp_Position(m_tDesc.tBaseDesc.vPos);
	m_pTransform->SetUp_Scale(m_tDesc.tBaseDesc.vSize);

	_float3 vGoalPos = m_tDesc.tBaseDesc.vPos;
	_float3 vGoalSize = m_tDesc.tBaseDesc.vSize;
	
	
	CManagement* pManagment = CManagement::Get_Instance();
	if (nullptr == pManagment) return E_FAIL;

	CTransform* pLightTransform = nullptr;
	CGameObject* pLightImage = nullptr;

	CImage3D::STATEDESC tImgDesc;
	tImgDesc.eTextureSceneID = SCENE_STATIC;
	tImgDesc.pTextureTag = L"Component_Texture_Goal";
	tImgDesc.iTextureID = 1;
	tImgDesc.tBaseDesc = BASEDESC(_float3(vGoalPos.x + vGoalSize.x * 0.5f, vGoalPos.y, vGoalPos.z - vGoalSize.x * 0.5f), vGoalSize);
	
	if (nullptr == (pLightImage = pManagment->Add_Object_ToLayer(SCENE_STATIC, L"GameObject_Image3D", m_tDesc.eSceneID, L"GameObject", &tImgDesc)))
		return E_FAIL;

	pLightTransform = (CTransform*)pLightImage->Find_Component(L"Com_Transform");
	if (nullptr == pLightTransform) return E_FAIL;
	pLightTransform->SetUp_Rotation(_float3(0.f, 1.f, 0.f), D3DXToRadian(90.f));


	tImgDesc.tBaseDesc = BASEDESC(_float3(vGoalPos.x - vGoalSize.x * 0.5f, vGoalPos.y, vGoalPos.z - vGoalSize.x * 0.5f), vGoalSize);

	if (nullptr == (pLightImage = pManagment->Add_Object_ToLayer(SCENE_STATIC, L"GameObject_Image3D", m_tDesc.eSceneID, L"GameObject", &tImgDesc)))
		return E_FAIL;

	pLightTransform = (CTransform*)pLightImage->Find_Component(L"Com_Transform");
	if (nullptr == pLightTransform) return E_FAIL;
	pLightTransform->SetUp_Rotation(_float3(0.f, 1.f, 0.f), D3DXToRadian(90.f));




	tImgDesc.tBaseDesc = BASEDESC(_float3(vGoalPos.x, vGoalPos.y + vGoalSize.y * 0.5f, vGoalPos.z - vGoalSize.x * 0.5f), vGoalSize);

	if (nullptr == (pLightImage = pManagment->Add_Object_ToLayer(SCENE_STATIC, L"GameObject_Image3D", m_tDesc.eSceneID, L"GameObject", &tImgDesc)))
		return E_FAIL;

	pLightTransform = (CTransform*)pLightImage->Find_Component(L"Com_Transform");
	if (nullptr == pLightTransform) return E_FAIL;
	pLightTransform->Rotate(_float3(0.f, 0.f, 1.f), D3DXToRadian(-90.f));
	pLightTransform->Rotate(_float3(1.f, 0.f, 0.f), D3DXToRadian(90.f));





	tImgDesc.tBaseDesc = BASEDESC(_float3(vGoalPos.x, vGoalPos.y - vGoalSize.y * 0.5f, vGoalPos.z - vGoalSize.x * 0.5f), vGoalSize);

	if (nullptr == (pLightImage = pManagment->Add_Object_ToLayer(SCENE_STATIC, L"GameObject_Image3D", m_tDesc.eSceneID, L"GameObject", &tImgDesc)))
		return E_FAIL;

	pLightTransform = (CTransform*)pLightImage->Find_Component(L"Com_Transform");
	if (nullptr == pLightTransform) return E_FAIL;
	pLightTransform->Rotate(_float3(0.f, 0.f, 1.f), D3DXToRadian(-90.f));
	pLightTransform->Rotate(_float3(1.f, 0.f, 0.f), D3DXToRadian(90.f));












	
	return S_OK;
}

_int CTrigger::Update_GameObject(_double TimeDelta)
{
	if (nullptr == m_pBoxCollider ||
		nullptr == m_pTransform)
		return E_FAIL;

	m_pBoxCollider->Update_Collider(m_pTransform->Get_WorldMatrix());
	return _int();
}

_int CTrigger::Late_Update_GameObject(_double TimeDelta)
{
	if (nullptr == m_pRenderer) return -1;
	m_pRenderer->Add_RenderGroup(CRenderer::RENDER_ALPHABLEND, this);
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement) return -1;
	pManagement->Add_CollisionGroup(CCollisionMgr::COL_BOX, this);
	return _int();
}

HRESULT CTrigger::Render_GameObject()
{
	_matrix			matView, matProj;

	CManagement* pEnginMgr = CManagement::Get_Instance();
	if (nullptr == pEnginMgr) return E_FAIL;
	matView = pEnginMgr->Get_Transform(D3DTS_VIEW);
	matProj = pEnginMgr->Get_Transform(D3DTS_PROJECTION);

	ALPHABLEND;
	if (FAILED(m_pTexture->Set_TextureOnShader(m_pShader, "g_BaseTexture", m_tDesc.iTextureID)))
		return E_FAIL;

	if (FAILED(m_pShader->Set_Value("g_matWorld", &m_pTransform->Get_WorldMatrix(), sizeof(_matrix))))
		return E_FAIL;
	if (FAILED(m_pShader->Set_Value("g_matView", &matView, sizeof(_matrix))))
		return E_FAIL;
	if (FAILED(m_pShader->Set_Value("g_matProj", &matProj, sizeof(_matrix))))
		return E_FAIL;

	if (FAILED(m_pShader->Begin_Shader()))
		return E_FAIL;

	if (FAILED(m_pShader->Begin_Pass(m_tDesc.iShaderPass)))
		return E_FAIL;

	if (FAILED(m_pVIBuffer->Render_VIBuffer()))
		return E_FAIL;

	if (FAILED(m_pShader->End_Pass()))
		return E_FAIL;
	if (FAILED(m_pShader->End_Shader()))
		return E_FAIL;

	ALPHABLEND_END;
	return S_OK;
}

void CTrigger::OnCollisionEnter(CGameObject * _pOther)
{


}

CTrigger * CTrigger::Create(PDIRECT3DDEVICE9 pGraphic_Device)
{
	CTrigger*	pInstance = new CTrigger(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		MSG_BOX("Failed To Create CGoal");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CTrigger * CTrigger::Clone_GameObject(void * pArg)
{
	CTrigger*	pInstance = new CTrigger(*this);

	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		MSG_BOX("Failed To Create CGoal");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CTrigger::Free()
{
	Safe_Release(m_pRenderer);
	Safe_Release(m_pShader);
	Safe_Release(m_pBoxCollider);
	Safe_Release(m_pTexture);
	Safe_Release(m_pTransform);
	Safe_Release(m_pVIBuffer);
	CGameObject::Free();
}
